/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2016 Heikki Pulkkinen.
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
 * @file viastitching.h
 * @brief Definitions for Via Stitching.
 */

#ifndef VIASTITCHING_H
#define VIASTITCHING_H

#ifndef NEWCONALGO
#include <connect.h>
#include <pcbnew_id.h>
#endif

#include <drc_stuff.h>
#include <ratsnest_data.h>
#include <router/router_tool.h>
#include <board_commit.h>

#include <tool/tool_manager.h>
#include <view/view.h>
#include <class_drawpanel.h>
#include <wx/event.h>
#include <class_zone.h>
#include <class_track.h>
#include <class_board.h>
#include <wxPcbStruct.h>
#include <tool/tool_event.h>

//Layer selector
#include <fctsys.h>
#include <common.h>
#include <class_drawpanel.h>
#include <confirm.h>
#include <wxBasePcbFrame.h>
#include <class_layer_box_selector.h>
#include <class_board.h>
#include <dialogs/dialog_layer_selection_base.h>

class VIASTITCHING
{
public:
    VIASTITCHING(const BOARD* aBoard);
    ~VIASTITCHING();

    void SetNetcodes( const std::unordered_map<const VIA*, int>& aVias );
    void SetNetcodes( void );

    void AddThermalVia( const PCB_EDIT_FRAME* aEditFrame, const int aViaType_ID );//Legacy
    void AddThermalVia( const PCB_EDIT_FRAME* aEditFrame, const VIATYPE_T aViaType, const bool aSelectLayer ); //GAL

    void AddViaArrayPrepare( const PCB_EDIT_FRAME* aEditFrame, const VIA* aVia );
    void AddViaArrayFinnish( void );

    void RuleCheck( const TRACK* aTrack, DRC* aDRC );
    
    bool DestroyConflicts( BOARD_ITEM* aItem, PCB_BASE_FRAME* aFrame );
    bool Clean( PCB_EDIT_FRAME* aEditFrame, BOARD_COMMIT* aCommit );

    bool SelectLayer( PCB_EDIT_FRAME* aEditFrame, const wxPoint aPos );
    bool SelectLayerPair( PCB_EDIT_FRAME* aEditFrame, const wxPoint aPos );
    
    void ConnectToZones( void );
    
protected:
    VIASTITCHING(){};
    
private:
    const BOARD* m_board;
    
    //VIATYPE_T m_viatype{VIA_THROUGH};
    int m_via_array_netcode{0};
    std::vector<D_PAD*> m_via_array_netcode_pads{nullptr};

    void Collect_Vias_Zones_Chain( std::unordered_multimap<const VIA*, const SHAPE_POLY_SET::POLYGON*>& aViasPolys,
                                const int aNetCode,
                                const SHAPE_POLY_SET::POLYGON* aZonePoly,
                                std::unordered_map<const VIA*, bool>& aViasAll );

    void Collect_Zones_Hit_Via( std::vector<ZONE_CONTAINER*>& aZones,
                                const VIA* aVia,
                                const int aNetCode,
                                const LAYER_NUM aLayerOnly =  UNDEFINED_LAYER );

    void Collect_Zones_Hit_Pos( std::vector<ZONE_CONTAINER*>& aZones,
                                wxPoint aPos,
                                PCB_LAYER_ID aTopLayer,
                                PCB_LAYER_ID aBottomLayer,
                                const int aNetCode,
                                const LAYER_NUM aLayerOnly = UNDEFINED_LAYER );

    bool IsTrackConnection( const VIA* aVia, const int aNetcode );

    TRACK* ViaBreakTrack( const TRACK* aStartingTrack, const VIA* aVia );

//-----------------------------------------------------------------------------------------------------/
// Show wia in via tool mode.
//-----------------------------------------------------------------------------------------------------/
public:
    void StartDrawingVia(const PCB_EDIT_FRAME* aEditFrame, const EDA_DRAW_PANEL* aPanel, wxDC* aDC);
    
struct VIA_SETTINGS
{
    COLOR4D color;
    int rad{0};
    int hole_rad{0};
    int clearance_rad{0};
    
    wxString text{""};
};
    VIA_SETTINGS GetDrawViaSettings(void) const {return m_current_settings;}
    void SetDrawViaSettings(const VIA_SETTINGS aSettings){m_current_settings = aSettings;}
    
    wxPoint GetDrawViaPrevPos(void) const { return m_via_prev_pos; }
    void SetDrawViaPrevPos(wxPoint aPos) { m_via_prev_pos = aPos; }
    
private:
    PCB_EDIT_FRAME* m_EditFrame; 
    EDA_DRAW_PANEL* m_draw_panel;
    wxDC* m_dc;
    
    wxPoint m_via_prev_pos{0,0};
    VIA_SETTINGS m_current_settings;
//-----------------------------------------------------------------------------------------------------/

}; //VIASTITCHING


