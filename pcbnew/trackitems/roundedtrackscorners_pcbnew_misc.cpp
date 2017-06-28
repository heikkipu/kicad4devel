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
        std::set<TRACK*>* tracks_other = samelengt_tracks->GetAnotherSegments();
        
        //Find same length segments connected to aTrackFrom.
        std::set<TRACK*> tracks_arced = CollectSameLengthConnected(aTrackFrom, tracks_samelength);
        
        //Create rounded tracks corner
        if(tracks_arced.size() > 5) //Bunch of segments not only one or two.
        {
            //Find two endpoint segments connected with arced segments.
            std::map<TRACK*, bool> tracks_connected2arced = FindSegmentsBothSidesOfArced(&tracks_arced, tracks_other);
            
            if(tracks_connected2arced.size() == 2) //Only two segments are acceptable.
            {
                CreateCorner(&tracks_arced, &tracks_connected2arced, aUndoRedoList);
            }
        }
    }
}

void ROUNDEDTRACKSCORNERS::ConvertSegmentedCorners(const int aNetCode, const bool aUndo)
{
    PICKED_ITEMS_LIST undoredo_items;
    DLIST<TRACK>* tracks_list = &m_Parent->GetBoard()->m_Track;
    TRACK* track_seg = nullptr;
    bool created = false;
    do
    {
        created = false;
        track_seg = tracks_list->GetFirst()->GetStartNetCode(aNetCode);
        do
        {
            std::unique_ptr<NET_SCAN_TRACK_COLLECT_SAMELENGTH> samelengt_tracks(new NET_SCAN_TRACK_COLLECT_SAMELENGTH(track_seg, this, &undoredo_items));
            if(samelengt_tracks)
            {
                samelengt_tracks->Execute();
                std::set<TRACK*>* tracks_samelength = samelengt_tracks->GetSamelengthSegments();
                std::set<TRACK*>* tracks_other = samelengt_tracks->GetAnotherSegments();
                
                //Find same length segments connected to aTrackFrom.
                std::set<TRACK*> tracks_arced = CollectSameLengthConnected(track_seg, tracks_samelength);
                
                //Create rounded tracks corner
                if(tracks_arced.size() > 5) //Bunch of segments not only one or two.
                {
                    //Find two endpoint segments connected with arced segments.
                    std::map<TRACK*, bool> tracks_connected2arced = FindSegmentsBothSidesOfArced(&tracks_arced, tracks_other);
                    
                    if(tracks_connected2arced.size() == 2) //Only two segments are acceptable.
                    {
                        if(CreateCorner(&tracks_arced, &tracks_connected2arced, &undoredo_items))
                            created = true;
                    }
                }
            }
            track_seg = track_seg->Next();
            if(track_seg && (track_seg->GetNetCode() != aNetCode))
                track_seg = nullptr;
        }
        while(track_seg);
    }
    while(created);
    
    Repopulate(aNetCode, &undoredo_items);
    if(m_EditFrame && aUndo && undoredo_items.GetCount() )
        m_EditFrame->SaveCopyInUndoList(undoredo_items, UR_CHANGED);
}

ROUNDEDTRACKSCORNERS::NET_SCAN_TRACK_COLLECT_SAMELENGTH::NET_SCAN_TRACK_COLLECT_SAMELENGTH(const TRACK* aTrackSeg, const ROUNDEDTRACKSCORNERS* aParent, PICKED_ITEMS_LIST* aUndoRedoList) : NET_SCAN_BASE(aTrackSeg, aParent)
{
    m_picked_items = aUndoRedoList;
    m_track_length = round(aTrackSeg->GetLength() / ROUND_DIVIDER);
    m_samelength_segments.clear();
    m_another_segments.clear();
}

bool ROUNDEDTRACKSCORNERS::NET_SCAN_TRACK_COLLECT_SAMELENGTH::ExecuteAt(TRACK* aTrackSeg)
{
    if(aTrackSeg->Type() == PCB_TRACE_T)
    {
        unsigned int length = round(aTrackSeg->GetLength() / ROUND_DIVIDER);
        if(length == m_track_length)
            m_samelength_segments.insert(aTrackSeg);
        else
            m_another_segments.insert(aTrackSeg);
    }
    return false;
}

//Remove individual arced segments
void ROUNDEDTRACKSCORNERS::RemoveArcedSegments(std::set<TRACK*>* aTracksArced, PICKED_ITEMS_LIST* aUndoRedoList)
{
    ITEM_PICKER deleted_picker(nullptr, UR_DELETED);
    for(auto track : *aTracksArced)
    {
        GetBoard()->TrackItems()->Teardrops()->Remove(track, aUndoRedoList, true );
        Remove(track, aUndoRedoList, true );
        
        GetBoard()->Remove(track);
        GetBoard()->GetRatsnest()->Remove(track);
        deleted_picker.SetItem(track);
        aUndoRedoList->PushItem(deleted_picker);
    }
}

std::set<TRACK*> ROUNDEDTRACKSCORNERS::CollectSameLengthConnected(const TRACK* aTrackToConnect, const std::set<TRACK*>* aTracksSameLength)
{
    std::set<TRACK*> tracks_arced;
    tracks_arced.clear();
    tracks_arced.insert(const_cast<TRACK*>(aTrackToConnect));
    unsigned int num_collected = 0;
    do
    {
        num_collected = tracks_arced.size();
        for(auto tSamelength : *aTracksSameLength)
        {
            if(tSamelength)
            {
                wxPoint tSamelength_start = tSamelength->GetStart();
                wxPoint tSamelength_end = tSamelength->GetEnd();
                for(auto tArced : tracks_arced)
                {
                    if(tArced)
                    {
                        wxPoint tArced_start = tArced->GetStart();
                        wxPoint tArced_end = tArced->GetEnd();
                        if((tSamelength_start == tArced_start) || (tSamelength_start == tArced_end) || (tSamelength_end == tArced_start) || (tSamelength_end == tArced_end))
                            tracks_arced.insert(tSamelength);
                    }
                }
            }
        }
    }
    while(num_collected != tracks_arced.size());
    
    return tracks_arced;
}

