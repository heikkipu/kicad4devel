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
#include "teardrops.h"
#include "trackitems.h"

#include <ratsnest_data.h>
#include <view/view.h> //Gal canvas

using namespace TrackNodeItem;

const wxString ROUNDED_TRACKS_CORNERS::TXT_ROUNDEDTRACKSCORNERS = _( "Rounded Tracks Corners" );

ROUNDED_TRACKS_CORNERS::ROUNDED_TRACKS_CORNERS( const TRACKITEMS* aParent, const BOARD* aBoard ) :
    TRACKNODEITEMS( aParent, aBoard )
{
    m_Board = const_cast<BOARD*>( aBoard );
    m_EditFrame = nullptr;
    m_menu = nullptr;

    m_update_list = new RoundedTracksCorner_Container;
    m_update_list->clear();

    m_update_tracks_list = new RoundedCornerTrack_Container;;
    m_update_tracks_list->clear();

    m_recreate_list = new RoundedTracksCorner_Container;
    m_recreate_list->clear();

    m_to_edit = EDIT_NULL_T;
    m_can_edit = false;
    m_track_edit_corner = nullptr;

    LoadDefaultParams();
    m_params.length_ratio = 0; //Put corners to off mode in start.
}

ROUNDED_TRACKS_CORNERS::~ROUNDED_TRACKS_CORNERS()
{
    delete m_update_list;
    m_update_list = nullptr;

    delete m_update_tracks_list;
    m_update_tracks_list = nullptr;

    delete m_recreate_list;
    m_recreate_list = nullptr;
}

ROUNDED_TRACKS_CORNER* ROUNDED_TRACKS_CORNERS::Create( const TRACK* aTrackTo,
                                                   const TRACK* aTrackSegSecond,
                                                   const wxPoint aPosition,
                                                   const bool aNullTrackCheck
                                                 )
{
    ROUNDED_TRACKS_CORNER* corner = nullptr;

    if( aTrackTo && aTrackSegSecond )
    {
        if( dynamic_cast<ROUNDED_CORNER_TRACK*>( const_cast<TRACK*>( aTrackTo ) ) &&
            dynamic_cast<ROUNDED_CORNER_TRACK*>( const_cast<TRACK*>( aTrackSegSecond ) ) )
        {
            //Not in pad or via:
            if( const_cast<TRACK*>( aTrackTo )->GetVia( aPosition, aTrackTo->GetLayer() ) )
                return nullptr;

            BOARD_CONNECTED_ITEM* lock_point = m_Board->GetLockPoint( aPosition, aTrackTo->GetLayerSet() );
            if( lock_point && ( lock_point->Type() == PCB_PAD_T ) )
                return nullptr;

            lock_point = m_Board->GetLockPoint( aPosition, aTrackSegSecond->GetLayerSet() );
            if( lock_point && ( lock_point->Type() == PCB_PAD_T ) )
                return nullptr;

            ROUNDED_CORNER_TRACK* trackseg = static_cast<ROUNDED_CORNER_TRACK*>( const_cast<TRACK*>( aTrackTo ) );
            ROUNDED_TRACKS_CORNER* trackseg_corner_atpos = nullptr;
            if( aPosition == trackseg->GetStart() )
                trackseg_corner_atpos = trackseg->GetStartPointCorner();
            if( aPosition == trackseg->GetEnd() )
                trackseg_corner_atpos = trackseg->GetEndPointCorner();

            ROUNDED_CORNER_TRACK* tracksegsecond = static_cast<ROUNDED_CORNER_TRACK*>( const_cast<TRACK*>( aTrackSegSecond ) );
            ROUNDED_TRACKS_CORNER* tracksegsecond_corner_atpos = nullptr;
            if( aPosition == tracksegsecond->GetStart() )
                tracksegsecond_corner_atpos = tracksegsecond->GetStartPointCorner();
            if( aPosition == tracksegsecond->GetEnd() )
                trackseg_corner_atpos = tracksegsecond->GetEndPointCorner();

            if( !trackseg_corner_atpos && !tracksegsecond_corner_atpos &&
                !m_Parent->Teardrops()->Get( aTrackTo, aPosition, true ) &&
                !m_Parent->Teardrops()->Get( aTrackSegSecond, aPosition, true ) )
            {

                ROUNDED_TRACKS_CORNER::PARAMS params = GetParams();
                corner = new ROUNDED_TRACKS_CORNER( m_Board,
                                                  aTrackTo,
                                                  aTrackSegSecond,
                                                  aPosition,
                                                  params,
                                                  aNullTrackCheck );

                if( corner )
                {
                    if( !corner->IsCreatedOK() )
                    {
                        delete corner;
                        corner = nullptr;
                    }
                    else
                    {
                        m_Parent->Teardrops()->Update( corner->GetTrackSeg() );
                        m_Parent->Teardrops()->Update( corner->GetTrackSegSecond() );

                        //GAL View.
                        if( m_EditFrame && m_EditFrame->IsGalCanvasActive() )
                        {
                            m_EditFrame->GetGalCanvas()->GetView()->Add( corner );
                            m_EditFrame->GetGalCanvas()->GetView()->Update( corner->GetTrackSeg(), KIGFX::GEOMETRY );
                            m_EditFrame->GetGalCanvas()->GetView()->Update( corner->GetTrackSegSecond(), KIGFX::GEOMETRY );
                            //Update teardrops in GAL
                            std::set<TRACK*> commit_container;
                            m_Parent->Teardrops()->CollectCommit( corner->GetTrackSeg(), &commit_container, true );
                            m_Parent->Teardrops()->CollectCommit( corner->GetTrackSegSecond(), &commit_container, true );
                            for( auto tear : commit_container )
                                if( tear )
                                    m_EditFrame->GetGalCanvas()->GetView()->Update( tear, KIGFX::GEOMETRY );
                        }

#ifdef NEWCONALGO
#ifndef MYCONALGO
                        //New connectivity algo add
                        m_Board->GetConnectivity()->Add( corner );
#endif
#endif
                    }
                }
            }
        }
    }
    return corner;
}

