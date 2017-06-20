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
void ROUNDEDTRACKSCORNERS::ConvertSegmentsArc(const TRACK* aTrackFrom, PICKED_ITEMS_LIST* aUndoRedoList)
{
    std::unique_ptr<NET_SCAN_TRACK_CORNER_CONVERT> arc_convert(new NET_SCAN_TRACK_CORNER_CONVERT(aTrackFrom, this, aUndoRedoList));
    if(arc_convert)
    {
        arc_convert->Execute();
        
    }
}

ROUNDEDTRACKSCORNERS::NET_SCAN_TRACK_CORNER_CONVERT::NET_SCAN_TRACK_CORNER_CONVERT(const TRACK* aTrackSeg, const ROUNDEDTRACKSCORNERS* aParent, PICKED_ITEMS_LIST* aUndoRedoList) : NET_SCAN_BASE(aTrackSeg, aParent)
{
    m_picked_items = aUndoRedoList;
    m_track_length = aTrackSeg->GetLength();
    m_collected_segments.clear();
}

bool ROUNDEDTRACKSCORNERS::NET_SCAN_TRACK_CORNER_CONVERT::ExecuteAt(const TRACK* aTrackSeg)
{
    if(aTrackSeg->Type() == PCB_TRACE_T)
    {
        if(aTrackSeg->GetLength() == m_track_length)
        {
            m_collected_segments.insert(const_cast<TRACK*>(aTrackSeg));
            return false;
        }
        else
        {
            if(!m_reverse)
            {
                m_reverse = true;
                return false;
            }
        }
    }
    return true;
}

//-----------------------------------------------------------------------------------------------------/

