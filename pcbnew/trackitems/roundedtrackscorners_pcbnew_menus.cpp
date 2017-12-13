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
#include <bitmaps.h>
#include <pcbnew_id.h>
#include <menus_helpers.h>

using namespace TrackNodeItem;


void ROUNDED_TRACKS_CORNERS::Popup( wxMenu* aMenu,
                                  const TRACK* aTrackSeg,
                                  const wxPoint& aPos
                                ) const
{
    if( aTrackSeg && aMenu && IsOn() )
    {
        wxString msg;
        wxPoint track_nearest_endpoint = TrackNodeItem::TrackSegNearestEndpoint( aTrackSeg, aPos );
        msg.Printf( _( "Rounded Corners" ) );

        ROUNDED_TRACKS_CORNER* corner = nullptr;
        TRACKNODEITEM* item = Get( aTrackSeg, track_nearest_endpoint );
        if( item && dynamic_cast<ROUNDED_TRACKS_CORNER*>( item ) )
            corner = static_cast<ROUNDED_TRACKS_CORNER*>( item );

        wxMenu* corner_menu = new wxMenu;
        AddMenuItem( aMenu,
                     corner_menu,
                     ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_COMMON_MNU,
                     msg,
                     KiBitmap( add_tracks_xpm ) );

        if( IsOn() )
        {
            if( !corner )
                Menu_AddToTrack( corner_menu, aTrackSeg, track_nearest_endpoint );
            if( corner )
            {
                if( corner->GetParams() != GetParams() )
                {
                    Menu_CopyParamsToCurrent( corner_menu, aTrackSeg, track_nearest_endpoint );
                    Menu_ChangeFromTrack( corner_menu, aTrackSeg, track_nearest_endpoint );
                }
                Menu_RemoveFromTrack( corner_menu, aTrackSeg, track_nearest_endpoint );
            }
            corner_menu->AppendSeparator();
            Menu_AddToNet( corner_menu, aTrackSeg, aPos );
            Menu_RemoveFromNet( corner_menu, aTrackSeg, aPos );
            corner_menu->AppendSeparator();
            Menu_ConvertSegmentedCorner( corner_menu, aTrackSeg, aPos );
            Menu_ConvertSegmentedCornersNet( corner_menu, aTrackSeg, aPos );
            corner_menu->AppendSeparator();
        }
        Menu_ChangeSize( corner_menu );
    }
}

void ROUNDED_TRACKS_CORNERS::Menu_AddToTrack( wxMenu* aMenu,
                                            const TRACK* aTrackSeg,
                                            const wxPoint& aPos
                                          ) const
{
    if( aMenu && aTrackSeg && IsOn() )
    {
        wxString msg;
        msg.Printf( _( "Add Corner %s" ), GetChars( ParamsTxtToMenu( GetParams() ) ) );
        AddMenuItem( aMenu, ID_POPUP_PCB_ROUNDEDTRACKSCORNER_PLACE, msg, KiBitmap( add_arc_xpm ) );
    }
}

void ROUNDED_TRACKS_CORNERS::Menu_RemoveFromTrack( wxMenu* aMenu,
                                                 const TRACK* aTrackSeg,
                                                 const wxPoint& aPos
                                               ) const
{
    if( aMenu && aTrackSeg && IsOn() )
    {
        if( aTrackSeg->Type() == PCB_TRACE_T )
        {
            TRACKNODEITEM* item = Get( aTrackSeg, aPos );
            if( item && dynamic_cast<ROUNDED_TRACKS_CORNER*>( item ) )
            {
                AddMenuItem( aMenu,
                             ID_POPUP_PCB_ROUNDEDTRACKSCORNER_DELETE,
                             _( "Remove Corner" ),
                             KiBitmap( delete_xpm ) );
            }
        }
    }
}

void ROUNDED_TRACKS_CORNERS::Menu_ChangeFromTrack( wxMenu* aMenu,
                                                 const TRACK* aTrackSeg,
                                                 const wxPoint& aPos
                                               ) const
{
    if( aMenu && aTrackSeg && IsOn() )
    {
        if( aTrackSeg->Type() == PCB_TRACE_T )
        {
            TRACKNODEITEM* item = Get( aTrackSeg, aPos );
            if( item && dynamic_cast<ROUNDED_TRACKS_CORNER*>( item ) )
            {
                wxString msg;
                msg.Printf( _( "Change Corner %s" ), GetChars( ParamsTxtToMenu( GetParams() ) ) );
                AddMenuItem( aMenu,
                             ID_POPUP_PCB_ROUNDEDTRACKSCORNER_CHANGE,
                             msg,
                             KiBitmap( add_arc_xpm ) );
            }
        }
    }
}

