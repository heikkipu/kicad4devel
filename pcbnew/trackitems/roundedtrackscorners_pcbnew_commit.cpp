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


//Gal canvas add when drag and route tracks.
void ROUNDEDTRACKSCORNERS::GalCommitPushAdd( BOARD_ITEM* aItem, PICKED_ITEMS_LIST* aUndoRedoList )
{
    if( aItem )
    {
        if( aItem->Type() == PCB_TRACE_T )
        {
            m_gal_commit_tracks.insert( static_cast<TRACK*>( aItem ) );
        }
    }
}

//Gal canva remove when drag and route tracks.
void ROUNDEDTRACKSCORNERS::GalCommitPushRemove( BOARD_ITEM* aItemFrom, PICKED_ITEMS_LIST* aUndoRedoList )
{
    if( dynamic_cast<ROUNDEDCORNERTRACK*>( aItemFrom ) )
        Remove( static_cast<ROUNDEDCORNERTRACK*>( aItemFrom ), aUndoRedoList, true );
}

void ROUNDEDTRACKSCORNERS::GalCommitPushPrepare( void )
{
    m_gal_removed_list->clear();
    m_gal_commit_tracks.clear();
}

void ROUNDEDTRACKSCORNERS::GalCommitPushFinish( PICKED_ITEMS_LIST* aUndoRedoList )
{
    for( TRACK* track : m_gal_commit_tracks )
        Add( track, aUndoRedoList );
    
    m_gal_removed_list->clear();
    m_gal_commit_tracks.clear();
}

