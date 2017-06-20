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

#include <ratsnest_data.h>
using namespace TrackNodeItem;

//-----------------------------------------------------------------------------------------------------/
// PLOT
//-----------------------------------------------------------------------------------------------------/
void ROUNDEDTRACKSCORNERS::Plot(const TRACKNODEITEM* aTrackNodeItem, PLOTTER* aPlotter, const EDA_DRAW_MODE_T* aPlotMode, void* aData)
{
    if(aTrackNodeItem && dynamic_cast<ROUNDEDTRACKSCORNER*>(const_cast<TRACKNODEITEM*>(aTrackNodeItem)))
    {
        ROUNDEDTRACKSCORNER* corner = static_cast<ROUNDEDTRACKSCORNER*>(const_cast<TRACKNODEITEM*>(aTrackNodeItem));
        if(corner->IsSetOK())
        {
            std::vector<wxPoint> corners;
            corners.clear();
            for(uint n = 0; n < corner->GetPolyPointsNum() - 1; n++)
            {
                corners.push_back(corner->GetPolyPoint(n));
                aPlotter->ThickSegment(corner->GetPolyPoint(n), corner->GetPolyPoint(n+1), corner->GetWidth(), *aPlotMode, aData );
            }
        }
    }
}
//-----------------------------------------------------------------------------------------------------/

//-----------------------------------------------------------------------------------------------------/
// Convert segments arc to rounded corner.
//-----------------------------------------------------------------------------------------------------/
void ROUNDEDTRACKSCORNERS::ConvertSegmentedCorners(const TRACK* aTrackFrom, const bool aUndo)
{
    PICKED_ITEMS_LIST undoredo_items;
    ConvertSegmentedCorners(aTrackFrom, &undoredo_items);
    if(m_EditFrame && aUndo && undoredo_items.GetCount() )
        m_EditFrame->SaveCopyInUndoList(undoredo_items, UR_CHANGED);
}

void ROUNDEDTRACKSCORNERS::ConvertSegmentedCorners(const TRACK* aTrackFrom, PICKED_ITEMS_LIST* aUndoRedoList)
{
    std::unique_ptr<NET_SCAN_TRACK_COLLECT_SAME_LENGTH> same_lengt_tracks(new NET_SCAN_TRACK_COLLECT_SAME_LENGTH(aTrackFrom, this, aUndoRedoList));
    if(same_lengt_tracks)
    {
        same_lengt_tracks->Execute();
        std::set<TRACK*>* tracks_same = same_lengt_tracks->GetSegments();
        
        std::set<TRACK*> tracks_collected;
        tracks_collected.clear();
        tracks_collected.insert(const_cast<TRACK*>(aTrackFrom));
        
        uint num_collected = 0;
        do
        {
            num_collected = tracks_collected.size();
            for(auto t : *tracks_same)
            {
                if(t)
                {
                    wxPoint t_start = t->GetStart();
                    wxPoint t_end = t->GetEnd();
                    for(auto t2 : tracks_collected)
                    {
                        if(t2)
                        {
                            wxPoint t2_start = t2->GetStart();
                            wxPoint t2_end = t2->GetEnd();
                            if((t_start == t2_start) || (t_start == t2_end) || (t_end == t2_start) || (t_end == t2_end))
                                tracks_collected.insert(t);
                        }
                    }
                }
            }
        }
        while(num_collected != tracks_collected.size());
        
        if(tracks_collected.size() > 5)
        {
            ITEM_PICKER picker( NULL, UR_DELETED );
            for(auto track : tracks_collected)
            {
                GetBoard()->TrackItems()->Teardrops()->Remove(track, aUndoRedoList, true );
                Remove(track, aUndoRedoList, true );
                
                GetBoard()->Remove(track);
                GetBoard()->GetRatsnest()->Remove(track);
                picker.SetItem(track);
                aUndoRedoList->PushItem(picker);
            }
        }
    }
}
    
ROUNDEDTRACKSCORNERS::NET_SCAN_TRACK_COLLECT_SAME_LENGTH::NET_SCAN_TRACK_COLLECT_SAME_LENGTH(const TRACK* aTrackSeg, const ROUNDEDTRACKSCORNERS* aParent, PICKED_ITEMS_LIST* aUndoRedoList) : NET_SCAN_BASE(aTrackSeg, aParent)
{
    m_picked_items = aUndoRedoList;
    m_track_length = aTrackSeg->GetLength() / ROUND_DIVIDER;
    m_collected_segments.clear();
}

bool ROUNDEDTRACKSCORNERS::NET_SCAN_TRACK_COLLECT_SAME_LENGTH::ExecuteAt(TRACK* aTrackSeg)
{
    if(aTrackSeg->Type() == PCB_TRACE_T)
    {
        uint length = aTrackSeg->GetLength() / ROUND_DIVIDER;
        if(length == m_track_length)
            m_collected_segments.insert(aTrackSeg);
    }
    return false;
}

//-----------------------------------------------------------------------------------------------------/