void ROUNDED_TRACKS_CORNERS::Menu_CopyParamsToCurrent( wxMenu* aMenu,
                                                     const TRACK* aTrackSeg,
                                                     const wxPoint& aPos
                                                   ) const
{
    if( aMenu && aTrackSeg && IsOn() )
    {
        if( aTrackSeg->Type() == PCB_TRACE_T )
        {
            TRACKNODEITEM* item = Get( aTrackSeg, aPos );
            if( item && dynamic_cast<ROUNDED_TRACKS_CORNER*>( item ) )
            {
                wxString msg;
                msg.Printf( _( "Copy Corner Settings %s to Current" ),
                            GetChars( ParamsTxtToMenu( static_cast<ROUNDED_TRACKS_CORNER*>( item )->GetParams() ) ) );
                AddMenuItem( aMenu,
                             ID_POPUP_PCB_ROUNDEDTRACKSCORNER_COPYCURRENT,
                             msg,
                             KiBitmap( tools_xpm ) );
            }
        }
    }
}

void ROUNDED_TRACKS_CORNERS::Menu_AddToNet( wxMenu* aMenu,
                                          const TRACK* aTrackSeg,
                                          const wxPoint& aPos
                                        ) const
{
    if( aMenu && aTrackSeg && IsOn() )
    {
        wxString msg;
        msg.Printf( _( "Add Corners %s to NET: %s" ),
                    GetChars( ParamsTxtToMenu( GetParams() ) ),
                    GetChars( m_Board->FindNet( aTrackSeg->GetNetCode() )->GetShortNetname() ) );

        AddMenuItem( aMenu,
                     ID_POPUP_PCB_ROUNDEDTRACKSCORNER_PLACE_NET,
                     msg,
                     KiBitmap( add_arc_xpm ) );
    }
}

void ROUNDED_TRACKS_CORNERS::Menu_RemoveFromNet( wxMenu* aMenu,
                                               const TRACK* aTrackSeg,
                                               const wxPoint& aPos
                                             ) const
{
    if( aMenu && aTrackSeg && IsOn() )
    {
        if( aTrackSeg->Type() == PCB_TRACE_T )
        {
            wxString msg;
            msg.Printf( _( "Remove Corners from NET: %s" ),
                        GetChars( m_Board->FindNet( aTrackSeg->GetNetCode() )->GetShortNetname() ) );

            AddMenuItem( aMenu,
                         ID_POPUP_PCB_ROUNDEDTRACKSCORNER_DELETE_NET,
                         msg,
                         KiBitmap( delete_xpm ) );
        }
    }
}

void ROUNDED_TRACKS_CORNERS::Menu_ConvertSegmentedCorner( wxMenu* aMenu,
                                                        const TRACK* aTrackSeg,
                                                        const wxPoint& aPos
                                                      ) const
{
    if( aMenu && aTrackSeg && IsOn() )
    {
        AddMenuItem( aMenu,
                     ID_POPUP_PCB_ROUNDEDTRACKSCORNER_CONVERT_SEGMENTED,
                     _( "Convert Segments to Corner" ),
                     KiBitmap( add_arc_xpm ) );
    }
}

void ROUNDED_TRACKS_CORNERS::Menu_ConvertSegmentedCornersNet( wxMenu* aMenu,
                                                            const TRACK* aTrackSeg,
                                                            const wxPoint& aPos
                                                          ) const
{
    if( aMenu && aTrackSeg && IsOn() )
    {
        if( aTrackSeg->Type() == PCB_TRACE_T )
        {
            wxString msg;
            msg.Printf( _( "Convert Segments to Corner from NET: %s" ),
                        GetChars( m_Board->FindNet( aTrackSeg->GetNetCode() )->GetShortNetname() ) );

            AddMenuItem( aMenu,
                         ID_POPUP_PCB_ROUNDEDTRACKSCORNER_CONVERT_SEGMENTED_NET,
                         msg,
                         KiBitmap( add_arc_xpm ) );
        }
    }
}

