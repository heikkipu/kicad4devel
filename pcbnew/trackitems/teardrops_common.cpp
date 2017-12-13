/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2012- Heikki Pulkkinen.
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

#include <view/view.h> //Gal canvas
#include "teardrops.h"
#ifdef NEWCONALGO
#include <connectivity_data.h>
#endif

using namespace TrackNodeItem;
using namespace TrackNodeItems;


TEARDROPS::TEARDROPS( const TRACKITEMS* aParent, const BOARD* aBoard ) : TRACKNODEITEMS( aParent, aBoard )
{
    m_to_edit = EDIT_NULL_T;
    m_can_edit = false;
    m_track_edit_tear = nullptr;
    m_menu = nullptr;

    m_update_list = new Teardrop_Container;
    m_update_list->clear();

    m_recreate_list = new Teardrop_Container;
    m_recreate_list->clear();

    LoadDefaultParams( TEARDROP::TEARDROP_T );
    LoadDefaultParams( TEARDROP::FILLET_T );
    LoadDefaultParams( TEARDROP::SUBLAND_T );
    LoadDefaultParams( TEARDROP::ZERO_T );
    m_current_shape = TEARDROP::NULL_T; //Put teardrops to off mode in startup.

}

TEARDROPS::~TEARDROPS()
{
    delete m_update_list;
    m_update_list = nullptr;

    delete m_recreate_list;
    m_recreate_list = nullptr;

#if 0
    delete m_gal_removed_list;
    m_gal_removed_list = nullptr;
#endif //0

    if( m_track_edit_tear )
    {
        delete m_track_edit_tear;
        m_track_edit_tear = nullptr;
    }
}

//All teardrops are created here.
//Create JUNCTIONS & T-JUNCTIONS when !aViaOrPadTo && aPosition != 0,0
TEARDROP* TEARDROPS::Create( const TRACK* aTrackSegTo,
                             const BOARD_CONNECTED_ITEM* aViaOrPadTo,
                             const bool aNullTrackCheck,
                             const wxPoint aPosition
                           )
{
    TEARDROP* tear = nullptr;
    if( IsOn() )
    {
        if( !m_Parent->RoundedTracksCorners()->Get( aTrackSegTo, aPosition ) )
        {
            TEARDROP::PARAMS params = GetShapeParams( GetCurrentShape() );
            if( aViaOrPadTo )
            {
                if( !GetTeardrop( aTrackSegTo, aViaOrPadTo ) )
                {
                    if( aViaOrPadTo->Type() == PCB_VIA_T )
                        tear = new TEARDROP_VIA( m_Board,
                                                 static_cast<VIA*>( const_cast<BOARD_CONNECTED_ITEM*>( aViaOrPadTo ) ),
                                                 aTrackSegTo,
                                                 params, aNullTrackCheck );

                    if( aViaOrPadTo->Type() == PCB_PAD_T )
                        tear = new TEARDROP_PAD( m_Board,
                                                 static_cast<D_PAD*>( const_cast<BOARD_CONNECTED_ITEM*>( aViaOrPadTo ) ),
                                                 aTrackSegTo,
                                                 params,
                                                 aNullTrackCheck );
                }
            }
            else //TJUNCTIONS, JUNCTIONS
                if( ( aPosition != wxPoint( 0,0 ) ) &&
                    !Get( aTrackSegTo, aPosition ) )
                {
                    tear = new TEARDROP_JUNCTIONS( m_Board,
                                                   aTrackSegTo,
                                                   aPosition,
                                                   params,
                                                   aNullTrackCheck );
                }
            if( tear )
            {
                if( !tear->IsCreatedOK() )
                {
                    delete tear;
                    tear = nullptr;
                }
                else
                {
                    tear->Update();

                    //GAL View addition.
                    if( m_EditFrame )
                        if( m_EditFrame->IsGalCanvasActive() )
                            m_EditFrame->GetGalCanvas()->GetView()->Add( tear );

#ifdef NEWCONALGO
                    //New connectivity algo add
                    m_Board->GetConnectivity()->Add( tear );
#endif
                }
            }
        }
    }
    return tear;
}

//All teardrops are removed here.
void TEARDROPS::Delete( TEARDROP* aTeardrop,
                        DLIST<TRACK>* aTrackListAt,
                        PICKED_ITEMS_LIST* aUndoRedoList
                      )
{
    if( aTeardrop && aTrackListAt )
    {
        if( aTeardrop->GetList() == aTrackListAt )
        {
            ITEM_PICKER picker( nullptr, UR_DELETED );

            //GAL View removing.
            if( m_EditFrame && m_EditFrame->IsGalCanvasActive() )
            {
                m_EditFrame->GetGalCanvas()->GetView()->Remove( aTeardrop );
            }

#ifdef NEWCONALGO
            //New connectivity algo remove
            m_Board->GetConnectivity()->Remove( aTeardrop );
#endif

            picker.SetItem( aTeardrop );
            aUndoRedoList->PushItem( picker );
            TracksDList_Remove( aTrackListAt, aTeardrop );

            if( m_EditFrame )
                m_EditFrame->GetCanvas()->RefreshDrawingRect( aTeardrop->GetBoundingBox() );
        }
    }
}

TEARDROP* TEARDROPS::Add( const TRACK* aTrackSegTo,
                          const BOARD_CONNECTED_ITEM* aViaOrPadTo,
                          DLIST<TRACK>*aTrackListAt
                        )
{
    TEARDROP* tear = nullptr;
    if( m_current_shape )
    {
        TRACK*  last_non_teardrop = const_cast<TRACK*>( aTrackSegTo );

        tear = Create( aTrackSegTo, aViaOrPadTo, true );
        if( tear )
            aTrackListAt->PushBack( tear );

        aTrackSegTo = last_non_teardrop;

    }
    return tear;
}

TEARDROP* TEARDROPS::Add( const TRACK* aTrackSegTo,
                          const BOARD_CONNECTED_ITEM* aViaOrPadTo,
                          DLIST<TRACK>*aTrackListAt,
                          const bool aNullTrackCheck
                        )
{
    TEARDROP* tear = nullptr;
    if( m_current_shape )
    {
        TRACK*  last_non_teardrop = const_cast<TRACK*>( aTrackSegTo );

        tear = Create( aTrackSegTo, aViaOrPadTo, aNullTrackCheck );
        if( tear )
            TracksDList_Insert( aTrackListAt, tear, tear->GetTrackSeg() );

        aTrackSegTo = last_non_teardrop;
    }
    return tear;
}

//Add T-Junctions too.
TEARDROP* TEARDROPS::Add( const TRACK* aTrackSegTo, const wxPoint& aCurPosAt )
{
    if( ( aTrackSegTo->Type() == PCB_TRACE_T ) )
    {
        VIA* item_via = nullptr;
        TEARDROP* tear = nullptr;
        D_PAD* item_pad = nullptr;
        unsigned int min_dist = std::numeric_limits<unsigned int>::max();

#ifdef NEWCONALGO
        auto connity = m_Board->GetConnectivity();
        auto pads = connity->GetConnectedPads( const_cast<TRACK*>( aTrackSegTo ) );
        for( auto pad : pads )
        {
            wxPoint pad_pos = pad->GetPosition();
            unsigned int dist = hypot( abs( pad_pos.y - aCurPosAt.y ),
                                       abs( pad_pos.x - aCurPosAt.x ) );
            if( dist < min_dist )
            {
                min_dist = dist;
                item_pad = pad;
            }
        }
#else
        for( unsigned int n = 0; n<aTrackSegTo->m_PadsConnected.size(); ++n )
        {
            wxPoint pad_pos = aTrackSegTo->m_PadsConnected.at( n )->GetPosition();
            unsigned int dist = hypot( abs( pad_pos.y - aCurPosAt.y ),
                                       abs( pad_pos.x - aCurPosAt.x ) );
            if( dist < min_dist )
            {
                min_dist = dist;
                item_pad = aTrackSegTo->m_PadsConnected.at( n );
            }
        }
#endif

        for( unsigned int n = 0; n < 2; ++n )
        {
            VIA* via;
            ( n )? via = m_Parent->BackVia( aTrackSegTo ) : via = m_Parent->NextVia( aTrackSegTo );
            if( via )
            {
                wxPoint via_pos = via->GetPosition();
                unsigned int dist = hypot( abs( via_pos.y - aCurPosAt.y ),
                                           abs( via_pos.x - aCurPosAt.x ) );
                if( dist < min_dist )
                {
                    item_via = via;
                    min_dist = dist;
                }
            }
        }

        unsigned int dist_start = hypot( abs( aTrackSegTo->GetStart().y - aCurPosAt.y ),
                                         abs( aTrackSegTo->GetStart().x - aCurPosAt.x ) );
        if( dist_start < min_dist )
        {
            item_via = nullptr;
            item_pad = nullptr;
            min_dist = dist_start;
        }
        unsigned int dist_end = hypot( abs( aTrackSegTo->GetEnd().y - aCurPosAt.y ),
                                       abs( aTrackSegTo->GetEnd().x - aCurPosAt.x ) );
        if( dist_end < min_dist )
        {
            item_via = nullptr;
            item_pad = nullptr;
        }
        if( item_via )
            tear = Create( aTrackSegTo, item_via, false );
        else
            if( item_pad )
                tear = Create( aTrackSegTo, item_pad, false );
            else //TJUNCTIONS
            {
                wxPoint tjunctions_pos;
                ( dist_start < dist_end )? tjunctions_pos = aTrackSegTo->GetStart() :
                                           tjunctions_pos = aTrackSegTo->GetEnd();

                tear = Create( aTrackSegTo, nullptr, false, tjunctions_pos );
            }
        if( tear )
        {
            DLIST<TRACK>* tracks_list = static_cast<DLIST<TRACK>*>( aTrackSegTo->GetList() );
            if( tracks_list )
                TracksDList_Insert( tracks_list, tear, tear->GetTrackSeg() );

            if( m_EditFrame )
            {
                m_EditFrame->SaveCopyInUndoList( tear, UR_NEW );
                m_EditFrame->GetCanvas()->RefreshDrawingRect( tear->GetBoundingBox() );
            }
            return tear;
        }
    }
    return nullptr;
}

void TEARDROPS::Add( const BOARD_CONNECTED_ITEM* aViaOrPadTo )
{
    PICKED_ITEMS_LIST undoredo_items;
    if( aViaOrPadTo->Type() == PCB_VIA_T )
    {
        Add( static_cast<VIA*>( const_cast<BOARD_CONNECTED_ITEM*>( aViaOrPadTo ) ),
             &undoredo_items );
    }

    if( aViaOrPadTo->Type() == PCB_PAD_T )
    {
        Add( static_cast<D_PAD*>( const_cast<BOARD_CONNECTED_ITEM*>( aViaOrPadTo ) ),
             &undoredo_items );
    }

    if( m_EditFrame && undoredo_items.GetCount() )
        m_EditFrame->SaveCopyInUndoList( undoredo_items, UR_NEW );
}

TEARDROPS::NET_SCAN_PAD_ADD::NET_SCAN_PAD_ADD( const D_PAD* aPad,
                                               const TEARDROPS* aParent,
                                               PICKED_ITEMS_LIST* aUndoRedoList
                                             ) :
    NET_SCAN_PAD_BASE( aPad, aParent )
{
    m_picked_items = aUndoRedoList;
}

bool TEARDROPS::NET_SCAN_PAD_ADD::ExecuteAt( TRACK* aTrack )
{
    ITEM_PICKER picker( nullptr, UR_NEW );
    TEARDROP* tear = m_Parent->Create( aTrack, m_pad, false );
    if( tear )
    {
        DLIST<TRACK>* tracks_list = static_cast<DLIST<TRACK>*>( aTrack->GetList() );
        if( tracks_list )
        {
            m_Parent->TracksDList_Insert( tracks_list, tear, tear->GetTrackSeg() );
            picker.SetItem( tear );
            m_picked_items->PushItem( picker );
        }
        if( m_Parent->GetEditFrame() )
            m_Parent->GetEditFrame()->GetCanvas()->RefreshDrawingRect( tear->GetBoundingBox() );
    }
    return false;
}

void TEARDROPS::Add( const D_PAD* aPadTo, PICKED_ITEMS_LIST* aUndoRedoList )
{
    if( aPadTo )
    {
        std::unique_ptr<NET_SCAN_PAD_ADD> pad( new NET_SCAN_PAD_ADD( aPadTo, this, aUndoRedoList ) );
        if( pad )
            pad->Execute();
    }
}

TEARDROPS::NET_SCAN_VIA_ADD::NET_SCAN_VIA_ADD( const VIA* aVia,
                                               const TEARDROPS* aParent,
                                               PICKED_ITEMS_LIST* aUndoRedoList
                                             ) :
    NET_SCAN_VIA_UPDATE( aVia, aParent )
{
    m_picked_items = const_cast<PICKED_ITEMS_LIST*>( aUndoRedoList );
}

