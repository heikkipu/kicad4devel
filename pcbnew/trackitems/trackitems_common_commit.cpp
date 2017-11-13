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

#include "trackitems.h"

using namespace TrackNodeItem;

//-----------------------------------------------------------------------------------------------------/
// Gal canvas route commit push.
//-----------------------------------------------------------------------------------------------------/
void TRACKITEMS::GalCommitPushPrepare( void )
{
    m_gal_commit_added_tracks.clear();
}

//Gal canvas add when drag and route tracks.
void TRACKITEMS::GalCommitPushAdd( BOARD_ITEM* aItem, PICKED_ITEMS_LIST* aUndoRedoList )
{
    if( aItem )
    {
        if( aItem->Type() == PCB_TRACE_T )
        {
            TRACK* track = static_cast<TRACK*>( aItem );
            /*
            m_Teardrops->Add( track, aUndoRedoList );
            if( m_RoundedTracksCorners->IsOn() )
            {
                std::pair<ROUNDEDTRACKSCORNER*, ROUNDEDTRACKSCORNER*> tracks_cormers =
                    m_RoundedTracksCorners->Add( track, aUndoRedoList );

                for( int n = 0; n < 2; ++n )
                {
                    ROUNDEDTRACKSCORNER* corner = nullptr;
                    n? corner = tracks_cormers.second : corner = tracks_cormers.first;
                    if( corner )
                    {
                        m_gal_commit_added_tracks.insert( corner->GetTrackSeg() );
                        m_gal_commit_added_tracks.insert( corner->GetTrackSegSecond() );
                    }
                }

            }
            else
                */
                m_gal_commit_added_tracks.insert( track );
        }
    }
}

//Gal canva remove when drag and route tracks.
void TRACKITEMS::GalCommitPushRemove( BOARD_ITEM* aItemFrom, PICKED_ITEMS_LIST* aUndoRedoList )
{
    if( aItemFrom )
    {
        if( aItemFrom->Type() == PCB_TRACE_T )
        {
            m_Teardrops->Remove( static_cast<TRACK*>( aItemFrom ), aUndoRedoList, true );
            m_RoundedTracksCorners->Remove( static_cast<TRACK*>( aItemFrom ), aUndoRedoList, true );
        }
    }
}

void TRACKITEMS::GalCommitPushFinish( PICKED_ITEMS_LIST* aUndoRedoList )
{
    for( TRACK* track : m_gal_commit_added_tracks )
    {
        m_Teardrops->Add( track, aUndoRedoList );
    }

    m_RoundedTracksCorners->Add( &m_gal_commit_added_tracks, aUndoRedoList );
}

//-----------------------------------------------------------------------------------------------------/