void ROUNDED_TRACKS_CORNERS::Delete( ROUNDED_TRACKS_CORNER* aCorner, DLIST<TRACK>* aTrackListAt, PICKED_ITEMS_LIST* aUndoRedoList )
{
    if( aCorner && aTrackListAt )
    {
        if( aCorner->GetList() == aTrackListAt )
        {
            ITEM_PICKER picker( nullptr, UR_DELETED );

            TRACK* first_track = aCorner->GetTrackSeg();
            TRACK* second_track = aCorner->GetTrackSegSecond();

            //GAL View removing.
            if( m_EditFrame && m_EditFrame->IsGalCanvasActive() )
            {
               m_EditFrame->GetGalCanvas()->GetView()->Remove( aCorner );
            }

#ifdef NEWCONALGO
#ifndef MYCONALGO
            //New connectivity algo remove
            m_Board->GetConnectivity()->Remove( aCorner );
#endif
#endif

            aCorner->ResetVisibleEndpoints();
            aCorner->ReleaseTrackSegs();
            Update( first_track );
            Update( second_track );

            picker.SetItem( aCorner );
            aUndoRedoList->PushItem( picker );
            TracksDList_Remove( aTrackListAt, aCorner );

            m_Parent->Teardrops()->Update( first_track );
            m_Parent->Teardrops()->Update( second_track );

            if( m_EditFrame )
            {
                m_EditFrame->GetCanvas()->RefreshDrawingRect( aCorner->GetBoundingBox() );
                if( m_EditFrame->IsGalCanvasActive() )
                {
                    m_EditFrame->GetGalCanvas()->GetView()->Update( first_track, KIGFX::GEOMETRY );
                    m_EditFrame->GetGalCanvas()->GetView()->Update( second_track, KIGFX::GEOMETRY );
                    std::set<TRACK*> commit_container;
                    m_Parent->Teardrops()->CollectCommit( first_track, &commit_container, true );
                    m_Parent->Teardrops()->CollectCommit( second_track, &commit_container, true );
                    for( auto tear : commit_container )
                        if( tear )
                            m_EditFrame->GetGalCanvas()->GetView()->Update( tear, KIGFX::GEOMETRY );
                }
            }
        }
    }
}

TRACK* ROUNDED_TRACKS_CORNERS::FindSecondTrack( const TRACK* aTrackTo, wxPoint aPosition )
{
    if( aTrackTo )
    {
        Tracks_Container tracks;
        TracksConnected( aTrackTo, aPosition, tracks );
        if( tracks.size() == 1 )
        {
            TRACK* track_second = *tracks.begin();
            if( track_second->Type() == PCB_TRACE_T )
                return track_second;
        }
    }
    return nullptr;
}


ROUNDED_CORNER_TRACK* ROUNDED_TRACKS_CORNERS::Convert( TRACK* aTrack, PICKED_ITEMS_LIST* aUndoRedoList )
{
    ROUNDED_CORNER_TRACK* rounded_track = nullptr;
    if( aTrack && aTrack->Type() == PCB_TRACE_T )
    {
        ITEM_PICKER picker_new( nullptr, UR_NEW );
        ITEM_PICKER picker_deleted( nullptr, UR_DELETED );
        ROUNDED_CORNER_TRACK* created_track = nullptr;
        DLIST<TRACK>* tracks_list = static_cast<DLIST<TRACK>*>( aTrack->GetList() );
        if( tracks_list )
        {
            if( !dynamic_cast<ROUNDED_CORNER_TRACK*>( const_cast<TRACK*>( aTrack ) ) )
            {
                created_track = new ROUNDED_CORNER_TRACK( m_Board, const_cast<TRACK*>( aTrack ) );
                rounded_track = created_track;
            }
            else
                rounded_track = static_cast<ROUNDED_CORNER_TRACK*>( const_cast<TRACK*>( aTrack ) );

            if( created_track )
            {
                //If teardrops save them.
                m_Parent->Teardrops()->ToMemory( aTrack );
                m_Parent->Teardrops()->Remove( aTrack, aUndoRedoList, true );

                //Add created.
                picker_new.SetItem( created_track );
                aUndoRedoList->PushItem( picker_new );
#ifdef NEWCONALGO
#ifndef MYCONALGO
                m_Board->GetConnectivity()->Add( created_track );
#endif
#else
                m_Board->GetRatsnest()->Add( created_track );
#endif
                TracksDList_Insert( tracks_list, created_track, aTrack );

                //Remove old.
                picker_deleted.SetItem( aTrack );
                aUndoRedoList->PushItem( picker_deleted );
#ifdef NEWCONALGO
#ifndef MYCONALGO
                m_Board->GetConnectivity()->Remove( aTrack );
#endif
#else
                m_Board->GetRatsnest()->Remove( aTrack );
#endif
                TracksDList_Remove( tracks_list, aTrack );

                //If teardrops add them.
                m_Parent->Teardrops()->FromMemory( created_track, aUndoRedoList );
                m_Parent->Teardrops()->Update( created_track->GetNetCode(), created_track );

                //GAL remove and add
                if( m_EditFrame )
                {
                    if( m_EditFrame->IsGalCanvasActive() )
                    {
                        m_EditFrame->GetGalCanvas()->GetView()->Remove( aTrack );
                        m_EditFrame->GetGalCanvas()->GetView()->Add( created_track );
                    }
                }
            }
        }
    }
    return rounded_track;
}

