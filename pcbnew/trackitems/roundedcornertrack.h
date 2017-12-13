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

/**
 * @file roundedcornertrack.h
 * @brief Definitions for rounded corner track.
 */

#ifndef ROUNDED_CORNER_TRACK_H
#define ROUNDED_CORNER_TRACK_H

#include <class_track.h>
#include <class_board.h>
#include "roundedtrackscorner.h"


class ROUNDED_CORNER_TRACK : public TRACK
{
public:
    ROUNDED_CORNER_TRACK( const ROUNDED_CORNER_TRACK& aTrack );
    ROUNDED_CORNER_TRACK( const BOARD_ITEM* aParent );
    ROUNDED_CORNER_TRACK( const BOARD_ITEM* aParent, const TRACK* aTrack );
    ~ROUNDED_CORNER_TRACK(){};

    EDA_ITEM* Clone() const override
    {
        return new ROUNDED_CORNER_TRACK( *this );
    }

    double GetLengthVisible() const
    {
        return GetLineLength( m_StartVisible, m_EndVisible );
    }

    void ResetVisibleEndpoints( void );
    const wxPoint& GetStartVisible( void ) const { return m_StartVisible; }
    const wxPoint& GetEndVisible( void ) const { return m_EndVisible; }

    TrackNodeItem::ROUNDED_TRACKS_CORNER* Contains( const wxPoint& aPos ) const;
    TrackNodeItem::ROUNDED_TRACKS_CORNER* GetStartPointCorner( void ) const { return m_StartPointCorner; }
    TrackNodeItem::ROUNDED_TRACKS_CORNER* GetEndPointCorner( void ) const { return m_EndPointCorner; }

    void TransformShapeWithClearanceToPolygon( SHAPE_POLY_SET& aCornerBuffer,
                                               int aClearanceValue,
                                               int aCircleToSegmentsCount,
                                               double aCorrectionFactor
                                             ) const override; //override TRACK.

    void Draw( EDA_DRAW_PANEL* aPanel,
               wxDC* DC,
               GR_DRAWMODE aDrawMode,
               const wxPoint& aOffset = ZeroOffset
             ) override;

private:
    friend class TrackNodeItem::ROUNDED_TRACKS_CORNER; //Only, who can set values.

    wxPoint m_StartVisible {0,0};
    wxPoint m_EndVisible {0,0};

    TrackNodeItem::ROUNDED_TRACKS_CORNER* m_StartPointCorner {nullptr};
    TrackNodeItem::ROUNDED_TRACKS_CORNER* m_EndPointCorner {nullptr};
};

#endif //ROUNDED_CORNER_TRACK_H