bool TEARDROPS::NET_SCAN_VIA_ADD::ExecuteAt( TRACK* aTrack )
{
    if( aTrack->Type() == PCB_TRACE_T )
    {
        ITEM_PICKER picker( nullptr, UR_NEW );
        if( ( aTrack->GetEnd() == m_via->GetPosition() ) ||
            ( aTrack->GetStart() == m_via->GetPosition() ) )
        {
            TEARDROP* tear = dynamic_cast<TEARDROPS*>( m_Parent )->Create( aTrack, m_via, false );
            if( tear )
            {
                DLIST<TRACK>* tracks_list = static_cast<DLIST<TRACK>*>( aTrack->GetList() );
                if( tracks_list )
                {
                    m_Parent->GetBoard()->TrackItems()->Teardrops()->TracksDList_Insert( tracks_list,
                                                                                         tear,
                                                                                         tear->GetTrackSeg()
                                                                                       );
                    picker.SetItem( tear );
                    m_picked_items->PushItem( picker );
                }
                if( m_Parent->GetEditFrame() )
                    m_Parent->GetEditFrame()->GetCanvas()->RefreshDrawingRect( tear->GetBoundingBox() );
            }
        }
    }
    return false;
}

void TEARDROPS::Add( const VIA* aViaTo, PICKED_ITEMS_LIST* aUndoRedoList )
{
    if( aViaTo )
    {
        std::unique_ptr<NET_SCAN_VIA_ADD> via( new NET_SCAN_VIA_ADD( aViaTo, this, aUndoRedoList ) );
        if( via )
            via->Execute();
    }
}

void TEARDROPS::Add( const TRACK* aTrackSegTo )
{
    if( aTrackSegTo )
    {
        PICKED_ITEMS_LIST undoredo_items;
        Add( aTrackSegTo, &undoredo_items );
        if( m_EditFrame && undoredo_items.GetCount() )
            m_EditFrame->SaveCopyInUndoList( undoredo_items, UR_NEW );
    }
}

void TEARDROPS::Add( const TRACK* aTrackSegTo, PICKED_ITEMS_LIST* aUndoRedoList )
{
    if( aTrackSegTo && aUndoRedoList )
    {
        Add( aTrackSegTo, m_Parent->NextVia( aTrackSegTo ), aUndoRedoList );
        Add( aTrackSegTo, m_Parent->BackVia( aTrackSegTo ), aUndoRedoList );

        Add( aTrackSegTo,
             m_Board->GetPadFast( aTrackSegTo->GetStart(), LSET( aTrackSegTo->GetLayer() ) ),
             aUndoRedoList );

        Add( aTrackSegTo,
             m_Board->GetPadFast( aTrackSegTo->GetEnd(), LSET( aTrackSegTo->GetLayer() ) ),
             aUndoRedoList );
    }
}

//Add TJUNCTIONS when !aViaOrPadTo && aPosition != 0,0
TEARDROP* TEARDROPS::Add( const TRACK* aTrackSegTo,
                          const BOARD_CONNECTED_ITEM* aViaOrPadTo,
                          PICKED_ITEMS_LIST* aUndoRedoList,
                          const wxPoint aPosition
                        )
{
    TEARDROP* tear = nullptr;
    if( aTrackSegTo )
    {
        if( aTrackSegTo->Type() == PCB_TRACE_T )
        {
            ITEM_PICKER picker( nullptr, UR_NEW );
            tear = Create( aTrackSegTo, aViaOrPadTo, false, aPosition );
            if( tear )
            {
                DLIST<TRACK>* tracks_list = static_cast<DLIST<TRACK>*>( aTrackSegTo->GetList() );
                if( tracks_list )
                {
                    TracksDList_Insert( tracks_list, tear, tear->GetTrackSeg() );
                    picker.SetItem( tear );
                    aUndoRedoList->PushItem( picker );
                }
                if( m_EditFrame )
                    m_EditFrame->GetCanvas()->RefreshDrawingRect( tear->GetBoundingBox() );
            }
        }
    }
    return tear;
}

//Add all modules pads.
void TEARDROPS::Add( const MODULE* aModuleTo )
{
    if( aModuleTo )
    {
        D_PAD* pad = aModuleTo->PadsList();
        if( pad )
        {
            PICKED_ITEMS_LIST undoredo_items;
            while( pad )
            {
                Add( pad, &undoredo_items );
                pad = pad->Next();
            }
            if( m_EditFrame && undoredo_items.GetCount() )
                m_EditFrame->SaveCopyInUndoList( undoredo_items, UR_NEW );
        }
    }
}

TEARDROPS::NET_SCAN_NET_ADD::NET_SCAN_NET_ADD( const int aNet,
                                               const TEARDROPS* aParent,
                                               PICKED_ITEMS_LIST* aUndoRedoList,
                                               const TEARDROPS::TEARDROPS_TYPE_TODO aTypeToDo
                                             ) :
    NET_SCAN_NET_UPDATE( aNet, nullptr, aParent )
{
    m_picked_items = aUndoRedoList;
    m_type_todo = aTypeToDo;
}

bool TEARDROPS::NET_SCAN_NET_ADD::ExecuteAt( TRACK* aTrack )
{
    if( ( m_type_todo == ALL_TYPES_T ) ||
        ( m_type_todo == ONLY_TJUNCTIONS_T ) ||
        ( m_type_todo == JUNCTIONS_AND_TJUNCTIONS_T ) )
    {
        dynamic_cast<TEARDROPS*>( m_Parent )->Add( aTrack, TJUNCTION_TO_ADD, m_picked_items );
    }

    if( ( m_type_todo == ALL_TYPES_T ) ||
        ( m_type_todo == ONLY_JUNCTIONS_T ) ||
        ( m_type_todo == JUNCTIONS_AND_TJUNCTIONS_T ) )
    {
        dynamic_cast<TEARDROPS*>( m_Parent )->Add( aTrack, JUNCTION_TO_ADD, m_picked_items );
    }

    if( ( m_type_todo == ALL_TYPES_T ) || ( m_type_todo == ONLY_TEARDROPS_T ) )
    {
        dynamic_cast<TEARDROPS*>( m_Parent )->Add( m_Parent->GetParent()->NextVia( aTrack ), m_picked_items );
        dynamic_cast<TEARDROPS*>( m_Parent )->Add( m_Parent->GetParent()->BackVia( aTrack ), m_picked_items );
        dynamic_cast<TEARDROPS*>( m_Parent )->Add( m_Parent->GetParent()->NextPad( aTrack ), m_picked_items );
        dynamic_cast<TEARDROPS*>( m_Parent )->Add( m_Parent->GetParent()->BackPad( aTrack ), m_picked_items );
    }

    return false;
}

void TEARDROPS::Add( const int aNetCodeTo, const TEARDROPS::TEARDROPS_TYPE_TODO aTypeToDo )
{
    PICKED_ITEMS_LIST undoredo_items;
    std::unique_ptr<NET_SCAN_NET_ADD> net( new NET_SCAN_NET_ADD( aNetCodeTo, this, &undoredo_items, aTypeToDo ) );
    if( net )
    {
        net->Execute();

        if( m_EditFrame && undoredo_items.GetCount() )
            m_EditFrame->SaveCopyInUndoList( undoredo_items, UR_NEW );
    }
}

//Junction & T-Junction.
void TEARDROPS::Add( const TRACK* aTrackSegTo, const bool aTJunction, PICKED_ITEMS_LIST* aUndoRedoList )
{
    for( int n = 0; n < 2; ++n )
    {
        VIA* via = nullptr;
        D_PAD* pad = nullptr;
        ( n )? via = m_Parent->BackVia( aTrackSegTo ) : via = m_Parent->NextVia( aTrackSegTo );
        ( n )? pad = m_Parent->BackPad( aTrackSegTo ) : pad = m_Parent->NextPad( aTrackSegTo );
        if( !via && !pad )
        {
            TRACK* track_next = nullptr;
            TRACK* track_back = nullptr;
            wxPoint track_pos;
            ( n )? track_pos = aTrackSegTo->GetStart() : track_pos = aTrackSegTo->GetEnd();
            bool t_tracks = Find_T_Tracks( aTrackSegTo, track_pos, track_next, track_back );
            if( aTJunction )
            {
                if( t_tracks )
                    Add( aTrackSegTo, nullptr, aUndoRedoList, track_pos );
            }
            else
            {
                Tracks_Container tracks_list;
                Collect( aTrackSegTo, track_pos, tracks_list );
                if( tracks_list.size() )
                    if( ( GetMaxWidth( tracks_list ) > aTrackSegTo->GetWidth() ) && !t_tracks )
                        Add( aTrackSegTo, nullptr, aUndoRedoList, track_pos );
            }
        }
    }
}

void TEARDROPS::Remove( const BOARD_CONNECTED_ITEM* aItemFrom,
                        const bool aUndo,
                        const bool aLockedToo
                      )
{
    if( aItemFrom )
    {
        PICKED_ITEMS_LIST undoredo_items;
        Remove( aItemFrom, &undoredo_items, aLockedToo );
        if( m_EditFrame && aUndo && undoredo_items.GetCount() )
            m_EditFrame->SaveCopyInUndoList( undoredo_items, UR_DELETED );
    }
}

void TEARDROPS::Remove( const BOARD_CONNECTED_ITEM* aItemFrom,
                        BOARD_COMMIT& aCommit,
                        const bool aLockedToo
                      )
{
    if( aItemFrom )
    {
        PICKED_ITEMS_LIST undoredo_items;
        Remove( aItemFrom, &undoredo_items, aLockedToo );

        unsigned int num_removed_tears = undoredo_items.GetCount();
        if( num_removed_tears )
            for( unsigned int n = 0; n < num_removed_tears; ++n )
                aCommit.Removed( undoredo_items.GetPickedItem( n ) );
    }
}