ROUNDED_TRACKS_CORNER* ROUNDED_TRACKS_CORNERS::Add( TRACK* aTrackTo, const wxPoint aPosition, PICKED_ITEMS_LIST* aUndoRedoList )
{
    ROUNDED_TRACKS_CORNER* corner = nullptr;
    ITEM_PICKER picker_new( nullptr, UR_NEW );
    if( aTrackTo && IsOn() )
    {
        TRACK* track_second = FindSecondTrack( aTrackTo, aPosition );
        if( track_second )
        {
            track_second = Convert( track_second, aUndoRedoList );
            aTrackTo = Convert( aTrackTo, aUndoRedoList );

            corner = Create( aTrackTo, track_second, aPosition, false );

            if( corner )
            {
                DLIST<TRACK>* tracks_list = static_cast<DLIST<TRACK>*>( aTrackTo->GetList() );
                TracksDList_Insert( tracks_list, corner, corner->GetTrackSeg() );

                picker_new.SetItem( corner );
                aUndoRedoList->PushItem( picker_new );

                m_Parent->Teardrops()->Update( corner->GetTrackSeg() );
                m_Parent->Teardrops()->Update( corner->GetTrackSegSecond() );

                if( m_EditFrame )
                {
                    m_EditFrame->GetCanvas()->RefreshDrawingRect( corner->GetBoundingBox() );
                }
            }
        }
    }
    return corner;
}

ROUNDED_TRACKS_CORNER* ROUNDED_TRACKS_CORNERS::Add( TRACK* aTrackTo,
                                                const wxPoint aPosition,
                                                const unsigned int aLength,
                                                PICKED_ITEMS_LIST* aUndoRedoList
                                              )
{
    ROUNDED_TRACKS_CORNER* corner = nullptr;
    ROUNDED_TRACKS_CORNER::PARAMS current_params = GetParams();
    ROUNDED_TRACKS_CORNER::PARAMS params = current_params;
    params.length_set = aLength;
    params.length_ratio = 100;
    SetParams( params );
    corner = Add( aTrackTo, aPosition, aUndoRedoList );
    SetParams( current_params );
    RecreateMenu();
    return corner;
}

ROUNDED_TRACKS_CORNER* ROUNDED_TRACKS_CORNERS::Add( TRACK* aTrackTo, const wxPoint& aCurPosAt )
{
    ROUNDED_TRACKS_CORNER* corner = nullptr;
    if( aTrackTo && ( aTrackTo->Type() == PCB_TRACE_T ) && IsOn() )
    {
        PICKED_ITEMS_LIST undoredo_items;

        unsigned int min_dist = std::numeric_limits<unsigned int>::max();

        unsigned int dist_start = hypot( abs( aTrackTo->GetStart().y - aCurPosAt.y ),
                                        abs( aTrackTo->GetStart().x - aCurPosAt.x ) );
        if( dist_start < min_dist )
            min_dist = dist_start;

        unsigned int dist_end = hypot( abs( aTrackTo->GetEnd().y - aCurPosAt.y ),
                                      abs( aTrackTo->GetEnd().x - aCurPosAt.x ) );
        wxPoint pos;
        ( dist_start < dist_end )? pos = aTrackTo->GetStart() : pos = aTrackTo->GetEnd();

        corner = Add( aTrackTo, pos, &undoredo_items );
        if( undoredo_items.GetCount() )
            m_EditFrame->SaveCopyInUndoList( undoredo_items, UR_NEW );
    }
    return corner;
}

std::pair<ROUNDED_TRACKS_CORNER*, ROUNDED_TRACKS_CORNER*> ROUNDED_TRACKS_CORNERS::Add( TRACK* aTrackTo,
                                                                                 PICKED_ITEMS_LIST* aUndoRedoList
                                                                               )
{
    std::pair<ROUNDED_TRACKS_CORNER*, ROUNDED_TRACKS_CORNER*> ret_pair{nullptr, nullptr};
    if( aTrackTo && ( aTrackTo->Type() == PCB_TRACE_T ) && !aTrackTo->IsNull() && IsOn() )
    {
        ROUNDED_TRACKS_CORNER* corner = nullptr;
        for( int n = 0; n < 2; ++n )
        {
            wxPoint pos = aTrackTo->GetStart();
            if( n )
                pos = aTrackTo->GetEnd();

            corner = Add( aTrackTo, pos, aUndoRedoList );
            n? ret_pair.second = corner : ret_pair.first = corner;
        }
    }
    return ret_pair;
}

