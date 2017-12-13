/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2017- Heikki Pulkkinen.
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


void ROUNDED_TRACKS_CORNERS::UpdateList_DrawTracks_Route( EDA_DRAW_PANEL* aPanel,
                                                          wxDC* aDC,
                                                          const bool aOnlyChanged
                                                        )
{
    if( m_update_tracks_list )
    {
        TRACK* seg_third_back = nullptr;
        TRACK* seg_back = g_CurrentTrackSegment->Back();
        if( seg_back )
        {
            seg_third_back = seg_back->Back();
            for( TRACK* t = seg_third_back; t; t=t->Back() )
                if( t->Type() == PCB_TRACE_T )
                {
                    seg_third_back = t;
                    break;
                }
        }

        for( auto r_t: *m_update_tracks_list )
        {
            if( r_t )
            {
                if( ( r_t == g_CurrentTrackSegment ) ||
                    ( r_t == seg_back ) ||
                    ( r_t == seg_third_back ) )
                {
                    r_t->Draw( aPanel, aDC, GR_XOR );
                }
                else
                    if( !aOnlyChanged )
                        r_t->Draw( aPanel, aDC, GR_OR );
            }
        }
    }
    GRSetDrawMode( aDC, GR_XOR );
}

void ROUNDED_TRACKS_CORNERS::UpdateListDo_Route( EDA_DRAW_PANEL* aPanel,
                                                 wxDC* aDC,
                                                 bool aErase
                                               )
{
    if( m_update_tracks_list )
        for( auto r_t: *m_update_tracks_list )
            r_t->ResetVisibleEndpoints();

    if( m_update_list )
    {
        for( ROUNDED_TRACKS_CORNER* corner : *m_update_list )
            if( corner )
                if( aErase )
                    if( ( corner->GetTrackSeg() == g_CurrentTrackSegment ) ||
                        ( corner->GetTrackSeg() == g_CurrentTrackSegment->Back() ) ||
                        ( corner->GetTrackSegSecond() == g_CurrentTrackSegment ) ||
                        ( corner->GetTrackSegSecond() == g_CurrentTrackSegment->Back() ) )
                    {
                        corner->Draw( aPanel, aDC, GR_XOR );
                    }

        for( ROUNDED_TRACKS_CORNER* corner : *m_update_list )
            if( corner )
                corner->ResetVisibleEndpoints();

        for( ROUNDED_TRACKS_CORNER* corner : *m_update_list )
            if( corner )
                corner->Update();

        for( ROUNDED_TRACKS_CORNER* corner : *m_update_list )
            if( corner )
            {
                if( ( corner->GetTrackSeg() == g_CurrentTrackSegment ) ||
                    ( corner->GetTrackSeg() == g_CurrentTrackSegment->Back() ) ||
                    ( corner->GetTrackSegSecond() == g_CurrentTrackSegment ) ||
                    ( corner->GetTrackSegSecond() == g_CurrentTrackSegment->Back() ) )
                {
                    corner->Draw( aPanel, aDC, GR_XOR );
                }
                else
                    corner->Draw( aPanel, aDC, GR_OR );
            }

        GRSetDrawMode( aDC, GR_XOR );
    }
}