void TEARDROPS::Remove( const BOARD_CONNECTED_ITEM* aItemFrom,
                        PICKED_ITEMS_LIST* aUndoRedoList,
                        const bool aLockedToo
                      )
{
    if( aItemFrom )
    {
        if( aItemFrom->Type() == PCB_TEARDROP_T )
        {
            Remove( static_cast<TEARDROP*>( const_cast<BOARD_CONNECTED_ITEM*>( aItemFrom ) ),
                    aUndoRedoList, !CAN_RECREATE,
                    aLockedToo );
        }
        else
        {
            if( aItemFrom->Type() == PCB_VIA_T )
            {
                Remove( static_cast<VIA*>( const_cast<BOARD_CONNECTED_ITEM*>( aItemFrom ) ),
                        aUndoRedoList,
                        !CAN_RECREATE,
                        aLockedToo );
            }
            else
            {
                if( aItemFrom->Type() == PCB_PAD_T )
                {
                    m_recreate_list->clear();
                    Remove( static_cast<D_PAD*>( const_cast<BOARD_CONNECTED_ITEM*>( aItemFrom ) ),
                            aUndoRedoList,
                            CAN_RECREATE,
                            aLockedToo ); //Can be recreated.
                }
                else
                {
                    for( unsigned int n = 0; n < 2; ++n )
                    {
                        TRACKNODEITEM* item = nullptr;
                        n? item = Next( static_cast<TRACK*>( const_cast<BOARD_CONNECTED_ITEM*>( aItemFrom ) ) ) :
                           item = Back( static_cast<TRACK*>( const_cast<BOARD_CONNECTED_ITEM*>( aItemFrom ) ) );

                        if( item && dynamic_cast<TEARDROP*>( item ) )
                        {
                            TEARDROP* tear = static_cast<TEARDROP*>( item );
                            if( aLockedToo || ( !aLockedToo && !tear->IsLocked() ) )
                            {
                                if( tear->GetTrackSeg() == static_cast<TRACK*>( const_cast<BOARD_CONNECTED_ITEM*>( aItemFrom ) ) )
                                {
                                    DLIST<TRACK>* tracks_list = static_cast<DLIST<TRACK>*>( aItemFrom->GetList() );
                                    if( tracks_list )
                                    {
                                        Delete( tear, tracks_list, aUndoRedoList );
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void TEARDROPS::Remove( const BOARD_ITEM* aItemFrom,
                        PICKED_ITEMS_LIST* aUndoRedoList,
                        const bool aLockedToo )
{
    if( dynamic_cast<BOARD_CONNECTED_ITEM*>( const_cast<BOARD_ITEM*>( aItemFrom ) ) )
        Remove( static_cast<BOARD_CONNECTED_ITEM*>( const_cast<BOARD_ITEM*>( aItemFrom ) ),
                aUndoRedoList,
                aLockedToo );
}

void TEARDROPS::Remove( TEARDROP* aTeardrop,
                        PICKED_ITEMS_LIST* aUndoRedoList,
                        const bool aSaveRemoved,
                        const bool aLockedToo
                      )
{
    if( aTeardrop )
    {
        if( aLockedToo || ( !aLockedToo && !aTeardrop->IsLocked() ) )
        {
            DLIST<TRACK>* tracks_list = static_cast<DLIST<TRACK>*>( aTeardrop->GetTrackSeg()->GetList() );
            if( tracks_list )
            {
                if( aSaveRemoved )
                    m_recreate_list->insert( aTeardrop );

                Delete( aTeardrop, tracks_list, aUndoRedoList );
            }
        }
    }
}

TEARDROPS::NET_SCAN_VIA_REMOVE::NET_SCAN_VIA_REMOVE( const VIA* aVia,
                                                     const TEARDROPS* aParent,
                                                     PICKED_ITEMS_LIST* aUndoRedoList,
                                                     Teardrop_Container* aRecreateList,
                                                     const bool aLockedToo
                                                   ) :
    NET_SCAN_VIA_ADD( aVia, aParent, aUndoRedoList )
{
    m_recreate_list = const_cast<Teardrop_Container*>( aRecreateList );
    m_locked_too = aLockedToo;
}

bool TEARDROPS::NET_SCAN_VIA_REMOVE::ExecuteAt( TRACK* aTrack )
{
    if( aTrack->Type() == PCB_TRACE_T )
    {
        TEARDROP* tear = dynamic_cast<TEARDROPS*>( m_Parent )->GetTeardrop( aTrack, m_via );
        if( tear )
        {
            if( m_locked_too || ( !m_locked_too && !tear->IsLocked() ) )
            {
                DLIST<TRACK>* tracks_list = static_cast<DLIST<TRACK>*>( aTrack->GetList() );
                if( tracks_list )
                {
                    if( m_recreate_list )
                        m_recreate_list->insert( tear ); //Save at m_recreate_list if it must recreate.

                    dynamic_cast<TEARDROPS*>( m_Parent )->Delete( tear, tracks_list, m_picked_items );
                }
            }
        }
    }
    return false;
}

void TEARDROPS::Remove( const VIA* aViaFrom,
                        PICKED_ITEMS_LIST* aUndoRedoList,
                        const bool aSaveRemoved,
                        const bool aLockedToo
                      )
{
    if( aViaFrom )
    {
        Teardrop_Container* recreate_list = m_recreate_list;
        if( !aSaveRemoved )
            recreate_list = nullptr;

        std::unique_ptr<NET_SCAN_VIA_REMOVE> via( new NET_SCAN_VIA_REMOVE( aViaFrom,
                                                                           this,
                                                                           aUndoRedoList,
                                                                           recreate_list,
                                                                           aLockedToo ) );
        if( via )
            via->Execute();
    }
}

TEARDROPS::NET_SCAN_PAD_REMOVE::NET_SCAN_PAD_REMOVE( const D_PAD* aPad,
                                                     const TEARDROPS* aParent,
                                                     PICKED_ITEMS_LIST* aUndoRedoList,
                                                     Teardrop_Container* aRecreateList,
                                                     const bool aLockedToo
                                                   ) :
    NET_SCAN_PAD_ADD( aPad, aParent, aUndoRedoList )
{
    m_recreate_list = aRecreateList;
    m_locked_too = aLockedToo;
}

bool TEARDROPS::NET_SCAN_PAD_REMOVE::ExecuteAt( TRACK* aTrack )
{
    TEARDROP* tear = dynamic_cast<TEARDROPS*>( m_Parent )->GetTeardrop( aTrack, m_pad );
    if( tear )
    {
        if( m_locked_too || ( !m_locked_too && !tear->IsLocked() ) )
        {
            DLIST<TRACK>* tracks_list = static_cast<DLIST<TRACK>*>( aTrack->GetList() );
            if( tracks_list )
            {
                if( m_recreate_list )
                    m_recreate_list->insert( tear ); //Save to m_recreate_list, so it can be  recreated.

                dynamic_cast<TEARDROPS*>( m_Parent )->Delete( tear, tracks_list, m_picked_items );
            }
        }
    }
    return false;
}

void TEARDROPS::Remove( const D_PAD* aPadFrom,
                        PICKED_ITEMS_LIST* aUndoRedoList,
                        const bool aSaveRemoved,
                        const bool aLockedToo
                      )
{
    Teardrop_Container* recreate_list = m_recreate_list;
    if( !aSaveRemoved )
        recreate_list = nullptr;

    std::unique_ptr<NET_SCAN_PAD_REMOVE> pad( new NET_SCAN_PAD_REMOVE( aPadFrom,
                                                                       this,
                                                                       aUndoRedoList,
                                                                       recreate_list,
                                                                       aLockedToo ) );
    if( pad )
        pad->Execute();
}

void TEARDROPS::Remove( const TRACK* aTrackSegFrom,
                        const BOARD_CONNECTED_ITEM* aViaOrPadFrom,
                        PICKED_ITEMS_LIST* aUndoRedoList,
                        const bool aLockedToo
                      )
{
    if( aTrackSegFrom )
        if( aTrackSegFrom->Type() == PCB_TRACE_T )
        {
            DLIST<TRACK>* tracks_list = static_cast<DLIST<TRACK>*>( aTrackSegFrom->GetList() );
            TEARDROP* tear = GetTeardrop( aTrackSegFrom, aViaOrPadFrom );
            if( tear && tracks_list )
            {
                if( aLockedToo || ( !aLockedToo && !tear->IsLocked() ) )
                {
                    Delete( tear, tracks_list, aUndoRedoList );
                }
            }
        }
}

void TEARDROPS::Remove( const MODULE* aModuleFrom,
                        PICKED_ITEMS_LIST* aUndoRedoList,
                        const bool aLockedToo
                      )
{
    if( aModuleFrom )
        if( aModuleFrom->Type() == PCB_MODULE_T )
        {
            D_PAD* pad = aModuleFrom->PadsList();
            if( pad )
            {
                m_recreate_list->clear();
                while( pad )
                {
                    Remove( pad, aUndoRedoList, CAN_RECREATE, aLockedToo );
                    pad = pad->Next();
                }
            }
        }
}

void TEARDROPS::Remove( const MODULE* aModuleFrom, const bool aUndo, const bool aLockedToo )
{
    if( aModuleFrom )
        if( aModuleFrom->Type() == PCB_MODULE_T )
        {
            PICKED_ITEMS_LIST undoredo_items;
            Remove( aModuleFrom, &undoredo_items, aLockedToo );
            if( m_EditFrame && aUndo && undoredo_items.GetCount() )
                m_EditFrame->SaveCopyInUndoList( undoredo_items, UR_NEW );
        }
}

void TEARDROPS::Remove( const int aNetCodeFrom,
                        const TEARDROPS::TEARDROPS_TYPE_TODO aTypeToDo,
                        const bool aUndo,
                        const bool aLockedToo
                      )
{
    PICKED_ITEMS_LIST undoredo_items;
    Remove( aNetCodeFrom, aTypeToDo, &undoredo_items, aLockedToo );
    if( m_EditFrame && aUndo && undoredo_items.GetCount() )
        m_EditFrame->SaveCopyInUndoList( undoredo_items, UR_DELETED );
}

TEARDROPS::NET_SCAN_NET_REMOVE::NET_SCAN_NET_REMOVE( const int aNet,
                                                     const TEARDROPS* aParent,
                                                     PICKED_ITEMS_LIST* aUndoRedoList,
                                                     const TEARDROPS::TEARDROPS_TYPE_TODO aTypeToDo,
                                                     Teardrop_Container* aRecreateList,
                                                     const bool aLockedToo
                                                   ) :
    NET_SCAN_NET_ADD( aNet, aParent, aUndoRedoList, aTypeToDo )
{
    m_recreate_list = aRecreateList;
    m_locked_too = aLockedToo;
}

bool TEARDROPS::NET_SCAN_NET_REMOVE::ExecuteAt( TRACK* aTrack )
{
    if( aTrack->Type() == PCB_TEARDROP_T )
    {
        if( m_locked_too || ( !m_locked_too && !aTrack->IsLocked() ) )
        {
            bool is_T = false;
            bool is_junction = false;
            if( dynamic_cast<TEARDROP_JUNCTIONS*>( const_cast<TRACK*>( aTrack ) ) )
            {
                is_junction = true;
                if( ( m_type_todo == ONLY_JUNCTIONS_T ) || ( m_type_todo == ONLY_TJUNCTIONS_T ) )
                {
                    is_T = bool( dynamic_cast<TEARDROP_JUNCTIONS*>( const_cast<TRACK*>( aTrack ) )->Get_T_SegNext() &&
                                 dynamic_cast<TEARDROP_JUNCTIONS*>( const_cast<TRACK*>( aTrack ) )->Get_T_SegBack() );
                }
            }
            if( ( m_type_todo == ALL_TYPES_T ) ||
                ( ( m_type_todo == ONLY_TJUNCTIONS_T ) && is_T ) ||
                ( ( m_type_todo == ONLY_JUNCTIONS_T ) && is_junction && !is_T ) ||
                ( ( m_type_todo == ONLY_TEARDROPS_T ) && !is_junction ) ||
                ( ( m_type_todo == JUNCTIONS_AND_TJUNCTIONS_T ) && is_junction ) )
            {
                m_recreate_list->insert( static_cast<TEARDROP*>( const_cast<TRACK*>( aTrack ) ) );
            }
        }
    }
    return false;
}

//Save removed teardrops in list, recreating for.
void TEARDROPS::Remove( const int aNetCodeFrom,
                        const TEARDROPS::TEARDROPS_TYPE_TODO aTypeToDo,
                        PICKED_ITEMS_LIST* aUndoRedoList,
                        const bool aLockedToo
                      )
{
    DLIST<TRACK>* tracks_list = &m_Board->m_Track;  //? GetXxx
    m_recreate_list->clear();

    std::unique_ptr<NET_SCAN_NET_REMOVE> net( new NET_SCAN_NET_REMOVE( aNetCodeFrom,
                                                                       this,
                                                                       aUndoRedoList,
                                                                       aTypeToDo,
                                                                       m_recreate_list,
                                                                       aLockedToo ) );
    if( net )
        net->Execute();

    for( TEARDROP* tear : *m_recreate_list )
    {
        if( tear )
        {
            Delete( tear, tracks_list, aUndoRedoList );
        }
    }
}

void TEARDROPS::Change( const BOARD_CONNECTED_ITEM* aItemAt )
{
    if( aItemAt )
    {
        PICKED_ITEMS_LIST undoredo_items;

        if( dynamic_cast<TEARDROP*>( const_cast<BOARD_CONNECTED_ITEM*>( aItemAt ) ) )
        {
            Change( static_cast<TEARDROP*>( const_cast<BOARD_CONNECTED_ITEM*>( aItemAt ) ),
                    &undoredo_items );
        }

        if( dynamic_cast<VIA*>( const_cast<BOARD_CONNECTED_ITEM*>( aItemAt ) ) )
        {
            Change( static_cast<VIA*>( const_cast<BOARD_CONNECTED_ITEM*>( aItemAt ) ),
                    &undoredo_items );
        }

        if( dynamic_cast<D_PAD*>( const_cast<BOARD_CONNECTED_ITEM*>( aItemAt ) ) )
        {
            Change( static_cast<D_PAD*>( const_cast<BOARD_CONNECTED_ITEM*>( aItemAt ) ),
                    &undoredo_items );
        }

        if( m_EditFrame && undoredo_items.GetCount() )
            m_EditFrame->SaveCopyInUndoList( undoredo_items, UR_CHANGED );
    }
}

void TEARDROPS::Change( TEARDROP* aTeardrop, PICKED_ITEMS_LIST* aUndoRedoList )
{
    if( aTeardrop && ( aTeardrop->Type() == PCB_TEARDROP_T ) )
    {
        TRACK* track_seg = aTeardrop->GetTrackSeg();
        wxPoint pos = aTeardrop->GetEnd();
        BOARD_CONNECTED_ITEM* connected_item = aTeardrop->GetConnectedItem();
        Remove( aTeardrop, aUndoRedoList, false, false );
        Add( track_seg, connected_item, aUndoRedoList, pos );
    }
}

void TEARDROPS::Change( const VIA* aViaAt, PICKED_ITEMS_LIST* aUndoRedoList )
{
    if( aViaAt && ( aViaAt->Type() == PCB_VIA_T ) )
    {
        m_recreate_list->clear();
        Remove( static_cast<VIA*>( const_cast<VIA*>( aViaAt ) ), aUndoRedoList, true, false );

        for( TEARDROP* tear : *m_recreate_list )
            Add( tear->GetTrackSeg(), tear->GetConnectedItem(), aUndoRedoList, tear->GetEnd() );
    }
}

void TEARDROPS::Change( const D_PAD* aPadAt, PICKED_ITEMS_LIST* aUndoRedoList )
{
    if( aPadAt && ( aPadAt->Type() == PCB_PAD_T ) )
    {
        m_recreate_list->clear();
        Remove( static_cast<D_PAD*>( const_cast<D_PAD*>( aPadAt ) ), aUndoRedoList, true, false );

        for( TEARDROP* tear : *m_recreate_list )
            Add( tear->GetTrackSeg(), tear->GetConnectedItem(), aUndoRedoList );
    }
}

void TEARDROPS::Change( const int aNetCodeAt, const TEARDROPS::TEARDROPS_TYPE_TODO aTypeToDo )
{
    PICKED_ITEMS_LIST undoredo_items;

    m_recreate_list->clear();
    Remove( aNetCodeAt, aTypeToDo, &undoredo_items, false );

    for( TEARDROP* tear : *m_recreate_list )
        Add( tear->GetTrackSeg(), tear->GetConnectedItem(), &undoredo_items, tear->GetEnd() );

    if( m_EditFrame && undoredo_items.GetCount() )
        m_EditFrame->SaveCopyInUndoList( undoredo_items, UR_CHANGED );
}

void TEARDROPS::Change( const MODULE* aModuleAt )
{
    if( aModuleAt && aModuleAt->Type() == PCB_MODULE_T )
    {
        PICKED_ITEMS_LIST undoredo_items;

        m_recreate_list->clear();
        Remove( aModuleAt, &undoredo_items, false );

        for( TEARDROP* tear : *m_recreate_list )
            Add( tear->GetTrackSeg(), tear->GetConnectedItem(), &undoredo_items );

        if( m_EditFrame && undoredo_items.GetCount() )
            m_EditFrame->SaveCopyInUndoList( undoredo_items, UR_CHANGED );
    }
}

TEARDROPS::NET_SCAN_NET_RECREATE::NET_SCAN_NET_RECREATE( const int aNet,
                                                         const TEARDROPS* aParent,
                                                         PICKED_ITEMS_LIST* aUndoRedoList,
                                                         Teardrop_Container* aRecreateList
                                                       ) :
    NET_SCAN_NET_REMOVE( aNet, aParent, aUndoRedoList, TEARDROPS::ALL_TYPES_T, aRecreateList, false )
{
    m_current_shape = dynamic_cast<TEARDROPS*>( m_Parent )->GetCurrentShape();
    m_teardrop_params = dynamic_cast<TEARDROPS*>( m_Parent )->GetShapeParams( TEARDROP::TEARDROP_T );
    m_fillet_params = dynamic_cast<TEARDROPS*>( m_Parent )->GetShapeParams( TEARDROP::FILLET_T );
    m_subland_params = dynamic_cast<TEARDROPS*>( m_Parent )->GetShapeParams( TEARDROP::SUBLAND_T );
}

TEARDROPS::NET_SCAN_NET_RECREATE::~NET_SCAN_NET_RECREATE()
{
    dynamic_cast<TEARDROPS*>( m_Parent )->SetShapeParams( m_teardrop_params );
    dynamic_cast<TEARDROPS*>( m_Parent )->SetShapeParams( m_fillet_params );
    dynamic_cast<TEARDROPS*>( m_Parent )->SetShapeParams( m_subland_params );
    dynamic_cast<TEARDROPS*>( m_Parent )->SetCurrentShape( m_current_shape );
    dynamic_cast<TEARDROPS*>( m_Parent )->RecreateMenu();
}

bool TEARDROPS::NET_SCAN_NET_RECREATE::ExecuteAt( TRACK* aTrack )
{
    if( aTrack->Type() == PCB_TRACE_T )
    {
        for( TEARDROP* tear : *m_recreate_list )
        {
            wxPoint pos = tear->GetEnd();
            if( ( aTrack->GetStart() == pos ) || ( aTrack->GetEnd() == pos ) )
            {
                int track_angle = Rad2MilsInt( TrackSegAngle( aTrack, pos ) );
                int tear_angle = Rad2MilsInt( tear->GetTrackSegAngle() );
                if( ( aTrack->IsOnLayer( tear->GetLayer() ) ) && ( track_angle == tear_angle ) )
                {
                    TEARDROP::PARAMS params = tear->GetParams();
                    TEARDROP::SHAPES_T current_shape = dynamic_cast<TEARDROPS*>( m_Parent )->GetCurrentShape();
                    if( params != dynamic_cast<TEARDROPS*>( m_Parent )->GetShapeParams( current_shape ) )
                    {
                        dynamic_cast<TEARDROPS*>( m_Parent )->SetShapeParams( params );
                        dynamic_cast<TEARDROPS*>( m_Parent )->SetCurrentShape( params.shape );
                    }

                    BOARD_CONNECTED_ITEM* citem = tear->GetConnectedItem();
                    //if tear is junction and center of via or pad. Convert it teardrop.
                    if( !citem )
                    {
                        //Check inside Pad.
                        BOARD* board = dynamic_cast<TEARDROPS*>( m_Parent )->m_Board;
                        BOARD_CONNECTED_ITEM* in_item = board->GetLockPoint( pos, aTrack->GetLayerSet() );
                        if( in_item && ( in_item->Type() == PCB_PAD_T ) && ( in_item->GetPosition() == pos ) )
                            citem = in_item;

                        if( !citem )
                        {
                            //Check inside Via.
                            Tracks_Container result_list;
                            VIA* via_at = m_Parent->GetBoard()->TrackItems()->GetBadConnectedVia( aTrack,
                                                                                                  pos,
                                                                                                  &result_list );
                            if( via_at && ( via_at->GetEnd() == pos ) )
                                citem = via_at;
                        }
                    }

                    //if Via is removed do not recreate tears.
                    if( !( dynamic_cast<VIA*>( citem ) &&
                        !( citem->Next() || citem->Back() || citem->GetList() ) ) )
                    {
                        TEARDROP* added_tear = dynamic_cast<TEARDROPS*>( m_Parent )->Add( aTrack,
                                                                                          citem,
                                                                                          m_picked_items,
                                                                                          pos );
                        if( added_tear )
                            added_tear->SetLocked( tear->IsLocked() );
                    }
                }
            }
        }
    }
    return false;
}

void TEARDROPS::Recreate( const int aNetCodeTo, PICKED_ITEMS_LIST* aUndoRedoList )
{
    std::unique_ptr<NET_SCAN_NET_RECREATE> net( new NET_SCAN_NET_RECREATE( aNetCodeTo,
                                                                           this,
                                                                           aUndoRedoList,
                                                                           m_recreate_list ) );
    if( net )
        net->Execute();
}

void TEARDROPS::Recreate( const int aNetCodeTo, const bool aUndo )
{
    PICKED_ITEMS_LIST undoredo_list;
    Recreate( aNetCodeTo, &undoredo_list );
    if( aUndo && m_EditFrame && undoredo_list.GetCount() )
        m_EditFrame->SaveCopyInUndoList( undoredo_list, UR_NEW );
}

//move, rotate, flip, drag modules for.
void TEARDROPS::Recreate( const MODULE* aModuleTo, PICKED_ITEMS_LIST* aUndoRedoList )
{
    if( aModuleTo )
        if( aModuleTo->Type() == PCB_MODULE_T )
        {
            D_PAD* pad = aModuleTo->PadsList();
            while( pad )
            {
                Recreate( pad, aUndoRedoList );
                pad = pad->Next();
            }
        }
}

void TEARDROPS::Recreate( const MODULE* aModuleTo, const bool aUndo )
{
    if( aModuleTo )
        if( aModuleTo->Type() == PCB_MODULE_T )
        {
            PICKED_ITEMS_LIST undoredo_list;
            Recreate( aModuleTo, &undoredo_list );
            if( aUndo && m_EditFrame && undoredo_list.GetCount() )
                m_EditFrame->SaveCopyInUndoList( undoredo_list, UR_NEW );
        }
}

TEARDROPS::NET_SCAN_PAD_RECREATE::NET_SCAN_PAD_RECREATE( const D_PAD* aPad,
                                                         const TEARDROPS* aParent,
                                                         PICKED_ITEMS_LIST* aUndoRedoList,
                                                         Teardrop_Container* aRecreateList
                                                       ) :
    NET_SCAN_PAD_REMOVE( aPad, aParent, aUndoRedoList, aRecreateList, false )
{
    m_current_shape = m_Parent->GetCurrentShape();
    m_teardrop_params = m_Parent->GetShapeParams( TEARDROP::TEARDROP_T );
    m_fillet_params = m_Parent->GetShapeParams( TEARDROP::FILLET_T );
    m_subland_params = m_Parent->GetShapeParams( TEARDROP::SUBLAND_T );
}

TEARDROPS::NET_SCAN_PAD_RECREATE::~NET_SCAN_PAD_RECREATE()
{
    m_Parent->SetShapeParams( m_teardrop_params );
    m_Parent->SetShapeParams( m_fillet_params );
    m_Parent->SetShapeParams( m_subland_params );
    m_Parent->SetCurrentShape( m_current_shape );
    m_Parent->RecreateMenu();
}

bool TEARDROPS::NET_SCAN_PAD_RECREATE::ExecuteAt( TRACK* aTrack )
{
    for( TEARDROP* tear : *m_recreate_list )
    {
        wxPoint tear_pos = tear->GetEnd();
        if( m_pad_pos == tear_pos )
        {
            double angle_track = TrackSegAngle( aTrack, m_pad_pos );
            if( ( aTrack->IsOnLayer( tear->GetLayer() ) ) &&
                ( tear->GetTrackSegAngle() == angle_track ) )
            {
                TEARDROP::PARAMS params = tear->GetParams();
                if( params != m_Parent->GetShapeParams( m_Parent->GetCurrentShape() ) )
                {
                    m_Parent->SetShapeParams( params );
                    m_Parent->SetCurrentShape( params.shape );
                }
                TEARDROP* added_tear = m_Parent->Add( aTrack, m_pad, m_picked_items );
                if( added_tear )
                    added_tear->SetLocked( tear->IsLocked() );
            }
        }
    }
    return false;
}

void TEARDROPS::Recreate( const D_PAD* aPadTo, PICKED_ITEMS_LIST* aUndoRedoList )
{
    if( aPadTo )
    {
        std::unique_ptr<NET_SCAN_PAD_RECREATE> pad( new NET_SCAN_PAD_RECREATE( aPadTo,
                                                                               this,
                                                                               aUndoRedoList,
                                                                               m_recreate_list ) );
        if( pad )
            pad->Execute();
    }
}

void TEARDROPS::Recreate( const D_PAD* aPadTo, const bool aUndo )
{
    if( aPadTo )
    {
        PICKED_ITEMS_LIST undoredo_list;
        Recreate( aPadTo, &undoredo_list );
        if( aUndo && m_EditFrame && undoredo_list.GetCount() )
            m_EditFrame->SaveCopyInUndoList( undoredo_list, UR_NEW );
    }
}

void TEARDROPS::Repopulate( const int aNetCodeAt,
                            const TEARDROPS::TEARDROPS_TYPE_TODO aTypeToDo,
                            const bool aUndo
                          )
{
    PICKED_ITEMS_LIST undoredo_items;
    Repopulate( aNetCodeAt, aTypeToDo, &undoredo_items );
    if( m_EditFrame && aUndo && undoredo_items.GetCount())
        m_EditFrame->SaveCopyInUndoList( undoredo_items, UR_DELETED );
}

void TEARDROPS::Repopulate( const int aNetCodeAt,
                            const TEARDROPS::TEARDROPS_TYPE_TODO aTypeToDo,
                            PICKED_ITEMS_LIST* aUndoRedoList
                          )
{
    Remove( aNetCodeAt, aTypeToDo, aUndoRedoList, true );
    Recreate( aNetCodeAt, aUndoRedoList );
}

void TEARDROPS::Repopulate( DLIST<TRACK>* aTracksAll, const bool aUndo )
{
    PICKED_ITEMS_LIST undoredo_items;

    int netcode = -1;
    for( TRACK* track = aTracksAll->GetFirst();  track;  track = track->Next() )
    {
        int track_netcode = track->GetNetCode();
        if( track_netcode != netcode )
        {
            Repopulate( track_netcode, ALL_TYPES_T, &undoredo_items );
            netcode = track_netcode;
        }
    }

    if( m_EditFrame && aUndo && undoredo_items.GetCount() )
        m_EditFrame->SaveCopyInUndoList( undoredo_items, UR_DELETED );
}

void TEARDROPS::ToMemory( const TRACK* aTrackSegFrom )
{
    m_next_tear_in_memory = nullptr;
    m_back_tear_in_memory = nullptr;

    TRACKNODEITEM* item = Get( aTrackSegFrom, aTrackSegFrom->GetEnd() );
    if( item && dynamic_cast<TEARDROP*>( item ) )
        m_next_tear_in_memory = static_cast<TEARDROP*>( item );

    item = Get( aTrackSegFrom, aTrackSegFrom->GetStart() );
    if( item && dynamic_cast<TEARDROP*>( item ) )
        m_back_tear_in_memory = static_cast<TEARDROP*>( item );
}

void TEARDROPS::FromMemory( const TRACK* aTrackSegTo, PICKED_ITEMS_LIST* aUndoRedoList )
{
    TEARDROP::SHAPES_T current_shape = GetCurrentShape();
    TEARDROP::PARAMS teardrop_params = GetShapeParams( TEARDROP::TEARDROP_T );
    TEARDROP::PARAMS fillet_params = GetShapeParams( TEARDROP::FILLET_T );
    TEARDROP::PARAMS subland_params = GetShapeParams( TEARDROP::SUBLAND_T );
    if( m_next_tear_in_memory ) //Recreate next teardrop.
    {
        TEARDROP::PARAMS params = m_next_tear_in_memory->GetParams();
        SetShapeParams( params );
        SetCurrentShape( params.shape );
        TEARDROP* added_tear =Add( aTrackSegTo,
                                   m_next_tear_in_memory->GetConnectedItem(),
                                   aUndoRedoList,
                                   m_next_tear_in_memory->GetEnd() );
        if( added_tear )
            added_tear->SetLocked( m_next_tear_in_memory->IsLocked() );
    }
    if( m_back_tear_in_memory ) //Recreate back teardrop.
    {
        TEARDROP::PARAMS params = m_back_tear_in_memory->GetParams();
        SetShapeParams( params );
        SetCurrentShape( params.shape );
        TEARDROP* added_tear = Add( aTrackSegTo,
                                    m_back_tear_in_memory->GetConnectedItem(),
                                    aUndoRedoList,
                                    m_back_tear_in_memory->GetEnd() );
        if( added_tear )
            added_tear->SetLocked( m_back_tear_in_memory->IsLocked() );
    }
    SetShapeParams( teardrop_params );
    SetShapeParams( fillet_params );
    SetShapeParams( subland_params );
    SetCurrentShape( current_shape );
    RecreateMenu();
}

void TEARDROPS::FromMemory( const TRACK* aTrackSegTo )
{
    PICKED_ITEMS_LIST undo_redo_list;
    FromMemory( aTrackSegTo, &undo_redo_list );
}

void TEARDROPS::FromMemory( const TRACK* aTrackSegTo, BOARD_COMMIT& aCommit )
{
    PICKED_ITEMS_LIST undo_redo_list;
    FromMemory( aTrackSegTo, &undo_redo_list );

    unsigned int num_added_tears = undo_redo_list.GetCount();
    if( num_added_tears )
        for( unsigned int n = 0; n < num_added_tears; ++n )
            aCommit.Added( undo_redo_list.GetPickedItem( n ) );
}

void TEARDROPS::LockToggle( const TRACK* aTrackSegAt, const wxPoint& aCurPosAt )
{
    wxPoint track_pos = TrackSegNearestEndpoint( aTrackSegAt, aCurPosAt );
    TRACKNODEITEM* item = Get( aTrackSegAt, track_pos );
    if( item && dynamic_cast<TEARDROP*>( item ) )
    {
        TEARDROP* tear = static_cast<TEARDROP*>( item );
        tear->SetLocked( !tear->IsLocked() );
    }
}

TEARDROPS::NET_SCAN_PAD_LOCK::NET_SCAN_PAD_LOCK( const D_PAD* aPad,
                                                 const TEARDROPS* aParent
                                               ) :
    NET_SCAN_PAD_BASE( aPad, aParent )
{
}

bool TEARDROPS::NET_SCAN_PAD_LOCK::ExecuteAt( TRACK* aTrack )
{
    TEARDROP* tear = m_Parent->GetTeardrop( aTrack, m_pad );
    if( tear )
    {
        tear->SetLocked( true );
    }
    return false;
}

void TEARDROPS::Lock( const D_PAD* aPadAt )
{
    if( aPadAt )
    {
        std::unique_ptr<NET_SCAN_PAD_LOCK> pad( new NET_SCAN_PAD_LOCK( aPadAt, this ) );
        if( pad )
            pad->Execute();
    }
}

void TEARDROPS::Lock( const MODULE* aModuleAt )
{
    if( aModuleAt )
    {
        D_PAD* pad = aModuleAt->PadsList();
        if( pad )
        {
            while( pad )
            {
                Lock( pad );
                pad = pad->Next();
            }
        }
    }
}

TEARDROPS::NET_SCAN_PAD_UNLOCK::NET_SCAN_PAD_UNLOCK( const D_PAD* aPad,
                                                     const TEARDROPS* aParent
                                                   ) :
    NET_SCAN_PAD_BASE( aPad, aParent )
{
}

bool TEARDROPS::NET_SCAN_PAD_UNLOCK::ExecuteAt( TRACK* aTrack )
{
    TEARDROP* tear = m_Parent->GetTeardrop( aTrack, m_pad );
    if( tear )
    {
        tear->SetLocked( false );
    }
    return false;
}

void TEARDROPS::Unlock( const D_PAD* aPadAt )
{
    if( aPadAt )
    {
        std::unique_ptr<NET_SCAN_PAD_UNLOCK> pad( new NET_SCAN_PAD_UNLOCK( aPadAt, this ) );
        if( pad )
            pad->Execute();
    }
}

void TEARDROPS::Unlock( const MODULE* aModuleAt )
{
    if( aModuleAt )
    {
        D_PAD* pad = aModuleAt->PadsList();
        if( pad )
        {
            while( pad )
            {
                Unlock( pad );
                pad = pad->Next();
            }
        }
    }
}

TEARDROPS::NET_SCAN_VIA_LOCK::NET_SCAN_VIA_LOCK( const VIA* aVia,
                                                 const TEARDROPS* aParent
                                               ) :
    NET_SCAN_VIA_UPDATE( aVia, aParent )
{
}

bool TEARDROPS::NET_SCAN_VIA_LOCK::ExecuteAt( TRACK* aTrack )
{
    if( aTrack->Type() == PCB_TRACE_T )
    {
        TEARDROP* tear = dynamic_cast<TEARDROPS*>( m_Parent )->GetTeardrop( aTrack, m_via );
        if( tear )
        {
            tear->SetLocked( true );
        }
    }
    return false;
}

void TEARDROPS::Lock( const VIA* aViaAt )
{
    if( aViaAt )
    {
        std::unique_ptr<NET_SCAN_VIA_LOCK> via( new NET_SCAN_VIA_LOCK( aViaAt, this ) );
        if( via )
            via->Execute();
    }
}

TEARDROPS::NET_SCAN_VIA_UNLOCK::NET_SCAN_VIA_UNLOCK( const VIA* aVia,
                                                     const TEARDROPS* aParent
                                                   ) :
    NET_SCAN_VIA_UPDATE( aVia, aParent )
{
}

bool TEARDROPS::NET_SCAN_VIA_UNLOCK::ExecuteAt( TRACK* aTrack )
{
    if( aTrack->Type() == PCB_TRACE_T )
    {
        TEARDROP* tear = dynamic_cast<TEARDROPS*>( m_Parent )->GetTeardrop( aTrack, m_via );
        if( tear )
        {
            tear->SetLocked( false );
        }
    }
    return false;
}

void TEARDROPS::Unlock( const VIA* aViaAt )
{
    if( aViaAt )
    {
        std::unique_ptr<NET_SCAN_VIA_UNLOCK> via( new NET_SCAN_VIA_UNLOCK( aViaAt, this ) );
        if( via )
            via->Execute();
    }
}

void TEARDROPS::Lock( const BOARD_CONNECTED_ITEM* aViaOrPadAt )
{
    if( aViaOrPadAt->Type() == PCB_VIA_T )
        Lock( static_cast<VIA*>( const_cast<BOARD_CONNECTED_ITEM*>( aViaOrPadAt ) ) );
    if( aViaOrPadAt->Type() == PCB_PAD_T )
        Lock( static_cast<D_PAD*>( const_cast<BOARD_CONNECTED_ITEM*>( aViaOrPadAt ) ) );
}

void TEARDROPS::Unlock( const BOARD_CONNECTED_ITEM* aViaOrPadAt )
{
    if( aViaOrPadAt->Type() == PCB_VIA_T )
        Unlock( static_cast<VIA*>( const_cast<BOARD_CONNECTED_ITEM*>( aViaOrPadAt ) ) );
    if( aViaOrPadAt->Type() == PCB_PAD_T )
        Unlock( static_cast<D_PAD*>( const_cast<BOARD_CONNECTED_ITEM*>( aViaOrPadAt ) ) );
}

bool TEARDROPS::NET_SCAN_NET_LOCK::ExecuteAt( TRACK* aTrack )
{
    if( aTrack->Type() == PCB_TEARDROP_T )
        dynamic_cast<TEARDROP*> ( const_cast<TRACK*>( aTrack ) )->SetLocked( true );
    return false;
}

void TEARDROPS::Lock( const int aNetCodeAt, const TRACK* aStartTrack )
{
    std::unique_ptr<NET_SCAN_NET_LOCK> net( new NET_SCAN_NET_LOCK( aNetCodeAt, aStartTrack, this ) );
    if( net )
        net->Execute();
}

bool TEARDROPS::NET_SCAN_NET_UNLOCK::ExecuteAt( TRACK* aTrack )
{
    if( aTrack->Type() == PCB_TEARDROP_T )
        dynamic_cast<TEARDROP*> ( const_cast<TRACK*>( aTrack ) )->SetLocked( false );
    return false;
}

void TEARDROPS::Unlock( const int aNetCodeAt, const TRACK* aStartTrack )
{
    std::unique_ptr<NET_SCAN_NET_UNLOCK> net( new NET_SCAN_NET_UNLOCK( aNetCodeAt, aStartTrack, this ) );
    if( net )
        net->Execute();
}

TEARDROPS::NET_SCAN_PAD_EMPTY::NET_SCAN_PAD_EMPTY( const D_PAD* aPad,
                                                   const TEARDROPS* aParent
                                                 ) :
    NET_SCAN_PAD_BASE( aPad, aParent )
{
    m_num_tears = 0;
}

bool TEARDROPS::NET_SCAN_PAD_EMPTY::ExecuteAt( TRACK* aTrack )
{
    TRACKNODEITEM* item = m_Parent->Get( aTrack, m_pad_pos );
    TEARDROP* tear = nullptr;
    if( item && dynamic_cast<TEARDROP*>( item ) )
        tear = static_cast<TEARDROP*>( item );
    if( !tear )
    {
        ++m_num_tears;
    }
    return false;
}

bool TEARDROPS::Empty( const D_PAD* aPadTo ) const
{
    bool result = false;
    if( aPadTo )
    {
        std::unique_ptr<NET_SCAN_PAD_EMPTY> pad( new NET_SCAN_PAD_EMPTY( aPadTo, this ) );
        if( pad )
        {
            pad->Execute();
            result = pad->NumTeardrops();
        }
    }
    return result;
}

TEARDROPS::NET_SCAN_VIA_EMPTY::NET_SCAN_VIA_EMPTY( const VIA* aVia,
                                                   const TEARDROPS* aParent
                                                 ) :
    NET_SCAN_VIA_UPDATE( aVia, aParent )
{
    m_num_tears = 0;
}

bool TEARDROPS::NET_SCAN_VIA_EMPTY::ExecuteAt( TRACK* aTrack )

{
    if( aTrack->Type() == PCB_TRACE_T )
    {
        if( m_via->IsOnLayer( aTrack->GetLayer() ) &&
            ( ( aTrack->GetStart() == m_via_pos ) ||
            ( aTrack->GetEnd() == m_via_pos ) ) )
        {
            if( !dynamic_cast<TEARDROPS*>( m_Parent )->Get( aTrack, m_via_pos ) )
            {
                ++m_num_tears;
            }
        }
    }
    return false;
}

bool TEARDROPS::Empty( const VIA* aViaTo ) const
{
    bool result = false;
    if( aViaTo )
    {
        std::unique_ptr<NET_SCAN_VIA_EMPTY> via( new NET_SCAN_VIA_EMPTY( aViaTo, this ) );
        if( via )
        {
            via->Execute();
            result = via->NumTeardrops();
        }
    }
    return result;
}

bool TEARDROPS::Empty( const MODULE* aModuleTo ) const
{
    if( aModuleTo )
    {
        D_PAD* pad = aModuleTo->PadsList();
        while( pad )
        {
            if( Empty( pad ) )
                return true;
            pad = pad->Next();
        }
    }
    return false;
}

TEARDROPS::NET_SCAN_NET_EMPTY::NET_SCAN_NET_EMPTY( const int aNet,
                                                   const TRACK* aStartTrack,
                                                   const TEARDROPS* aParent,
                                                   const TEARDROPS::TEARDROPS_TYPE_TODO aTypeToDo
                                                 ) :
    NET_SCAN_NET_UPDATE( aNet, aStartTrack, aParent )
{
    m_type_todo = aTypeToDo;
    m_result_value = false;
}

bool TEARDROPS::NET_SCAN_NET_EMPTY::ExecuteAt( TRACK* aTrack )
{
    if( aTrack->Type() == PCB_TRACE_T )
    {
        //Teardrops of Pads.
        if( ( m_type_todo == ALL_TYPES_T ) || ( m_type_todo == ONLY_TEARDROPS_T ) )
        {
#ifdef NEWCONALGO
            auto connity = m_Parent->GetBoard()->GetConnectivity();
            auto pads = connity->GetConnectedPads( aTrack );
            for( auto pad : pads )
            {
                wxPoint pad_pos = pad->GetPosition();
                if( ( aTrack->GetStart() == pad_pos ) || ( aTrack->GetEnd() == pad_pos ) )
                {
                    TEARDROP* tear = nullptr;
                    TRACKNODEITEM* item = dynamic_cast<TEARDROPS*>( m_Parent )->Get( aTrack, pad_pos );
                    if( item && dynamic_cast<TEARDROP*>( item ) )
                        tear = static_cast<TEARDROP*>( item );
                    if( !tear )
                    {
                        m_result_value = true;
                        return true;
                    }
                }
            }
#else
            for( unsigned int m = 0; m < aTrack->m_PadsConnected.size(); ++m )
            {
                wxPoint pad_pos = aTrack->m_PadsConnected.at( m )->GetPosition();
                if( ( aTrack->GetStart() == pad_pos ) || ( aTrack->GetEnd() == pad_pos ) )
                {
                    TEARDROP* tear = nullptr;
                    TRACKNODEITEM* item = dynamic_cast<TEARDROPS*>( m_Parent )->Get( aTrack, pad_pos );
                    if( item && dynamic_cast<TEARDROP*>( item ) )
                        tear = static_cast<TEARDROP*>( item );
                    if( !tear )
                    {
                        m_result_value = true;
                        return true;
                    }
                }
            }
#endif
        }
        //T-Junctoins and Junctions of Traces.
        if( ( m_type_todo == ALL_TYPES_T ) ||
            ( m_type_todo == ONLY_TJUNCTIONS_T )||
            ( m_type_todo == ONLY_JUNCTIONS_T ) ||
            ( m_type_todo == JUNCTIONS_AND_TJUNCTIONS_T ) )
        {
            for( unsigned int m = 0; m < 2; ++m )
            {
                wxPoint track_pos;
                ( m )? track_pos = aTrack->GetStart() : track_pos = aTrack->GetEnd();
                VIA* via = m_Parent->GetParent()->GetVia( aTrack, track_pos );
                D_PAD* pad = m_Parent->GetParent()->GetPad( aTrack, track_pos );
                if( !via && !pad )
                {
                    if( !( m_type_todo == ONLY_JUNCTIONS_T ) )
                    {
                        TRACK* track_next = nullptr;
                        TRACK* track_back = nullptr;
                        if( Find_T_Tracks( aTrack, track_pos, track_next, track_back ) )
                        {
                            TEARDROP* tear = nullptr;
                            TRACKNODEITEM* item = dynamic_cast<TEARDROPS*>( m_Parent )->Get( aTrack, track_pos );
                            if( item && dynamic_cast<TEARDROP*>( item ) )
                                tear = static_cast<TEARDROP*>( item );
                            if( !tear )
                            {
                                m_result_value = true;
                                return true;
                            }
                        }
                    }
                    if( !( m_type_todo == ONLY_TJUNCTIONS_T ) )
                    {
                        Tracks_Container tracks_list;
                        Collect( aTrack, track_pos, tracks_list );
                        if( tracks_list.size() )
                            if( TrackNodeItem::GetMaxWidth( tracks_list ) > aTrack->GetWidth() )
                            {
                                TEARDROP* tear = nullptr;
                                TRACKNODEITEM* item = dynamic_cast<TEARDROPS*>( m_Parent )->Get( aTrack, track_pos );
                                if( item && dynamic_cast<TEARDROP*>( item ) )
                                    tear = static_cast<TEARDROP*>( item );
                                if( !tear )
                                {
                                    m_result_value = true;
                                    return true;
                                }
                            }
                    }
                }
            }
        }
    }

    //Teardrops of Vias.
    if( ( aTrack->Type() == PCB_VIA_T ) &&
        ( ( m_type_todo == ALL_TYPES_T ) ||
        ( m_type_todo == ONLY_TEARDROPS_T ) ) )
    {
        if( dynamic_cast<TEARDROPS*>( m_Parent )->Empty( static_cast<VIA*>( const_cast<TRACK*>( aTrack ) ) ) )
        {
            m_result_value = true;
            return true;
        }
    }

    return false;
}


bool TEARDROPS::Empty( const int aNetCodeTo,
                       const TRACK* aStartTrack,
                       const TEARDROPS::TEARDROPS_TYPE_TODO aTypeToDo
                     ) const
{
    bool result = false;
    std::unique_ptr<NET_SCAN_NET_EMPTY> net( new NET_SCAN_NET_EMPTY( aNetCodeTo,
                                                                     aStartTrack,
                                                                     this,
                                                                     aTypeToDo ) );
    if( net )
    {
        net->Execute();
        result = net->GetResult();
    }
    return result;
}

TEARDROPS::NET_SCAN_PAD_CONTAINS::NET_SCAN_PAD_CONTAINS( const D_PAD* aPad,
                                                         const TEARDROPS* aParent
                                                       ) :
    NET_SCAN_PAD_EMPTY( aPad, aParent )
{
}

bool TEARDROPS::NET_SCAN_PAD_CONTAINS::ExecuteAt( TRACK* aTrack )
{
    TEARDROP* tear = m_Parent->GetTeardrop( aTrack, m_pad );
    if( tear )
    {
        ++m_num_tears;
        if( tear->IsLocked() )
            ++m_num_locked;
    }
    return false;
}

int TEARDROPS::Contains( const D_PAD* aPadAt, int& aNumLocked ) const
{
    int num_tears = 0;
    aNumLocked = 0;
    if( aPadAt )
    {
        std::unique_ptr<NET_SCAN_PAD_CONTAINS> pad( new NET_SCAN_PAD_CONTAINS( aPadAt, this ) );
        if( pad )
        {
            pad->Execute();
            num_tears = pad->NumTeardrops();
            aNumLocked = pad->NumLocked();
        }
    }
    return num_tears;
}


TEARDROPS::NET_SCAN_VIA_CONTAINS::NET_SCAN_VIA_CONTAINS( const VIA* aVia,
                                                         const TEARDROPS* aParent
                                                       ) :
    NET_SCAN_VIA_EMPTY( aVia, aParent )
{
}

bool TEARDROPS::NET_SCAN_VIA_CONTAINS::ExecuteAt( TRACK* aTrack )
{
    if( aTrack->Type() == PCB_TRACE_T )
    {
        TEARDROP* tear = dynamic_cast<TEARDROPS*>( m_Parent )->GetTeardrop( aTrack, m_via );
        if( tear )
        {
            ++m_num_tears;
            if( tear->IsLocked() )
                ++m_num_locked;
        }
    }
    return false;
}

int TEARDROPS::Contains( const VIA* aViaAt, int& aNumLocked ) const
{
    int num_tears = 0;
    aNumLocked = 0;
    if( aViaAt && !dynamic_cast<const VIA*>( aViaAt )->GetThermalCode() )
    {
        std::unique_ptr<NET_SCAN_VIA_CONTAINS> via( new NET_SCAN_VIA_CONTAINS( aViaAt, this ) );
        if( via )
        {
            via->Execute();
            num_tears = via->NumTeardrops();
            aNumLocked = via->NumLocked();
        }
    }
    return num_tears;
}

int TEARDROPS::Contains( const MODULE* aModuleAt, int& aNumLocked ) const
{
    int num_tears = 0;
    aNumLocked = 0;
    if( aModuleAt )
    {
        D_PAD* pad = aModuleAt->PadsList();
        while( pad )
        {
            int num_pad_locked = 0;
            int num_pad_tears = Contains( pad, num_pad_locked );
            if( num_pad_tears )
            {
                num_tears += num_pad_tears;
                aNumLocked += num_pad_locked;
            }
            pad = pad->Next();
        }
    }
    return num_tears;
}

TEARDROPS::NET_SCAN_NET_CONTAINS::NET_SCAN_NET_CONTAINS( const int aNet,
                                                         const TRACK* aStartTrack,
                                                         const TEARDROPS* aParent,
                                                         const TEARDROPS::TEARDROPS_TYPE_TODO aTypeToDo
                                                       ) :
    NET_SCAN_NET_EMPTY( aNet, aStartTrack, aParent, aTypeToDo )
{
}

bool TEARDROPS::NET_SCAN_NET_CONTAINS::ExecuteAt( TRACK* aTrack )
{
    if( aTrack->Type() == PCB_TRACE_T )
    {
        //Teardrops of Pads.
        if( ( m_type_todo == ALL_TYPES_T ) || ( m_type_todo == ONLY_TEARDROPS_T ) )
        {
#ifdef NEWCONALGO
            auto connity = m_Parent->GetBoard()->GetConnectivity();
            auto pads = connity->GetConnectedPads( aTrack );
            for( auto pad : pads )
            {
                wxPoint pad_pos = pad->GetPosition();
                if( ( aTrack->GetStart() == pad_pos ) || ( aTrack->GetEnd() == pad_pos ) )
                {
                    TEARDROP* tear = nullptr;
                    TRACKNODEITEM* item = dynamic_cast<TEARDROPS*>( m_Parent )->Get( aTrack, pad_pos );
                    if( item && dynamic_cast<TEARDROP*>( item ) )
                        tear = static_cast<TEARDROP*>( item );
                    if( tear )
                    {
                        m_result_value = true;
                        return true;
                    }
                }
            }
#else
            for( unsigned int m = 0; m < aTrack->m_PadsConnected.size(); ++m )
            {
                wxPoint pad_pos = aTrack->m_PadsConnected.at( m )->GetPosition();
                if( ( aTrack->GetStart() == pad_pos ) || ( aTrack->GetEnd() == pad_pos ) )
                {
                    TEARDROP* tear = nullptr;
                    TRACKNODEITEM* item = dynamic_cast<TEARDROPS*>( m_Parent )->Get( aTrack, pad_pos );
                    if( item && dynamic_cast<TEARDROP*>( item ) )
                        tear = static_cast<TEARDROP*>( item );
                    if( tear )
                    {
                        m_result_value = true;
                        return true;
                    }
                }
            }
#endif
        }

        //T-Junctions and Junctions of Traces.
        if( ( m_type_todo == ALL_TYPES_T ) ||
            ( m_type_todo == ONLY_JUNCTIONS_T ) ||
            ( m_type_todo == ONLY_TJUNCTIONS_T ) ||
            ( m_type_todo == JUNCTIONS_AND_TJUNCTIONS_T ) )
        {
            for( unsigned int m = 0; m < 2; ++m )
            {
                wxPoint track_pos;
                ( m )? track_pos = aTrack->GetStart() : track_pos = aTrack->GetEnd();
                VIA* via = m_Parent->GetParent()->GetVia( aTrack, track_pos );
                D_PAD* pad = m_Parent->GetParent()->GetPad( aTrack, track_pos );
                if( !via && !pad )
                {
                    if( !( m_type_todo == ONLY_JUNCTIONS_T ) )
                    {
                        TRACK* track_next = nullptr;
                        TRACK* track_back = nullptr;
                        if( Find_T_Tracks( aTrack, track_pos, track_next, track_back ) )
                        {
                            TEARDROP* tear = nullptr;
                            TRACKNODEITEM* item = dynamic_cast<TEARDROPS*>( m_Parent )->Get( aTrack, track_pos );
                            if( item && dynamic_cast<TEARDROP*>( item ) )
                                tear = static_cast<TEARDROP*>( item );
                            if( tear )
                            {
                                m_result_value = true;
                                return true;
                            }
                        }
                    }
                    if( !( m_type_todo == ONLY_TJUNCTIONS_T ) )
                    {
                        Tracks_Container tracks_list;
                        Collect( aTrack, track_pos, tracks_list );
                        if( tracks_list.size() )
                            if( TrackNodeItem::GetMaxWidth( tracks_list ) > aTrack->GetWidth() )
                            {
                                TEARDROP* tear = nullptr;
                                TRACKNODEITEM* item = dynamic_cast<TEARDROPS*>( m_Parent )->Get( aTrack, track_pos );
                                if( item && dynamic_cast<TEARDROP*>( item ) )
                                    tear = static_cast<TEARDROP*>( item );
                                if( tear )
                                {
                                    m_result_value = true;
                                    return true;
                                }
                            }
                    }
                }
            }
        }
    }

    //Teardrops of Vias.
    if( ( aTrack->Type() == PCB_VIA_T ) &&
        ( ( m_type_todo == ALL_TYPES_T ) ||
        ( m_type_todo == ONLY_TEARDROPS_T ) ) )
    {
        int num_locked_tears = 0;
        m_result_value = dynamic_cast<TEARDROPS*>( m_Parent )->Contains( static_cast<VIA*>( const_cast<TRACK*>( aTrack ) ),
                                                                         num_locked_tears );
        if( m_result_value )
            return true;
    }

    return false;
}

bool TEARDROPS::Contains( const int aNetCodeAt,
                          const TRACK* aStartTrack,
                          const TEARDROPS::TEARDROPS_TYPE_TODO aTypeToDo
                        ) const
{
    bool result = false;
    std::unique_ptr<NET_SCAN_NET_CONTAINS> net( new NET_SCAN_NET_CONTAINS( aNetCodeAt,
                                                                           aStartTrack,
                                                                           this,
                                                                           aTypeToDo ) );
    if( net )
    {
        net->Execute();
        result = net->GetResult();
    }
    return result;
}

bool TEARDROPS::IsTrimmed( const TRACK* aTrackSegAt ) const
{
    if( aTrackSegAt && ( aTrackSegAt->Type() == PCB_TRACE_T ) )
    {
        unsigned int t_length = aTrackSegAt->GetLength();
        if( dynamic_cast<ROUNDED_CORNER_TRACK*>( const_cast<TRACK*>( aTrackSegAt ) ) )
            t_length = dynamic_cast<ROUNDED_CORNER_TRACK*>( const_cast<TRACK*>( aTrackSegAt ) )->GetLengthVisible();

        TEARDROP* tear = nullptr;
        TRACKNODEITEM* item = Next( aTrackSegAt );
        if( item && dynamic_cast<TEARDROP*>( item ) )
            tear = static_cast<TEARDROP*>( item );
        if( tear && ( tear->GetCalcLength() >= t_length ) )
            return true;

        item = Back( aTrackSegAt );
        if( item && dynamic_cast<TEARDROP*>( item ) )
            tear = static_cast<TEARDROP*>( item );
        if( tear && ( tear->GetCalcLength() >= t_length ) )
            return true;
    }

    return false;
}

bool TEARDROPS::IsTrimmed( const TEARDROP* aTear ) const
{
    if( aTear && aTear->Type() == PCB_TEARDROP_T )
    {
        TRACK* track = aTear->GetTrackSeg();
        unsigned int t_length = track->GetLength();
        if( dynamic_cast<ROUNDED_CORNER_TRACK*>( track ) )
            t_length = dynamic_cast<ROUNDED_CORNER_TRACK*>( track )->GetLengthVisible();

        if( ( aTear->GetCalcLength() > t_length ) )
            return true;
    }
    return false;
}

TEARDROPS::NET_SCAN_PAD_SMALLTEARS::NET_SCAN_PAD_SMALLTEARS( const D_PAD* aPad,
                                                             const TEARDROPS* aParent
                                                           ) :
    NET_SCAN_PAD_EMPTY( aPad, aParent )
{
}

bool TEARDROPS::NET_SCAN_PAD_SMALLTEARS::ExecuteAt( TRACK* aTrack )
{
    TEARDROP* tear = m_Parent->GetTeardrop( aTrack, m_pad );
    if( tear )
        if( !tear->IsSetOK() )
        {
            ++m_num_tears;
            return true;
        }

    return false;
}

bool TEARDROPS::IsSmallTeardrops( const D_PAD* aPadAt ) const
{
    bool result = false;
    if( aPadAt )
    {
        std::unique_ptr<NET_SCAN_PAD_SMALLTEARS> pad( new NET_SCAN_PAD_SMALLTEARS( aPadAt, this ) );
        if( pad )
        {
            pad->Execute();
            result = pad->NumTeardrops();
        }
    }
    return result;
}

bool TEARDROPS::IsConnected( const D_PAD* aPadAt, const TRACK* aTrackSegTo ) const
{
    if( aPadAt && aTrackSegTo )
    {
        wxPoint pad_pos = aPadAt->GetPosition();
        if( ( aTrackSegTo->GetStart() == pad_pos ) || ( aTrackSegTo->GetEnd() == pad_pos ) )
            if( aTrackSegTo->Type() == PCB_TRACE_T )
                if( aPadAt->IsOnLayer( aTrackSegTo->GetLayer() ) )
                    return true;
    }
    return false;
}

void TEARDROPS::Update( const BOARD_ITEM* aItemAt )
{
    if( aItemAt )
    {
        if( dynamic_cast<MODULE*> ( const_cast<BOARD_ITEM*> ( aItemAt ) ) )
            Update( static_cast<MODULE*>( const_cast<BOARD_ITEM*>( aItemAt ) ) );
        if( dynamic_cast<D_PAD*> ( const_cast<BOARD_ITEM*> ( aItemAt ) ) )
            Update( static_cast<D_PAD*>( const_cast<BOARD_ITEM*>( aItemAt ) ) );
        if( dynamic_cast<TRACK*> ( const_cast<BOARD_ITEM*> ( aItemAt ) ) )
            Update( static_cast<TRACK*>( const_cast<BOARD_ITEM*>( aItemAt ) ) );
    }
}

TEARDROPS::NET_SCAN_TRACK_UPDATE::NET_SCAN_TRACK_UPDATE( const TRACK* aStartTrack,
                                                         const TEARDROPS* aParent
                                                       ) :
    NET_SCAN_BASE( aStartTrack, aParent )
{
}

bool TEARDROPS::NET_SCAN_TRACK_UPDATE::ExecuteAt( TRACK* aTrack )
{
    if( dynamic_cast<TEARDROP*>( const_cast<TRACK*>( aTrack ) ) )
        if( dynamic_cast<TEARDROP*>( const_cast<TRACK*>( aTrack ) )->GetTrackSeg() == m_scan_start_track )
            dynamic_cast<TEARDROP*>( const_cast<TRACK*>( aTrack ) )->Update();

    return false;
}

void TEARDROPS::Update( const TRACK* aTrackSegAt )
{
    if( aTrackSegAt )
    {
        if( aTrackSegAt->Type() == PCB_VIA_T )
            Update( static_cast<VIA*>( const_cast<TRACK*>( aTrackSegAt ) ) );

        if( aTrackSegAt->Type() == PCB_TEARDROP_T )
            dynamic_cast<TEARDROP*> ( const_cast<TRACK*> ( aTrackSegAt ) )->Update();

        if( aTrackSegAt->Type() == PCB_TRACE_T )
        {
            std::unique_ptr<NET_SCAN_TRACK_UPDATE> track( new NET_SCAN_TRACK_UPDATE( aTrackSegAt, this ) );
            if( track )
                track->Execute();
        }
    }
}

TEARDROPS::NET_SCAN_VIA_UPDATE::NET_SCAN_VIA_UPDATE( const VIA* aVia,
                                                     const TEARDROPS* aParent
                                                   ) :
    NET_SCAN_BASE( aVia, aParent )
{
    m_via = const_cast<VIA*>( aVia );
    m_via_pos = m_via->GetEnd();
}

bool TEARDROPS::NET_SCAN_VIA_UPDATE::ExecuteAt( TRACK* aTrack )
{
    if( aTrack->Type() == PCB_TRACE_T )
    {
        if( ( aTrack->GetStart() == m_via_pos ) || ( aTrack->GetEnd() == m_via_pos ) )
        {
            TEARDROP* tear = dynamic_cast<TEARDROPS*>( m_Parent )->GetTeardrop( aTrack, m_via );
            if( tear )
                tear->Update();
        }
    }
    return false;
}

void TEARDROPS::Update( const VIA* aViaAt )
{
    if( aViaAt )
    {
        std::unique_ptr<NET_SCAN_VIA_UPDATE> via( new NET_SCAN_VIA_UPDATE( aViaAt, this ) );
        if( via )
            via->Execute();
    }
}

TEARDROPS::NET_SCAN_PAD_BASE::NET_SCAN_PAD_BASE( const D_PAD* aPad, const TEARDROPS* aParent )
{
    m_pad = const_cast<D_PAD*>( aPad );
    m_Parent = const_cast<TEARDROPS*>( aParent );
    m_pad_pos = m_pad->GetPosition();
    m_only_exact_connected = true;
}

void TEARDROPS::NET_SCAN_PAD_BASE::Execute( void )
{
#ifdef NEWCONALGO
    auto connity = m_Parent->GetBoard()->GetConnectivity();
    auto tracks = connity->GetConnectedTracks( m_pad );
    for( auto track : tracks )
        if( !m_only_exact_connected || ( m_only_exact_connected && m_Parent->IsConnected( m_pad, track ) ) )
            if( ExecuteAt( track ) )
                return;
#else
    for( unsigned int n = 0; n < m_pad->m_TracksConnected.size(); ++n )
    {
        TRACK* track_seg = static_cast<TRACK*>( m_pad->m_TracksConnected.at( n ) );
        if( !m_only_exact_connected || ( m_only_exact_connected && m_Parent->IsConnected( m_pad, track_seg ) ) )
            if( ExecuteAt( track_seg ) )
                return;
    }
#endif
}

TEARDROPS::NET_SCAN_PAD_UPDATE::NET_SCAN_PAD_UPDATE( const D_PAD* aPad,
                                                     const TEARDROPS* aParent
                                                   ) :
    NET_SCAN_PAD_BASE( aPad, aParent )
{
    m_only_exact_connected = false; //Update all connected at pad
}

bool TEARDROPS::NET_SCAN_PAD_UPDATE::ExecuteAt( TRACK* aTrack )
{
    for( int n=0; n<2; ++n )
    {
        wxPoint pos;
        ( n )? pos = aTrack->GetEnd() : pos = aTrack->GetStart();

        TEARDROP* tear = nullptr;
        TRACKNODEITEM* item = m_Parent->Get( aTrack, pos );
        if( item && dynamic_cast<TEARDROP*>( item ) )
            tear = static_cast<TEARDROP*>( item );

        if( tear )
        {
            tear->Update();
            if( m_Parent->GetEditFrame() )
                m_Parent->GetEditFrame()->GetCanvas()->RefreshDrawingRect( tear->GetBoundingBox() );
        }
    }
    return false;
}

void TEARDROPS::Update( const D_PAD* aPadAt )
{
    if( aPadAt )
    {
        std::unique_ptr<NET_SCAN_PAD_UPDATE> pad( new NET_SCAN_PAD_UPDATE( aPadAt, this ) );
        if( pad )
            pad->Execute();
    }
}

void TEARDROPS::Update( const MODULE* aModuleAt )
{
    if( aModuleAt )
    {
        D_PAD* pad = aModuleAt->PadsList();
        while( pad )
        {
            Update( pad );
            pad = pad->Next();
        }
    }
}

TEARDROPS::NET_SCAN_NET_UPDATE::NET_SCAN_NET_UPDATE( const int aNet,
                                                     const TRACK* aStartTrack,
                                                     const TEARDROPS* aParent
                                                   ) :
    NET_SCAN_BASE( aStartTrack, aParent )

{
    m_Parent = const_cast<TEARDROPS*>( aParent );
    DLIST<TRACK>* tracks_list = &m_Parent->GetBoard()->m_Track;
    m_scan_start_track = const_cast<TRACK*>( aStartTrack );
    if( !aStartTrack )
    {
        TRACK* first_track = tracks_list->GetFirst();
        if( first_track )
            m_scan_start_track = first_track->GetStartNetCode( aNet );
    }
}

bool TEARDROPS::NET_SCAN_NET_UPDATE::ExecuteAt( TRACK* aTrack )
{
    if( dynamic_cast<TEARDROP*> ( const_cast<TRACK*>( aTrack ) ) )
        dynamic_cast<TEARDROP*> ( const_cast<TRACK*>( aTrack ) )->Update();
    return false;
}

void TEARDROPS::Update( const int aNetCodeAt, const TRACK* aStartTrack )
{
    std::unique_ptr<NET_SCAN_NET_UPDATE> net( new NET_SCAN_NET_UPDATE( aNetCodeAt, aStartTrack, this ) );
    if( net )
        net->Execute();
}

void TEARDROPS::Refresh( const TRACK* aTrackSegAt )
{
    Update( static_cast<TRACK*>( const_cast<TRACK*>( aTrackSegAt ) ) );
    if( m_EditFrame )
        m_EditFrame->GetCanvas()->Refresh();
}

TEARDROPS::NET_SCAN_TRACK_DRAW_UPDATE::NET_SCAN_TRACK_DRAW_UPDATE( const TRACK* aStartTrack,
                                                                   const TEARDROPS* aParent,
                                                                   const EDA_DRAW_PANEL* aPanel,
                                                                   const wxDC* aDC,
                                                                   const GR_DRAWMODE aDrawMode,
                                                                   const bool aErase
                                                                 ) :
    NET_SCAN_BASE( aStartTrack, aParent )
{
    m_panel = const_cast<EDA_DRAW_PANEL*>( aPanel );
    m_dc = const_cast<wxDC*>( aDC );
    m_draw_mode = aDrawMode;
    m_erase = aErase;
}

bool TEARDROPS::NET_SCAN_TRACK_DRAW_UPDATE::ExecuteAt( TRACK* aTrack )
{
    if( dynamic_cast<TEARDROP*>( const_cast<TRACK*>( aTrack ) ) )
        if( dynamic_cast<TEARDROP*>( const_cast<TRACK*>( aTrack ) )->GetTrackSeg() == m_scan_start_track )
        {
            if( m_erase )
                dynamic_cast<TEARDROP*>( const_cast<TRACK*>( aTrack ) )->Draw( m_panel, m_dc, m_draw_mode );
            dynamic_cast<TEARDROP*>( const_cast<TRACK*>( aTrack ) )->Update();
            dynamic_cast<TEARDROP*>( const_cast<TRACK*>( aTrack ) )->Draw( m_panel, m_dc, m_draw_mode );
        }
    return false;
}

void TEARDROPS::Update( TRACK* aTrackSegAt,
                        EDA_DRAW_PANEL* aPanel,
                        wxDC* aDC,
                        GR_DRAWMODE aDrawMode,
                        bool aErase
                      )
{
    if( aTrackSegAt )
    {
        if( aTrackSegAt->Type() == PCB_TRACE_T )
        {
            std::unique_ptr<NET_SCAN_TRACK_DRAW_UPDATE> tear( new NET_SCAN_TRACK_DRAW_UPDATE( aTrackSegAt,
                                                                                              this,
                                                                                              aPanel,
                                                                                              aDC,
                                                                                              aDrawMode,
                                                                                              aErase ) );
            if( tear )
                tear->Execute();
        }
    }
}

void TEARDROPS::Update( D_PAD* aPadAt,
                        EDA_DRAW_PANEL* aPanel,
                        wxDC* aDC,
                        GR_DRAWMODE aDrawMode,
                        bool aErase
                      )
{
#ifdef NEWCONALGO
    auto connity = m_Parent->GetBoard()->GetConnectivity();
    auto tracks = connity->GetConnectedTracks( aPadAt );
    for( auto track : tracks )
        Update( track, aPanel, aDC, aDrawMode, aErase );
#else
    if( aPadAt )
        for( unsigned int n = 0; n < aPadAt->m_TracksConnected.size(); ++n )
            Update( static_cast<TRACK*>( aPadAt->m_TracksConnected.at( n ) ),
                    aPanel,
                    aDC,
                    aDrawMode,
                    aErase );
#endif
}

void TEARDROPS::Update( MODULE* aModuleAt,
                        EDA_DRAW_PANEL* aPanel,
                        wxDC* aDC,
                        GR_DRAWMODE aDrawMode,
                        bool aErase
                      )
{
    if( aModuleAt )
    {
        D_PAD* pad = aModuleAt->PadsList();
        while( pad )
        {
            Update( pad, aPanel, aDC, aDrawMode, aErase );
            pad = pad->Next();
        }
    }
}

TEARDROP::PARAMS TEARDROPS::SetParamLimits( const TEARDROP::PARAMS aShapeParams )
{
    TEARDROP::PARAMS ret_param = aShapeParams;
    if( ret_param.length_ratio < TEARDROP::TEARDROPS_POS_MIN )
        ret_param.length_ratio = TEARDROP::TEARDROPS_POS_MIN;
    if( ret_param.width_ratio < TEARDROP::TEARDROPS_SIZE_MIN )
        ret_param.width_ratio = TEARDROP::TEARDROPS_SIZE_MIN;
    if( ret_param.width_ratio > TEARDROP::TEARDROPS_WIDTH_MAX )
        ret_param.width_ratio = TEARDROP::TEARDROPS_WIDTH_MAX;
    switch( ret_param.shape )
    {
        case TEARDROP::TEARDROP_T:
            if( ret_param.num_segments > TEARDROP::TEARDROP_SEGS_MAX )
                ret_param.num_segments =  TEARDROP::TEARDROP_SEGS_MAX;
            if( ret_param.num_segments < TEARDROP::TEARDROP_SEGS_MIN )
                ret_param.num_segments =  TEARDROP::TEARDROP_SEGS_MIN;
            if( ret_param.length_ratio > TEARDROP::TEARDROPS_LENGTH_MAX )
                ret_param.length_ratio = TEARDROP::TEARDROPS_LENGTH_MAX;
            break;
        case TEARDROP::SUBLAND_T:
            if( ret_param.length_ratio > TEARDROP::SUBLAND_POS_MAX )
                ret_param.length_ratio = TEARDROP::SUBLAND_POS_MAX;
            if( ret_param.width_ratio > TEARDROP::SUBLAND_SIZE_MAX )
                ret_param.width_ratio = TEARDROP::SUBLAND_SIZE_MAX;
            break;
        case TEARDROP::FILLET_T:
            break;
        case TEARDROP::ZERO_T:
            ret_param.length_ratio = 0;
            ret_param.width_ratio = 0;
            ret_param.num_segments = 0;
            break;
        default:;
    }
    return ret_param;
}

void TEARDROPS::SetShapeParams( const TEARDROP::PARAMS aShapeParams )
{
    if( aShapeParams.shape != TEARDROP::NULL_T )
    {
        TEARDROP::PARAMS save_param = SetParamLimits( aShapeParams );
        switch( save_param.shape )
        {
            case TEARDROP::TEARDROP_T:
                m_teardrop_params = save_param;
                break;
            case TEARDROP::SUBLAND_T:
                m_subland_params = save_param;
                break;
            case TEARDROP::FILLET_T:
                m_fillet_params = save_param;
                break;
            case TEARDROP::ZERO_T:
                m_zero_params = save_param;
                break;
            default:;
        }
    }
}

TEARDROP::PARAMS TEARDROPS::GetShapeParams( const TEARDROP::SHAPES_T aShape ) const
{
    TEARDROP::PARAMS params = {TEARDROP::NULL_T, 0, 0, 0};
    switch( aShape )
    {
        case TEARDROP::SUBLAND_T:
            params = m_subland_params;
            break;
        case TEARDROP::FILLET_T:
            params = m_fillet_params;
            break;
        case TEARDROP::TEARDROP_T:
            params = m_teardrop_params;
            break;
        case TEARDROP::ZERO_T:
            params = m_zero_params;
            break;
        default:;
    }
    return params;
}

TEARDROP::PARAMS TEARDROPS::GetDefaultParams( const TEARDROP::SHAPES_T aShape ) const
{
    TEARDROP::PARAMS params = {TEARDROP::NULL_T, 0, 0, 0};
    switch( aShape )
    {
        case TEARDROP::TEARDROP_T:
            params = {TEARDROP::TEARDROP_T, 100, 100, 4};
            break;
        case TEARDROP::FILLET_T:
            params = {TEARDROP::FILLET_T, 100, 100, 1};
            break;
        case TEARDROP::SUBLAND_T:
            params = {TEARDROP::SUBLAND_T, 100, 75, 0};
            break;
        case TEARDROP::ZERO_T:
            params = {TEARDROP::ZERO_T, 0, 0, 0};
            break;
        default:;
    }
    return params;
}

void TEARDROPS::LoadDefaultParams( const TEARDROP::SHAPES_T aShape )
{
    switch( aShape )
    {
        case TEARDROP::TEARDROP_T:
            m_teardrop_params = GetDefaultParams( aShape );
            break;
        case TEARDROP::FILLET_T:
            m_fillet_params = GetDefaultParams( aShape );
            break;
        case TEARDROP::SUBLAND_T:
            m_subland_params = GetDefaultParams( aShape );
            break;
        case TEARDROP::ZERO_T:
            m_zero_params = GetDefaultParams( aShape );
            break;
        default:;
    }
    RecreateMenu();
}

wxString TEARDROPS::GetShapeName( const TEARDROP::SHAPES_T aShape ) const
{
    switch( aShape )
    {
        case TEARDROP::TEARDROP_T:
            return _( "TEARDROP" );
        case TEARDROP::FILLET_T:
            return _( "FILLET" );
        case TEARDROP::SUBLAND_T:
            return _( "SUBLAND" );
        case TEARDROP::ZERO_T:
            return _( "ZERO" );
        default:
            return _( "NULL" );
    }
}

void TEARDROPS::SelectShape( const TEARDROP::SHAPES_T aShape )
{
    if( ( aShape != m_current_shape ) )
    {
        switch( aShape )
        {
            case TEARDROP::TEARDROP_T:
            case TEARDROP::FILLET_T:
            case TEARDROP::SUBLAND_T:
            case TEARDROP::ZERO_T:
                m_current_shape = aShape;
                break;
            default:;
        }
    }
    else
    {
        m_current_shape = TEARDROP::NULL_T;
    }
    if( m_EditFrame )
        m_EditFrame->GetCanvas()->Refresh();

    RecreateMenu();
}

void TEARDROPS::SetCurrentShape( const TEARDROP::SHAPES_T aShape )
{
    switch( aShape )
    {
        case TEARDROP::TEARDROP_T:
        case TEARDROP::FILLET_T:
        case TEARDROP::SUBLAND_T:
        case TEARDROP::ZERO_T:
            m_current_shape = aShape;
            break;
        default:
            m_current_shape = TEARDROP::NULL_T;
    }
}

TEARDROP::PARAMS TEARDROPS::CopyCurrentParams( const TRACK* aTrackSegAt,
                                               const wxPoint& aCurPosAt
                                             )
{
    TEARDROP::PARAMS tear_params = {TEARDROP::NULL_T, 0, 0, 0};
    wxPoint track_pos = TrackSegNearestEndpoint( aTrackSegAt, aCurPosAt );

    TEARDROP* tear = nullptr;
    TRACKNODEITEM* item = Get( aTrackSegAt, track_pos );
    if( item && dynamic_cast<TEARDROP*>( item ) )
        tear = static_cast<TEARDROP*>( item );

    if( tear )
    {
        tear_params = tear->GetParams();
        SetShapeParams( tear_params );
        SetCurrentShape( tear_params.shape );
        RecreateMenu();
    }
    return tear_params;
}

TEARDROPS::NET_SCAN_GET_NEXT_TEARDROP::NET_SCAN_GET_NEXT_TEARDROP( const TRACK* aStartTrack,
                                                                   const TEARDROPS* aParent
                                                                 ) :
    NET_SCAN_GET_TEARDROP( aStartTrack, nullptr, aParent )
{
}

bool TEARDROPS::NET_SCAN_GET_NEXT_TEARDROP::ExecuteAt( TRACK* aTrack )
{
    if( dynamic_cast<TEARDROP*>( const_cast<TRACK*>( aTrack ) ) )
        if( dynamic_cast<TEARDROP*>( const_cast<TRACK*>( aTrack ) )->GetTrackSeg() == m_scan_start_track )
            if( dynamic_cast<TEARDROP*>( const_cast<TRACK*>( aTrack ) )->IsTrackEndpointEnd() )
            {
                m_result_teardrop = static_cast<TEARDROP*>( const_cast<TRACK*>( aTrack ) );
                return true;
            }

    return false;
}

TRACKNODEITEM* TEARDROPS::Next( const TRACK* aTrackSegAt ) const
{
    TEARDROP* result_tear = nullptr;
    if( aTrackSegAt && ( aTrackSegAt->Type() == PCB_TRACE_T ) )
    {
        std::unique_ptr<NET_SCAN_GET_NEXT_TEARDROP> tear( new NET_SCAN_GET_NEXT_TEARDROP( aTrackSegAt, this ) );
        if( tear )
        {
            tear->Execute();
            result_tear = tear->GetResult();
        }
    }
    return result_tear;
}

TEARDROPS::NET_SCAN_GET_BACK_TEARDROP::NET_SCAN_GET_BACK_TEARDROP( const TRACK* aStartTrack,
                                                                   const TEARDROPS* aParent
                                                                 ) :
    NET_SCAN_GET_TEARDROP( aStartTrack, nullptr, aParent )
{
    m_reverse = true;
}

bool TEARDROPS::NET_SCAN_GET_BACK_TEARDROP::ExecuteAt( TRACK* aTrack )
{
    if( dynamic_cast<TEARDROP*>( const_cast<TRACK*>( aTrack ) ) )
        if( dynamic_cast<TEARDROP*>( const_cast<TRACK*>( aTrack ) )->GetTrackSeg() == m_scan_start_track )
            if( dynamic_cast<TEARDROP*>( const_cast<TRACK*>( aTrack ) )->IsTrackEndpointStart() )
            {
                m_result_teardrop = static_cast<TEARDROP*>( const_cast<TRACK*>( aTrack ) );
                return true;
            }

    return false;
}

TRACKNODEITEM* TEARDROPS::Back( const TRACK* aTrackSegAt ) const
{
    TEARDROP* result_tear = nullptr;
    if( aTrackSegAt && ( aTrackSegAt->Type() == PCB_TRACE_T ) )
    {
        std::unique_ptr<NET_SCAN_GET_BACK_TEARDROP> tear( new NET_SCAN_GET_BACK_TEARDROP( aTrackSegAt, this ) );
        if( tear )
        {
            tear->Execute();
            result_tear = tear->GetResult();
        }
    }
    return result_tear;
}

TEARDROPS::NET_SCAN_GET_TEARDROP::NET_SCAN_GET_TEARDROP( const TRACK* aStartTrack,
                                                         const BOARD_CONNECTED_ITEM* aViaOrPad,
                                                         const TEARDROPS* aParent
                                                       ) :
    NET_SCAN_BASE( aStartTrack, aParent )
{
    m_via_or_pad = const_cast<BOARD_CONNECTED_ITEM*>( aViaOrPad );
    m_result_teardrop = nullptr;
}

bool TEARDROPS::NET_SCAN_GET_TEARDROP::ExecuteAt( TRACK* aTrack )
{
    if( aTrack->Type() == PCB_TEARDROP_T )
    {
        if( dynamic_cast<TEARDROP*>( const_cast<TRACK*>( aTrack ) )->GetTrackSeg() == m_scan_start_track )
        {
            if( ( m_via_or_pad->Type() == PCB_VIA_T ) &&
                ( dynamic_cast<TEARDROP_VIA*>( const_cast<TRACK*>( aTrack ) ) ) )
            {
                VIA* connected_via = dynamic_cast<TEARDROP_VIA*>( const_cast<TRACK*>( aTrack ) )->GetConnectedVia();
                if( connected_via == static_cast<VIA*>( m_via_or_pad ) )
                {
                    m_result_teardrop = static_cast<TEARDROP*>( const_cast<TRACK*>( aTrack ) );
                    return true;
                }
            }

            if( ( m_via_or_pad->Type() == PCB_PAD_T ) &&
                ( dynamic_cast<TEARDROP_PAD*>( const_cast<TRACK*>( aTrack ) ) ) )
            {
                D_PAD* connected_pad = dynamic_cast<TEARDROP_PAD*>( const_cast<TRACK*>( aTrack ) )->GetConnectedPad();
                if( connected_pad == static_cast<D_PAD*>( m_via_or_pad ) )
                {
                    m_result_teardrop = static_cast<TEARDROP*>( const_cast<TRACK*>( aTrack ) );
                    return true;
                }
            }
        }
    }
    return false;
}

TEARDROP* TEARDROPS::GetTeardrop( const TRACK* aTrackSegAt,
                                  const BOARD_CONNECTED_ITEM* atViaOrPad
                                ) const
{
    TEARDROP* result_tear = nullptr;
    if( aTrackSegAt && atViaOrPad )
    {
        if( aTrackSegAt->Type() == PCB_TRACE_T )
        {
            std::unique_ptr<NET_SCAN_GET_TEARDROP> tear( new NET_SCAN_GET_TEARDROP( aTrackSegAt,
                                                                                    atViaOrPad,
                                                                                    this ) );
            if( tear )
            {
                tear->Execute();
                result_tear = tear->GetResult();
            }
        }
    }
    return result_tear;
}

void TEARDROPS::ChangePad( TEARDROP* atTeardrop, const MODULE* inModule )
{
    if( inModule )
    {
        D_PAD* pad = inModule->PadsList();
        while( pad )
        {
            if( ( atTeardrop->GetEnd() == pad->GetPosition() ) &&
                pad->IsOnLayer( atTeardrop->GetLayer() ) )
            {
                dynamic_cast<TEARDROP_PAD*>( atTeardrop )->ChangePad( pad );
            }
            pad = pad->Next();
        }
    }
}