std::pair<ROUNDED_TRACKS_CORNER*, ROUNDED_TRACKS_CORNER*> ROUNDED_TRACKS_CORNERS::Add( TRACK* aTrackTo,
                                                                                 const unsigned int aLength,
                                                                                 PICKED_ITEMS_LIST* aUndoRedoList
                                                                               )
{
    std::pair<ROUNDED_TRACKS_CORNER*, ROUNDED_TRACKS_CORNER*> ret_pair{nullptr, nullptr};
    ROUNDED_TRACKS_CORNER::PARAMS current_params = GetParams();
    ROUNDED_TRACKS_CORNER::PARAMS params = current_params;
    params.length_set = aLength;
    params.length_ratio = 100;
    SetParams( params );
    ret_pair = Add( aTrackTo, aUndoRedoList );
    SetParams( current_params );
    RecreateMenu();
    return ret_pair;
}

std::pair<ROUNDED_TRACKS_CORNER*, ROUNDED_TRACKS_CORNER*> ROUNDED_TRACKS_CORNERS::Add( TRACK* aTrackTo )
{
    std::pair<ROUNDED_TRACKS_CORNER*, ROUNDED_TRACKS_CORNER*> ret_pair{nullptr, nullptr};
    PICKED_ITEMS_LIST null_undo_redo_list;
    ret_pair = Add( aTrackTo, &null_undo_redo_list );
    return ret_pair;
}

void ROUNDED_TRACKS_CORNERS::Add( std::set<TRACK*>* aTracksAt, PICKED_ITEMS_LIST* aUndoRedoList )
{
    if( IsOn() )
    {
        std::set<TRACK*> converted_tracks;
        for( auto track : *aTracksAt )
        {
            ROUNDED_CORNER_TRACK* rc_track = Convert( track, aUndoRedoList );
            converted_tracks.insert( rc_track );
        }

        for( auto track : converted_tracks )
            Add( track, aUndoRedoList );
    }
}

ROUNDED_TRACKS_CORNERS::NET_SCAN_NET_ADD::NET_SCAN_NET_ADD( const int aNet,
                                                          const ROUNDED_TRACKS_CORNERS* aParent,
                                                          PICKED_ITEMS_LIST* aUndoRedoList
                                                        ) :
    NET_SCAN_BASE( nullptr, aParent )
{
    m_picked_items = aUndoRedoList;
    DLIST<TRACK>* tracks_list = &m_Parent->GetBoard()->m_Track;
    m_scan_start_track = tracks_list->GetFirst()->GetStartNetCode( aNet );
}

bool ROUNDED_TRACKS_CORNERS::NET_SCAN_NET_ADD::ExecuteAt( TRACK* aTrack )
{
    if( aTrack->Type() == PCB_TRACE_T )
        dynamic_cast<ROUNDED_TRACKS_CORNERS*>( m_Parent )->Add( const_cast<TRACK*>( aTrack ), m_picked_items );
    return false;
}

void ROUNDED_TRACKS_CORNERS::Add( const int aNetCodeTo, PICKED_ITEMS_LIST* aUndoRedoList )
{
    if( IsOn() )
    {
        Convert( aNetCodeTo, aUndoRedoList );

        std::unique_ptr<NET_SCAN_NET_ADD> net_add( new NET_SCAN_NET_ADD( aNetCodeTo, this, aUndoRedoList ) );
        if( net_add )
            net_add->Execute();
    }
}

void ROUNDED_TRACKS_CORNERS::Add( const int aNetCodeTo )
{
    if( IsOn() )
    {
        PICKED_ITEMS_LIST undoredo_items;
        Add( aNetCodeTo, &undoredo_items );
        if( m_EditFrame && undoredo_items.GetCount() )
            m_EditFrame->SaveCopyInUndoList( undoredo_items, UR_NEW );
    }
}

ROUNDED_TRACKS_CORNERS::NET_SCAN_NET_CONVERT::NET_SCAN_NET_CONVERT( const int aNet,
                                                                  const ROUNDED_TRACKS_CORNERS* aParent,
                                                                  PICKED_ITEMS_LIST* aUndoRedoList
                                                                ) :
    NET_SCAN_NET_ADD( aNet, aParent, aUndoRedoList )
{
}

bool ROUNDED_TRACKS_CORNERS::NET_SCAN_NET_CONVERT::ExecuteAt( TRACK* aTrack )
{
    if( aTrack->Type() == PCB_TRACE_T )
    {
        TRACK* track = static_cast<TRACK*>( const_cast<TRACK*>( aTrack ) );
        track = static_cast<ROUNDED_TRACKS_CORNERS*>( m_Parent )->Convert( track, m_picked_items );
    }
    return false;
}

void ROUNDED_TRACKS_CORNERS::Convert( const int aNetCode, PICKED_ITEMS_LIST* aUndoRedoList )
{
    std::unique_ptr<NET_SCAN_NET_CONVERT> net_convert( new NET_SCAN_NET_CONVERT( aNetCode, this, aUndoRedoList ) );
    if( net_convert )
        net_convert->Execute();
}

void ROUNDED_TRACKS_CORNERS::Remove( const TRACK* aTrackFrom, const bool aUndo, const bool aLockedToo )
{
    if( aTrackFrom )
    {
        PICKED_ITEMS_LIST undoredo_items;
        Remove( aTrackFrom, &undoredo_items, aLockedToo );
        if( m_EditFrame && aUndo && undoredo_items.GetCount() )
            m_EditFrame->SaveCopyInUndoList( undoredo_items, UR_DELETED );
    }
}