namespace ViaStitching
{
//-----------------------------------------------------------------------------------------------------/
// Show wia in via tool mode mouse capture.
//-----------------------------------------------------------------------------------------------------/
void DrawMovingVia(EDA_DRAW_PANEL* aPanel, wxDC* aDC, const wxPoint& aPosition, bool aErase);
void StopDrawingVia(EDA_DRAW_PANEL* aPanel, wxDC* aDC);

void DrawViaText(EDA_DRAW_PANEL* aPanel, wxDC* aDC, const wxPoint& aPosition, COLOR4D aColor, const wxString& aText, const int aViaSize);
void DrawViaCircles(EDA_DRAW_PANEL* aPanel, wxDC* aDC, const wxPoint& aPosition, COLOR4D aColor, const int aViaRad, const int aViaHoleRad, const int aViaClearanceRad);
VIASTITCHING::VIA_SETTINGS GetCurrentViaSettings(const PCB_EDIT_FRAME* aEditFrame);

//-----------------------------------------------------------------------------------------------------/

const int MIN_THERMALVIA_ZONES = 1;

ZONE_CONTAINER* HitTestZone( const BOARD* aPcb, const wxPoint aPos, PCB_LAYER_ID aLayer );

//-----------------------------------------------------------------------------------------------------/
//Copy from sel_layer.cpp, because there where no header of this. And I do not want to disturb old code.
//-----------------------------------------------------------------------------------------------------/
#define SELECT_COLNUM       0
#define COLOR_COLNUM        1
#define LAYERNAME_COLNUM    2

class VS_LAYER_SELECTOR: public LAYER_SELECTOR
{
public:
    VS_LAYER_SELECTOR( BOARD* aBrd ) :
        LAYER_SELECTOR() {
        m_brd = aBrd;
    }

protected:
    BOARD*  m_brd;

    bool IsLayerEnabled( LAYER_NUM aLayer ) const override {
        return m_brd->IsLayerEnabled( PCB_LAYER_ID( aLayer ) );
    }

    COLOR4D GetLayerColor( LAYER_NUM aLayer ) const override {
#ifdef NEWCONALGO
        return m_brd->Colors().GetLayerColor( ToLAYER_ID( aLayer ) );
#else
        return m_brd->GetLayerColor( ToLAYER_ID( aLayer ) );
#endif
    }

    wxString GetLayerName( LAYER_NUM aLayer ) const override {
        return m_brd->GetLayerName( ToLAYER_ID( aLayer ) );
    }
};

// Through via selection
class THROUGH_VIA_LAYER_SELECTOR : public VS_LAYER_SELECTOR, public DIALOG_LAYER_SELECTION_BASE
{
    PCB_LAYER_ID    m_layerSelected;
    LSET        m_notAllowedLayersMask;

    std::vector<PCB_LAYER_ID> m_layersIdLeftColumn;

public:
    THROUGH_VIA_LAYER_SELECTOR( PCB_EDIT_FRAME* aEditFrame, const wxPoint aPos);

    LAYER_NUM GetLayerSelection() { return m_layerSelected; }

protected:
    void OnLeftGridCellClick( wxGridEvent& event ) override;
    void OnLeftButtonReleased( wxMouseEvent& event ) override; //Does not work.wxBroblem?
    
private:
    void buildList();

    wxPoint m_pos{0,0};
    PCB_EDIT_FRAME* m_frame{nullptr};
};

// Blind / Buried via selection.
class BURIEDBLIND_VIA_LAYER_SELECTOR : public VS_LAYER_SELECTOR, public DIALOG_COPPER_LAYER_PAIR_SELECTION_BASE
{
private:
    PCB_LAYER_ID m_frontLayer;
    PCB_LAYER_ID m_backLayer;
    int m_leftRowSelected;
    int m_rightRowSelected;

    std::vector<PCB_LAYER_ID> m_left_layersId;
    std::vector<PCB_LAYER_ID> m_right_layersId;

public:
    BURIEDBLIND_VIA_LAYER_SELECTOR( PCB_EDIT_FRAME* aEditFrame, const wxPoint aPos );

    void GetLayerPair( PCB_LAYER_ID& aFrontLayer, PCB_LAYER_ID& aBackLayer )
    {
        aFrontLayer = m_frontLayer;
        aBackLayer = m_backLayer;
    }

protected:
    void OnLeftGridCellClick( wxGridEvent& event ) override;
    void OnRightGridCellClick( wxGridEvent& event ) override;

    void OnOkClick( wxCommandEvent& event );
    void OnCancelClick( wxCommandEvent& event ) override;

private:
    void buildList();
    void SetGridCursor( wxGrid* aGrid, int aRow, bool aEnable );

    wxPoint m_pos{0,0};
    PCB_EDIT_FRAME* m_frame{nullptr};
};


} //namespace Via_Stitching

#endif //VIASTITCHING_H