std::map<TRACK*, bool> ROUNDEDTRACKSCORNERS::FindSegmentsBothSidesOfArced(const std::set<TRACK*>* aTracksArced, const std::set<TRACK*>* aTracksOthers)
{
    std::map<TRACK*, bool> tracks_connected2arced;
    tracks_connected2arced.clear();
    for(auto segArced : *aTracksArced)
    {
        if(segArced)
        {
            wxPoint segArced_start = segArced->GetStart();
            wxPoint segArced_end = segArced->GetEnd();
            for(auto segOther : *aTracksOthers)
            {
                if(segOther)
                {
                    wxPoint segOther_start = segOther->GetStart();
                    wxPoint segOther_end = segOther->GetEnd();
                    if((segArced_start == segOther_start) || (segArced_start == segOther_end) || (segArced_end == segOther_start) || (segArced_end == segOther_end))
                    {
                        bool startpoint = ((segOther_start == segArced_start)||(segOther_start == segArced_end));
                        tracks_connected2arced.insert(std::pair<TRACK*, bool>(segOther, startpoint));
                    }
                }
            }
        }
    }
    return tracks_connected2arced;
}

bool ROUNDEDTRACKSCORNERS::CreateCorner(std::set<TRACK*>* aTracksArced, const std::map<TRACK*, bool>* aBothSidesTracks, PICKED_ITEMS_LIST* aUndoRedoList)
{
    bool okay = false;
    ITEM_PICKER changed_picker(nullptr, UR_CHANGED);
    
    std::map<TRACK*, bool>::iterator connected_tracks_it = const_cast<std::map<TRACK*, bool>*>(aBothSidesTracks)->begin();
    TRACK* first_track = connected_tracks_it->first;
    bool is_first_track_connected_at_startpoint = connected_tracks_it->second;
    wxPoint first_track_connected_pos = first_track->GetEnd();
    wxPoint first_track_opposite_pos = first_track->GetStart();
    if(is_first_track_connected_at_startpoint)
    {
        first_track_connected_pos = first_track->GetStart();
        first_track_opposite_pos = first_track->GetEnd();
    }
    double first_track_angle = TrackSegAngle(first_track, first_track_connected_pos);
    
    connected_tracks_it++;
    TRACK* second_track = connected_tracks_it->first;
    bool is_second_track_connected_at_startpoint = connected_tracks_it->second;
    wxPoint second_track_connected_pos = second_track->GetEnd();
    wxPoint second_track_opposite_pos = second_track->GetStart();
    if(is_second_track_connected_at_startpoint)
    {
        second_track_connected_pos = second_track->GetStart();
        second_track_opposite_pos = second_track->GetEnd();
    }
    double second_track_angle = TrackSegAngle(second_track, second_track_connected_pos);
    
    first_track = Convert(first_track, aUndoRedoList);
    second_track = Convert(second_track, aUndoRedoList);
    changed_picker.SetItem(first_track);
    changed_picker.SetLink(first_track->Clone());
    aUndoRedoList->PushItem(changed_picker);
    changed_picker.SetItem(second_track);
    changed_picker.SetLink(second_track->Clone());
    aUndoRedoList->PushItem(changed_picker);
    
    //Parallel tracks.
    if(Rad2MilsInt(first_track_angle) == Rad2MilsInt(second_track_angle))
    {
        RemoveArcedSegments(aTracksArced, aUndoRedoList);
        
        double half_dist_btw_tracks = GetLineLength(first_track_connected_pos, second_track_connected_pos) / 2.0;
        
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
        okay = true;
    }
    else 
    {
        double angle_btw_tracks = AngleBtwTracks(first_track, first_track_connected_pos, second_track, second_track_connected_pos);
        double virtual_track_length = GetLineLength(first_track_opposite_pos, second_track_opposite_pos);
        
        RemoveArcedSegments(aTracksArced, aUndoRedoList);
        
        angle_btw_tracks = InnerAngle(angle_btw_tracks);
        
        double virtual_trac_angle = AngleRad(first_track_opposite_pos, second_track_opposite_pos);
        
        double first_trac_opposite_angle = TrackSegAngle(first_track, first_track_opposite_pos);
        double angle_first_btw_virtual = NormAngle(first_trac_opposite_angle - virtual_trac_angle, 0.0, M_PIx2, M_PIx2);
        angle_first_btw_virtual = InnerAngle(angle_first_btw_virtual);
        
        double second_track_full_length = virtual_track_length * sin(angle_first_btw_virtual) / sin(angle_btw_tracks);
        double second_track_length = GetLineLength(second_track->GetStart(), second_track->GetEnd());
        
        wxPoint common_point = GetPoint(second_track_opposite_pos, second_track_angle + M_PI, second_track_full_length);
        if(is_first_track_connected_at_startpoint)
            first_track->SetStart(common_point);
        else
            first_track->SetEnd(common_point);

        if(is_second_track_connected_at_startpoint)
            second_track->SetStart(common_point);
        else
            second_track->SetEnd(common_point);
        
        Add(first_track, common_point, second_track_full_length - second_track_length, aUndoRedoList);
        okay = true;
    }
    return okay;
}

//-----------------------------------------------------------------------------------------------------/



