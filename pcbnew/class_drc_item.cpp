/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2007 Dick Hollenbeck, dick@softplc.com
 * Copyright (C) 2015 KiCad Developers, see change_log.txt for contributors.
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


/*************************************************/
/* class_drc_item.cpp - DRC_ITEM class functions */
/*************************************************/
#include <fctsys.h>
#include <common.h>

#include <pcbnew.h>
#include <drc_stuff.h>
#include <class_drc_item.h>
#include <base_units.h>

#ifdef PCBNEW_WITH_TRACKITEMS
#include "trackitems/teardrops.h"
#endif


wxString DRC_ITEM::GetErrorText() const
{
    switch( m_ErrorCode )
    {
    case DRCE_UNCONNECTED_PADS:
        return wxString( _( "Unconnected pads" ) );
    case DRCE_TRACK_NEAR_THROUGH_HOLE:
        return wxString( _( "Track near thru-hole" ) );
    case DRCE_TRACK_NEAR_PAD:
        return wxString( _( "Track near pad" ) );
    case DRCE_TRACK_NEAR_VIA:
        return wxString( _( "Track near via" ) );
    case DRCE_VIA_NEAR_VIA:
        return wxString( _( "Via near via" ) );
    case DRCE_VIA_NEAR_TRACK:
        return wxString( _( "Via near track" ) );
    case DRCE_TRACK_ENDS1:
    case DRCE_TRACK_ENDS2:
    case DRCE_TRACK_ENDS3:
    case DRCE_TRACK_ENDS4:
    case DRCE_ENDS_PROBLEM1:
    case DRCE_ENDS_PROBLEM2:
    case DRCE_ENDS_PROBLEM3:
    case DRCE_ENDS_PROBLEM4:
    case DRCE_ENDS_PROBLEM5:
        return wxString( _( "Two track ends too close" ) );
    case DRCE_TRACK_SEGMENTS_TOO_CLOSE:
        return wxString( _( "Two parallel track segments too close" ) );
    case DRCE_TRACKS_CROSSING:
        return wxString( _( "Tracks crossing" ) );
    case DRCE_PAD_NEAR_PAD1:
        return wxString( _( "Pad near pad" ) );
    case DRCE_VIA_HOLE_BIGGER:
        return wxString( _( "Via hole > diameter" ) );
    case DRCE_MICRO_VIA_INCORRECT_LAYER_PAIR:
        return wxString( _( "Micro Via: incorrect layer pairs (not adjacent)" ) );
    case COPPERAREA_INSIDE_COPPERAREA:
        return wxString( _( "Copper area inside copper area" ) );
    case COPPERAREA_CLOSE_TO_COPPERAREA:
        return wxString( _( "Copper areas intersect or are too close" ) );

    case DRCE_SUSPICIOUS_NET_FOR_ZONE_OUTLINE:
        return wxString( _( "Copper area belongs a net which has no pads. This is strange" ) );

    case DRCE_HOLE_NEAR_PAD:
        return wxString( _( "Hole near pad" ) );
    case DRCE_HOLE_NEAR_TRACK:
        return wxString( _( "Hole near track" ) );
    case DRCE_TOO_SMALL_TRACK_WIDTH:
        return wxString( _( "Too small track width" ) );
    case DRCE_TOO_SMALL_VIA:
        return wxString( _( "Too small via size" ) );
    case DRCE_TOO_SMALL_MICROVIA:
        return wxString( _( "Too small micro via size" ) );
    case DRCE_TOO_SMALL_VIA_DRILL:
        return wxString( _( "Too small via drill" ) );
    case DRCE_TOO_SMALL_MICROVIA_DRILL:
        return wxString( _( "Too small micro via drill" ) );

    // use &lt; since this is text ultimately embedded in HTML
    case DRCE_NETCLASS_TRACKWIDTH:
        return wxString( _( "NetClass Track Width &lt; global limit" ) );
    case DRCE_NETCLASS_CLEARANCE:
        return wxString( _( "NetClass Clearance &lt; global limit" ) );
    case DRCE_NETCLASS_VIASIZE:
        return wxString( _( "NetClass Via Dia &lt; global limit" ) );
    case DRCE_NETCLASS_VIADRILLSIZE:
        return wxString( _( "NetClass Via Drill &lt; global limit" ) );
    case DRCE_NETCLASS_uVIASIZE:
        return wxString( _( "NetClass uVia Dia &lt; global limit" ) );
    case DRCE_NETCLASS_uVIADRILLSIZE:
        return wxString( _( "NetClass uVia Drill &lt; global limit" ) );

    case DRCE_VIA_INSIDE_KEEPOUT:
        return wxString( _( "Via inside a keepout area" ) );

    case DRCE_TRACK_INSIDE_KEEPOUT:
        return wxString( _( "Track inside a keepout area" ) );

    case DRCE_PAD_INSIDE_KEEPOUT:
        return wxString( _( "Pad inside a keepout area" ) );

    case DRCE_VIA_INSIDE_TEXT:
        return wxString( _( "Via inside a text" ) );

    case DRCE_TRACK_INSIDE_TEXT:
        return wxString( _( "Track inside a text" ) );

    case DRCE_PAD_INSIDE_TEXT:
        return wxString( _( "Pad inside a text" ) );

    case DRCE_OVERLAPPING_FOOTPRINTS:
        return wxString( _( "Courtyards overlap" ) );

    case DRCE_MISSING_COURTYARD_IN_FOOTPRINT:
        return wxString( _( "Footprint has no courtyard defined" ) );

    case DRCE_MALFORMED_COURTYARD_IN_FOOTPRINT:
        return wxString( _( "Footprint has incorrect courtyard (not a closed shape)" ) );

#ifdef PCBNEW_WITH_TRACKITEMS
    //Track node items
    case DRCE_TRACKNODEITEM_UNSPECIFIED:
        return wxString(_("Unspecified last action"));
        
    //Teardrops
    case DRCE_THERMAL_VIA_UNCONNECTED:
        return wxString( _( "Thermal Via unconnected" ) );
    case DRCE_THERMAL_VIA_CONNECTED_POURS:
        return wxString( _( "Thermal Via Connected Pours < 2" ) );
    case DRCE_TEARDROP_NEAR_TEARDROP:
        return wxString(_("Teardrop near Teardrop"));
    case DRCE_TEARDROP_NEAR_TRACK:
        return wxString(_("Teardrop near Track"));
    case DRCE_TEARDROP_NEAR_VIA:
        return wxString(_("Teardrop near Via"));
    case DRCE_TEARDROP_NEAR_PAD:
        return wxString(_("Teardrop near Pad"));
    case DRCE_TEARDROP_TOO_SMALL:
        return wxString(_("Teardrop too small"));
    case DRCE_JUNCTION_NEAR_TRACK:
        return wxString(_("Junction / T-Junction near Track"));
    case DRCE_JUNCTION_NEAR_VIA:
        return wxString(_("Junction / T-Junction near Via"));
    case DRCE_JUNCTION_NEAR_PAD:
        return wxString(_("Junction / T-Junction near Pad"));
    case DRCE_JUNCTION_NEAR_TEARDROP:
        return wxString(_("Junction / T-Junction near Teardrop"));
    case DRCE_JUNCTION_NEAR_JUNCTION:
        return wxString(_("Junction / T-Junction near Junction / T-Junction"));
    case DRCE_JUNCTION_TOO_SMALL:
        return wxString(_("Junction / T-Junction too small"));
    case DRCE_TJUNCTION_TRACK_SEGS_LENGTH:
        return wxString(_("T-Junction Length of Track Segments"));
    case DRCE_TEARDROP_INSIDE_TEXT:
        return wxString(_("Teardrop inside a text"));
    case DRCE_JUNCTION_INSIDE_TEXT:
        return wxString(_("Junction / T-Junction inside a text"));
    case DRCE_TEARDROP_TRIMMED:
        return wxString(_("Teardrop trimmed"));
    case DRCE_TEARDROP_BIG:
        return wxString(_("Teardrop too big"));
    case DRCE_TEARDROP_MISSING:
        return wxString(_("Teardrop Missing"));
    case DRCE_TJUNCTION_MISSING:
        return wxString(_("T-Junction Missing"));
    case DRCE_JUNCTION_MISSING:
        return wxString(_("Junction Missing"));
    case DRCE_TEARDROP_NOT_IN_PAD:
        return wxString(_("Teardrop not in Pad"));
    case DRCE_TEARDROP_NOT_IN_VIA:
        return wxString(_("Teardrop not in Via"));
    case DRCE_TEARDOPS_MULTIPLE:
        return wxString(_("Teardrops multiple"));
    case DRCE_TEARDROP_TRACK_ERROR:
        return wxString(_("Teardrop Track error"));
    case DRCE_TEARDROP_USELESS:
        return wxString(_("Teardrop may be useless"));
    case DRCE_TEARDROP_IMPOSSIBLE:
        return wxString(_("Teardrop impossible"));
    case DRCE_JUNCTION_TRACK_SEGS_WIDTH:
        return wxString(_("Junction / T-Junction Width of Track Segments"));
    case DRCE_TJUNCTION_ANGLE:
        return wxString(_("T-Junction angle"));
    case DRCE_TEARDROPS_INSIDE:
        return wxString(_("Teardrops inside"));
#endif
        
    default:
        return wxString::Format( wxT( "Unknown DRC error code %d" ), m_ErrorCode );
    }
}


wxString DRC_ITEM::ShowCoord( const wxPoint& aPos )
{
    wxString ret;

    ret << aPos;
    return ret;
}
