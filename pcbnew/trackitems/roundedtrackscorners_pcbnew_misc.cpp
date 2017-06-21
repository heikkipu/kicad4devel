/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) Heikki Pulkkinen.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#include "roundedtrackscorners.h"
#include "trackitems.h"

#include <drag.h>
#include <ratsnest_data.h>

using namespace TrackNodeItem;


void ROUNDEDTRACKSCORNERS::AddToDragList(const TRACK* aTrackFrom, std::vector<DRAG_SEGM_PICKER>& aDragSegList)
{
    if(aTrackFrom->Type() == PCB_TRACE_T)
    {
        for(int n = 0; n < 2; ++n)
        {
            ROUNDEDTRACKSCORNER* corner = nullptr;
            n? corner = dynamic_cast<ROUNDEDTRACKSCORNER*>(Back(aTrackFrom)) : corner = dynamic_cast<ROUNDEDTRACKSCORNER*>(Next(aTrackFrom));

            if(corner)
            {
                DRAG_SEGM_PICKER wrapper(corner);
                aDragSegList.push_back(wrapper);
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------------/
// Convert segments arc to rounded corner.
//-----------------------------------------------------------------------------------------------------/
void ROUNDEDTRACKSCORNERS::ConvertSegmentedCorners(TRACK* aTrackFrom, const bool aUndo)
{
    PICKED_ITEMS_LIST undoredo_items;
    ConvertSegmentedCorners(aTrackFrom, &undoredo_items);
    if(m_EditFrame && aUndo && undoredo_items.GetCount() )
        m_EditFrame->SaveCopyInUndoList(undoredo_items, UR_CHANGED);
}

void ROUNDEDTRACKSCORNERS::ConvertSegmentedCorners(TRACK* aTrackFrom, PICKED_ITEMS_LIST* aUndoRedoList)
{
    std::unique_ptr<NET_SCAN_TRACK_COLLECT_SAMELENGTH> samelengt_tracks(new NET_SCAN_TRACK_COLLECT_SAMELENGTH(aTrackFrom, this, aUndoRedoList));
    if(samelengt_tracks)
    {
        samelengt_tracks->Execute();
        std::set<TRACK*>* tracks_samelength = samelengt_tracks->GetSamelengthSegments();
        std::set<TRACK*>* tracks_another = samelengt_tracks->GetAnotherSegments();
        
        //Find segments togeather connected to aTrackFrom.
        std::set<TRACK*> tracks_arced;
        tracks_arced.clear();
        tracks_arced.insert(aTrackFrom);
        unsigned int num_collected = 0;
        do
        {
            num_collected = tracks_arced.size();
            for(auto t : *tracks_samelength)
            {
                if(t)
                {
                    wxPoint t_start = t->GetStart();
                    wxPoint t_end = t->GetEnd();
                    for(auto t2 : tracks_arced)
                    {
                        if(t2)
                        {
                            wxPoint t2_start = t2->GetStart();
                            wxPoint t2_end = t2->GetEnd();
                            if((t_start == t2_start) || (t_start == t2_end) || (t_end == t2_start) || (t_end == t2_end))
                                tracks_arced.insert(t);
                        }
                    }
                }
            }
        }
        while(num_collected != tracks_arced.size());
        
        //Create rounded tracks corner
        if(tracks_arced.size() > 5) //Bunch of segments not only one or two.
        {
            //Find two endpoint segments connected with arced segments.
            std::map<TRACK*, bool> tracks_connected2arced;
            tracks_connected2arced.clear();
            for(auto seg : tracks_arced)
            {
                if(seg)
                {
                    wxPoint seg_start = seg->GetStart();
                    wxPoint seg_end = seg->GetEnd();
                    for(auto seg2 : *tracks_another)
                    {
                        if(seg2)
                        {
                            wxPoint seg2_start = seg2->GetStart();
                            wxPoint seg2_end = seg2->GetEnd();
                            if((seg_start == seg2_start) || (seg_start == seg2_end) || (seg_end == seg2_start) || (seg_end == seg2_end))
                            {
                                bool startpoint = ((seg2_start == seg_start)||(seg2_start == seg_end));
                                tracks_connected2arced.insert(std::pair<TRACK*, bool>(seg2, startpoint));
                            }
                        }
                    }
                }
            }
            
            if(tracks_connected2arced.size() == 2) //Only two segments are acceptable.
            {
                //Remove individual arced segments
                ITEM_PICKER deleted_picker(nullptr, UR_DELETED);
                for(auto track : tracks_arced)
                {
                    GetBoard()->TrackItems()->Teardrops()->Remove(track, aUndoRedoList, true );
                    Remove(track, aUndoRedoList, true );
                    
                    GetBoard()->Remove(track);
                    GetBoard()->GetRatsnest()->Remove(track);
                    deleted_picker.SetItem(track);
                    aUndoRedoList->PushItem(deleted_picker);
                }
                
                //Create new cornersystem.
                std::map<TRACK*, bool>::iterator connected_tracks_it = tracks_connected2arced.begin();
                TRACK* first_track = connected_tracks_it->first;
                bool is_first_track_connected_at_startpoint = connected_tracks_it->second;
                wxPoint first_track_connected_pos = first_track->GetEnd();
                if(is_first_track_connected_at_startpoint)
                    first_track_connected_pos = first_track->GetStart();
                double first_track_angle = TrackSegAngle(first_track, first_track_connected_pos);
                
                connected_tracks_it++;
                TRACK* second_track = connected_tracks_it->first;
                bool is_second_track_connected_at_startpoint = connected_tracks_it->second;
                wxPoint second_track_connected_pos = second_track->GetEnd();
                if(is_second_track_connected_at_startpoint)
                    second_track_connected_pos = second_track->GetStart();
                double second_track_angle = TrackSegAngle(second_track, second_track_connected_pos);
                
                //Parallel tracks.
                if(Rad2MilsInt(first_track_angle) == Rad2MilsInt(second_track_angle))
                {
                    double half_dist_btw_tracks = GetLineLength(first_track_connected_pos, second_track_connected_pos) / 2.0;
                    
                    first_track = ConvertTrackInList(first_track, aUndoRedoList);
                    second_track = ConvertTrackInList(second_track, aUndoRedoList);
                    
                    wxPoint first_track_new_pos = GetPoint(first_track_connected_pos, first_track_angle + M_PI, half_dist_btw_tracks);
                    if(is_first_track_connected_at_startpoint)
                        first_track->SetStart(first_track_new_pos);
                    else
                        first_track->SetEnd(first_track_new_pos);
                    
                    wxPoint second_track_new_pos = GetPoint(second_track_connected_pos, second_track_angle + M_PI, half_dist_btw_tracks);
                    if(is_second_track_connected_at_startpoint)
                        second_track->SetStart(second_track_new_pos);
                    else
                        second_track->SetEnd(second_track_new_pos);
                    
                    ITEM_PICKER added_picker(nullptr, UR_NEW);
                    ROUNDEDCORNERTRACK* new_track((ROUNDEDCORNERTRACK*)first_track->Clone());
                    new_track->SetStart(first_track_new_pos);
                    new_track->SetEnd(second_track_new_pos);
                    added_picker.SetItem(new_track);
                    aUndoRedoList->PushItem(added_picker);
                    GetBoard()->m_Track.Insert(new_track, second_track);
                    
                    Add(new_track, half_dist_btw_tracks + 10, aUndoRedoList);
                }
                
            }
        }
    }
}
    
ROUNDEDTRACKSCORNERS::NET_SCAN_TRACK_COLLECT_SAMELENGTH::NET_SCAN_TRACK_COLLECT_SAMELENGTH(const TRACK* aTrackSeg, const ROUNDEDTRACKSCORNERS* aParent, PICKED_ITEMS_LIST* aUndoRedoList) : NET_SCAN_BASE(aTrackSeg, aParent)
{
    m_picked_items = aUndoRedoList;
    m_track_length = aTrackSeg->GetLength() / ROUND_DIVIDER;
    m_samelength_segments.clear();
    m_another_segments.clear();
}

bool ROUNDEDTRACKSCORNERS::NET_SCAN_TRACK_COLLECT_SAMELENGTH::ExecuteAt(TRACK* aTrackSeg)
{
    if(aTrackSeg->Type() == PCB_TRACE_T)
    {
        unsigned int length = aTrackSeg->GetLength() / ROUND_DIVIDER;
        if(length == m_track_length)
            m_samelength_segments.insert(aTrackSeg);
        else
            m_another_segments.insert(aTrackSeg);
    }
    return false;
}

//-----------------------------------------------------------------------------------------------------/