void ROUNDED_TRACKS_CORNERS::Remove( const TRACK* aTrackFrom,
                                   PICKED_ITEMS_LIST* aUndoRedoList,
                                   const bool aLockedToo
                                 )
{
    if( aTrackFrom )
    {
        if( aTrackFrom->Type() == PCB_ROUNDEDTRACKSCORNER_T )
        {
            Remove( static_cast<ROUNDED_TRACKS_CORNER*>( const_cast<TRACK*>( aTrackFrom ) ),
                    aUndoRedoList,
                    !CAN_RECREATE,
                    aLockedToo );
        }
        else
        {
            if( aTrackFrom->Type() == PCB_TRACE_T &&
                dynamic_cast<ROUNDED_CORNER_TRACK*>( const_cast<TRACK*>( aTrackFrom ) ) )
            {
                TRACKNODEITEM* item = nullptr;
                for( unsigned int n = 0; n < 2; ++n )
                {
                    n? item = EndPosItem( const_cast<TRACK*>( aTrackFrom ) ) :
                       item = StartPosItem( const_cast<TRACK*>( aTrackFrom ) );

                    if( item && dynamic_cast<ROUNDED_TRACKS_CORNER*>( item ) )
                    {
                        ROUNDED_TRACKS_CORNER* corner = static_cast<ROUNDED_TRACKS_CORNER*>( item );
                        if( aLockedToo || ( !aLockedToo && !corner->IsLocked() ) )
                        {
                            if( ( corner->GetTrackSeg() == const_cast<TRACK*>( aTrackFrom ) ) ||
                               ( corner->GetTrackSegSecond() == const_cast<TRACK*>( aTrackFrom ) ) )
                            {
                                DLIST<TRACK>* tracks_list = static_cast<DLIST<TRACK>*>( aTrackFrom->GetList() );
                                if( tracks_list )
                                {
                                    Delete( corner, tracks_list, aUndoRedoList );
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void ROUNDED_TRACKS_CORNERS::Remove( ROUNDED_TRACKS_CORNER* aCorner,
                                   PICKED_ITEMS_LIST* aUndoRedoList,
                                   const bool aSaveRemoved,
                                   const bool aLockedToo
                                 )
{
    if( aCorner )
    {
        if( aLockedToo || ( !aLockedToo && !aCorner->IsLocked() ) )
        {
            DLIST<TRACK>* tracks_list = static_cast<DLIST<TRACK>*>( aCorner->GetTrackSeg()->GetList() );
            if( tracks_list )
            {
                if( aSaveRemoved )
                    m_recreate_list->insert( aCorner );

                Delete( aCorner, tracks_list, aUndoRedoList );
            }
        }
    }
}

void ROUNDED_TRACKS_CORNERS::Remove( const TRACK* aTrackFrom,
                                   BOARD_COMMIT& aCommit,
                                   const bool aLockedToo )
{
    if( aTrackFrom )
    {
        PICKED_ITEMS_LIST undoredo_items;
        Remove( aTrackFrom, &undoredo_items, aLockedToo );

        unsigned int num_removed_corners = undoredo_items.GetCount();
        if( num_removed_corners )
            for( unsigned int n = 0; n < num_removed_corners; ++n )
                aCommit.Removed( undoredo_items.GetPickedItem( n ) );
    }
}

void ROUNDED_TRACKS_CORNERS::Remove( const BOARD_ITEM* aItemFrom,
                                   PICKED_ITEMS_LIST* aUndoRedoList,
                                   const bool aLockedToo )
{
    if( aItemFrom->Type() == PCB_TRACE_T )
        Remove( static_cast<TRACK*>( const_cast<BOARD_ITEM*>( aItemFrom ) ),
                aUndoRedoList,
                aLockedToo );
}

ROUNDED_TRACKS_CORNERS::NET_SCAN_NET_REMOVE::NET_SCAN_NET_REMOVE( const int aNet,
                                                                const ROUNDED_TRACKS_CORNERS* aParent,
                                                                PICKED_ITEMS_LIST* aUndoRedoList,
                                                                RoundedTracksCorner_Container* aRecreateList,
                                                                const bool aLockedToo
                                                              ) :
    NET_SCAN_BASE( nullptr, aParent )
{
    m_picked_items = aUndoRedoList;
    m_recreate_list = aRecreateList;
    m_locked_too = aLockedToo;

    DLIST<TRACK>* tracks_list = &m_Parent->GetBoard()->m_Track;
    TRACK* first_track = tracks_list->GetFirst();
    if( first_track )
        m_scan_start_track = first_track->GetStartNetCode( aNet );
}

bool ROUNDED_TRACKS_CORNERS::NET_SCAN_NET_REMOVE::ExecuteAt( TRACK* aTrack )
{
    if( aTrack->Type() == PCB_ROUNDEDTRACKSCORNER_T )
    {
        if( m_locked_too || ( !m_locked_too && !aTrack->IsLocked() ) )
        {
            m_recreate_list->insert( static_cast<ROUNDED_TRACKS_CORNER*>( const_cast<TRACK*>( aTrack ) ) );
        }
    }
    return false;
}

//Save removed corners in list, recreating for.
void ROUNDED_TRACKS_CORNERS::Remove( const int aNetCodeFrom, PICKED_ITEMS_LIST* aUndoRedoList, const bool aLockedToo )
{
    DLIST<TRACK>* tracks_list = &m_Board->m_Track;
    m_recreate_list->clear();

    std::unique_ptr<NET_SCAN_NET_REMOVE> net_remove( new NET_SCAN_NET_REMOVE( aNetCodeFrom,
                                                                              this,
                                                                              aUndoRedoList,
                                                                              m_recreate_list,
                                                                              aLockedToo ) );
    if( net_remove )
        net_remove->Execute();

    for( ROUNDED_TRACKS_CORNER* corner : *m_recreate_list )
        if( corner )
            Delete( corner, tracks_list, aUndoRedoList );
}

void ROUNDED_TRACKS_CORNERS::Remove( const int aNetCodeFrom, const bool aUndo, const bool aLockedToo )
{
    PICKED_ITEMS_LIST undoredo_items;
    Remove( aNetCodeFrom, &undoredo_items, aLockedToo );
    if( m_EditFrame && aUndo && undoredo_items.GetCount() )
        m_EditFrame->SaveCopyInUndoList( undoredo_items, UR_DELETED );
}

void ROUNDED_TRACKS_CORNERS::Repopulate( const int aNetCodeTo, PICKED_ITEMS_LIST* aUndoRedoList )
{
    Remove( aNetCodeTo, aUndoRedoList, true );
    Recreate( aNetCodeTo, aUndoRedoList );
}

void ROUNDED_TRACKS_CORNERS::Change( const TRACK* aTrackFrom, const bool aUndo, const bool aLockedToo )
{
    if( aTrackFrom && aTrackFrom->Type() == PCB_ROUNDEDTRACKSCORNER_T )
    {
        PICKED_ITEMS_LIST undoredo_items;
        ROUNDED_TRACKS_CORNER* corner = static_cast<ROUNDED_TRACKS_CORNER*>( const_cast<TRACK*>( aTrackFrom ) );
        wxPoint pos = corner->GetEnd();
        TRACK* track = corner->GetTrackSeg();

        if( track )
        {
            Remove( corner, &undoredo_items, aLockedToo );
            Add( track, pos, &undoredo_items );
        }

        if( m_EditFrame && aUndo && undoredo_items.GetCount() )
            m_EditFrame->SaveCopyInUndoList( undoredo_items, UR_DELETED );
    }
}

ROUNDED_TRACKS_CORNERS::NET_SCAN_NET_RECREATE::NET_SCAN_NET_RECREATE( const int aNet,
                                                                    const ROUNDED_TRACKS_CORNERS* aParent,
                                                                    PICKED_ITEMS_LIST* aUndoRedoList,
                                                                    RoundedTracksCorner_Container* aRecreateList
                                                                  ) :
    NET_SCAN_NET_REMOVE( aNet, aParent, aUndoRedoList, aRecreateList, false )
{
    m_current_params = dynamic_cast<ROUNDED_TRACKS_CORNERS*>( m_Parent )->GetParams();
}

ROUNDED_TRACKS_CORNERS::NET_SCAN_NET_RECREATE::~NET_SCAN_NET_RECREATE()
{
    dynamic_cast<ROUNDED_TRACKS_CORNERS*>( m_Parent )->SetParams( m_current_params );
    dynamic_cast<ROUNDED_TRACKS_CORNERS*>( m_Parent )->RecreateMenu();
}

bool ROUNDED_TRACKS_CORNERS::NET_SCAN_NET_RECREATE::ExecuteAt( TRACK* aTrack )
{
    if( aTrack->Type() == PCB_TRACE_T )
    {
        for( ROUNDED_TRACKS_CORNER* corner : *m_recreate_list )
        {
            if( corner )
            {
                dynamic_cast<ROUNDED_TRACKS_CORNERS*>( m_Parent )->SetParams( corner->GetParams() );
                wxPoint pos = corner->GetEnd();
                if( ( aTrack->GetStart() == pos ) || ( aTrack->GetEnd() == pos ) )
                {
                    if( aTrack->IsOnLayer( corner->GetLayer() ) )
                    {
                        dynamic_cast<ROUNDED_TRACKS_CORNERS*>( m_Parent )->Add( const_cast<TRACK*>( aTrack ),
                                                                              pos,
                                                                              m_picked_items );
                    }
                }
            }
        }
    }
    return false;
}

void ROUNDED_TRACKS_CORNERS::Recreate( const int aNetCodeTo, PICKED_ITEMS_LIST* aUndoRedoList )
{
    std::unique_ptr<NET_SCAN_NET_RECREATE> net( new NET_SCAN_NET_RECREATE( aNetCodeTo,
                                                                           this,
                                                                           aUndoRedoList,
                                                                           m_recreate_list ) );
    if( net )
        net->Execute();
}

void ROUNDED_TRACKS_CORNERS::Update( const BOARD_ITEM* aItemAt )
{
    if( aItemAt )
    {
        if( dynamic_cast<TRACK*> ( const_cast<BOARD_ITEM*> ( aItemAt ) ) )
            Update( static_cast<TRACK*>( const_cast<BOARD_ITEM*>( aItemAt ) ) );
    }
}

void ROUNDED_TRACKS_CORNERS::SetParams( const ROUNDED_TRACKS_CORNER::PARAMS aParams )
{
    m_params = aParams;
    if( m_params.num_segments < ROUNDED_TRACKS_CORNER::SEGMENTS_MIN )
        m_params.num_segments = ROUNDED_TRACKS_CORNER::SEGMENTS_MIN;
    if( m_params.num_segments > ROUNDED_TRACKS_CORNER::SEGMENTS_MAX )
        m_params.num_segments = ROUNDED_TRACKS_CORNER::SEGMENTS_MAX;
}

void ROUNDED_TRACKS_CORNERS::LoadDefaultParams( void )
{
    m_params = GetDefaultParams();
    RecreateMenu();
}

ROUNDED_TRACKS_CORNER::PARAMS ROUNDED_TRACKS_CORNERS::GetDefaultParams( void ) const
{
    ROUNDED_TRACKS_CORNER::PARAMS params = { 0,
                                           ROUNDED_TRACKS_CORNER::DEFAULT_LENGTH_RATIO,
                                           ROUNDED_TRACKS_CORNER::DEFAULT_NUM_SEGMENTS };
    return params;
}

ROUNDED_TRACKS_CORNER::PARAMS ROUNDED_TRACKS_CORNERS::CopyCurrentParams( const TRACK* aTrackSegAt, const wxPoint& aCurPosAt )
{
    ROUNDED_TRACKS_CORNER::PARAMS corner_params = {0,0,10};
    wxPoint track_pos = TrackSegNearestEndpoint( aTrackSegAt, aCurPosAt );
    TRACKNODEITEM* item = Get( aTrackSegAt, track_pos );
    if( item && dynamic_cast<ROUNDED_TRACKS_CORNER*>( item ) )
    {
        corner_params = static_cast<ROUNDED_TRACKS_CORNER*>( item )->GetParams();
        SetParams( corner_params );
        RecreateMenu();
    }
    return corner_params;
}

TRACKNODEITEM* ROUNDED_TRACKS_CORNERS::EndPosItem( const TRACK* aTrackSegAt ) const
{
    ROUNDED_TRACKS_CORNER* result = nullptr;
    if( dynamic_cast<ROUNDED_CORNER_TRACK*>( const_cast<TRACK*>( aTrackSegAt ) ) )
        result = dynamic_cast<ROUNDED_CORNER_TRACK*>( const_cast<TRACK*>( aTrackSegAt ) )->GetEndPointCorner();
    return result;
}

TRACKNODEITEM* ROUNDED_TRACKS_CORNERS::StartPosItem( const TRACK* aTrackSegAt ) const
{
    ROUNDED_TRACKS_CORNER* result = nullptr;
    if( dynamic_cast<ROUNDED_CORNER_TRACK*>( const_cast<TRACK*>( aTrackSegAt ) ) )
        result = dynamic_cast<ROUNDED_CORNER_TRACK*>( const_cast<TRACK*>( aTrackSegAt ) )->GetStartPointCorner();
    return result;
}

// Update without draw.
ROUNDED_TRACKS_CORNERS::NET_SCAN_TRACK_UPDATE::NET_SCAN_TRACK_UPDATE( const TRACK* aStartTrack,
                                                                    const ROUNDED_TRACKS_CORNERS* aParent
                                                                  ) :
    NET_SCAN_BASE( aStartTrack, aParent )
{
}

bool ROUNDED_TRACKS_CORNERS::NET_SCAN_TRACK_UPDATE::ExecuteAt( TRACK* aTrack )
{
    if( dynamic_cast<ROUNDED_TRACKS_CORNER*>( const_cast<TRACK*>( aTrack ) ) )
    {
        if( dynamic_cast<ROUNDED_TRACKS_CORNER*>( const_cast<TRACK*>( aTrack ) )->GetTrackSeg() == m_scan_start_track )
            dynamic_cast<ROUNDED_TRACKS_CORNER*>( const_cast<TRACK*>( aTrack ) )->Update();

        if( dynamic_cast<ROUNDED_TRACKS_CORNER*>( const_cast<TRACK*>( aTrack ) )->GetTrackSegSecond() == m_scan_start_track )
            dynamic_cast<ROUNDED_TRACKS_CORNER*>( const_cast<TRACK*>( aTrack ) )->Update();
    }
    return false;
}

void ROUNDED_TRACKS_CORNERS::Update( const TRACK* aTrackSegAt )
{
    if( aTrackSegAt )
    {
        if( aTrackSegAt->Type() == PCB_ROUNDEDTRACKSCORNER_T )
            dynamic_cast<ROUNDED_TRACKS_CORNER*>( const_cast<TRACK*>( aTrackSegAt ) )->Update();

        if( aTrackSegAt->Type() == PCB_TRACE_T )
        {
            if( dynamic_cast<ROUNDED_CORNER_TRACK*>( const_cast<TRACK*>( aTrackSegAt ) ) )
                dynamic_cast<ROUNDED_CORNER_TRACK*>( const_cast<TRACK*>( aTrackSegAt ) )->ResetVisibleEndpoints();

            std::unique_ptr<NET_SCAN_TRACK_UPDATE> track( new NET_SCAN_TRACK_UPDATE( aTrackSegAt, this ) );
            if( track )
                track->Execute();
        }
    }
}

void ROUNDED_TRACKS_CORNERS::Update( TrackNodeItem::ROUNDED_TRACKS_CORNER* aCorner,
                                   EDA_DRAW_PANEL* aPanel,
                                   wxDC* aDC,
                                   GR_DRAWMODE aDrawMode,
                                   bool aErase
                                 )
{
    if( aCorner )
    {
        RoundedCornerTrack_Container r_tracks;
        r_tracks.insert( static_cast<ROUNDED_CORNER_TRACK*>( aCorner->GetTrackSeg() ) );
        r_tracks.insert( static_cast<ROUNDED_CORNER_TRACK*>( aCorner->GetTrackSegSecond() ) );
        if( aErase )
        {
            for( auto r_t: r_tracks )
                if( r_t )
                    r_t->Draw( aPanel, aDC, aDrawMode );
            aCorner->Draw( aPanel, aDC, aDrawMode );
        }
        aCorner->Update();
        aCorner->Draw( aPanel, aDC, aDrawMode );
        for( auto r_t: r_tracks )
            if( r_t )
                r_t->Draw( aPanel, aDC, aDrawMode );
    }
}

void ROUNDED_TRACKS_CORNERS::Update( TRACK* aTrackSegAt,
                                   EDA_DRAW_PANEL* aPanel,
                                   wxDC* aDC,
                                   GR_DRAWMODE aDrawMode,
                                   bool aErase
                                 )
{
    if( aTrackSegAt )
    {
        if( aTrackSegAt->Type() == PCB_ROUNDEDTRACKSCORNER_T )
            Update( dynamic_cast<ROUNDED_TRACKS_CORNER*> ( const_cast<TRACK*> ( aTrackSegAt ) ),
                    aPanel,
                    aDC,
                    aDrawMode,
                    aErase );

        if( aTrackSegAt->Type() == PCB_TRACE_T && dynamic_cast<ROUNDED_CORNER_TRACK*>( aTrackSegAt ) )
        {
            dynamic_cast<ROUNDED_CORNER_TRACK*>( const_cast<TRACK*>( aTrackSegAt ) )->ResetVisibleEndpoints();
            ROUNDED_TRACKS_CORNER* corner = dynamic_cast<ROUNDED_CORNER_TRACK*>( aTrackSegAt )->GetStartPointCorner();
            if( corner )
                Update( corner, aPanel, aDC, aDrawMode, aErase );

            corner = dynamic_cast<ROUNDED_CORNER_TRACK*>( aTrackSegAt )->GetEndPointCorner();
            if( corner )
                Update( corner, aPanel, aDC, aDrawMode, aErase );
        }
    }
}

void ROUNDED_TRACKS_CORNERS::Update( const int aNetCode, const TRACK* aStartTrack )
{
    DLIST<TRACK>* tracks_list = &m_Board->m_Track;
    TRACK* track = tracks_list->GetFirst()->GetStartNetCode( aNetCode );
    UpdateListClear();
    while( track )
    {
        TRACK* next_track = track->Next();

        if( dynamic_cast<ROUNDED_CORNER_TRACK*>( track ) )
            UpdateListAdd( track );

        if( next_track && next_track->GetNetCode() != aNetCode )
            track = nullptr;
        else
            track = next_track;
    }
    UpdateListDo();
}

void ROUNDED_TRACKS_CORNERS::ToMemory( const TRACK* aTrackSegFrom )
{
    m_next_corner_in_memory = nullptr;
    m_back_corner_in_memory = nullptr;

    TRACKNODEITEM* item = EndPosItem( aTrackSegFrom );
    if( item )
        m_next_corner_in_memory = dynamic_cast<ROUNDED_TRACKS_CORNER*>( item );

    item = StartPosItem( aTrackSegFrom );
    if( item )
        m_back_corner_in_memory = dynamic_cast<ROUNDED_TRACKS_CORNER*>( item );
}

void ROUNDED_TRACKS_CORNERS::FromMemory( const TRACK* aTrackTo, PICKED_ITEMS_LIST* aUndoRedoList )
{
    ROUNDED_TRACKS_CORNER::PARAMS corner_params = GetParams();
    if( m_next_corner_in_memory ) //Recreate next corner.
    {
        ROUNDED_TRACKS_CORNER::PARAMS params = m_next_corner_in_memory->GetParams();
        SetParams( params );
        ROUNDED_TRACKS_CORNER* added_corner = Add( const_cast<TRACK*>( aTrackTo ),
                                                 m_next_corner_in_memory->GetEnd(),
                                                 aUndoRedoList );
        if( added_corner )
            added_corner->SetLocked( m_next_corner_in_memory->IsLocked() );
    }
    if( m_back_corner_in_memory ) //Recreate back corner.
    {
        ROUNDED_TRACKS_CORNER::PARAMS params = m_back_corner_in_memory->GetParams();
        SetParams( params );
        ROUNDED_TRACKS_CORNER* added_corner = Add( const_cast<TRACK*>( aTrackTo ),
                                                 m_back_corner_in_memory->GetEnd(),
                                                 aUndoRedoList );
        if( added_corner )
            added_corner->SetLocked( m_back_corner_in_memory->IsLocked() );
    }
    SetParams( corner_params );
    RecreateMenu();
}

void ROUNDED_TRACKS_CORNERS::FromMemory( const TRACK* aTrackTo )
{
    PICKED_ITEMS_LIST undoredo_list;
    FromMemory( aTrackTo, &undoredo_list );
}

void ROUNDED_TRACKS_CORNERS::FromMemory( const TRACK* aTrackTo, BOARD_COMMIT& aCommit )
{
    PICKED_ITEMS_LIST undoredo_items;
    FromMemory( aTrackTo, &undoredo_items );

    unsigned int num_added_corners = undoredo_items.GetCount();
    if( num_added_corners )
        for( unsigned int n = 0; n < num_added_corners; ++n )
            aCommit.Added( undoredo_items.GetPickedItem( n ) );
}
