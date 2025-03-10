
/* Do not modify this file it was automatically generated by the
 * TokenList2DsnLexer CMake script.
 *
 * Include this file in your lexer class to provide the keywords for
 * your DSN lexer.
 */

#include </opt/kicad/program/master/Tears_Master/build/common/pcb_lexer.h>

using namespace PCB_KEYS_T;

#define TOKDEF(x)    { #x, T_##x }

const KEYWORD PCB_LEXER::keywords[] = {
    TOKDEF( add_net ),
    TOKDEF( addsublayer ),
    TOKDEF( aligned ),
    TOKDEF( allow_missing_courtyard ),
    TOKDEF( allow_soldermask_bridges ),
    TOKDEF( allow_soldermask_bridges_in_footprints ),
    TOKDEF( allow_two_segments ),
    TOKDEF( allowed ),
    TOKDEF( anchor ),
    TOKDEF( angle ),
    TOKDEF( arc ),
    TOKDEF( arc_segments ),
    TOKDEF( area ),
    TOKDEF( arrow1a ),
    TOKDEF( arrow1b ),
    TOKDEF( arrow2a ),
    TOKDEF( arrow2b ),
    TOKDEF( arrow_direction ),
    TOKDEF( arrow_length ),
    TOKDEF( at ),
    TOKDEF( attr ),
    TOKDEF( autoplace_cost180 ),
    TOKDEF( autoplace_cost90 ),
    TOKDEF( aux_axis_origin ),
    TOKDEF( back ),
    TOKDEF( best_length_ratio ),
    TOKDEF( best_width_ratio ),
    TOKDEF( bevelled ),
    TOKDEF( blind ),
    TOKDEF( blind_buried_vias_allowed ),
    TOKDEF( board_only ),
    TOKDEF( bold ),
    TOKDEF( border ),
    TOKDEF( bottom ),
    TOKDEF( bottom_left ),
    TOKDEF( bottom_right ),
    TOKDEF( capping ),
    TOKDEF( castellated_pads ),
    TOKDEF( cells ),
    TOKDEF( center ),
    TOKDEF( chamfer ),
    TOKDEF( chamfer_ratio ),
    TOKDEF( checksum ),
    TOKDEF( circle ),
    TOKDEF( class ),
    TOKDEF( clearance ),
    TOKDEF( clearance_min ),
    TOKDEF( color ),
    TOKDEF( cols ),
    TOKDEF( column_count ),
    TOKDEF( column_widths ),
    TOKDEF( comment ),
    TOKDEF( company ),
    TOKDEF( component_class ),
    TOKDEF( component_classes ),
    TOKDEF( connect ),
    TOKDEF( connect_pads ),
    TOKDEF( convexhull ),
    TOKDEF( copper_finish ),
    TOKDEF( copper_line_width ),
    TOKDEF( copper_text_dims ),
    TOKDEF( copperpour ),
    TOKDEF( courtyard_line_width ),
    TOKDEF( covering ),
    TOKDEF( creepage ),
    TOKDEF( cross_hatch ),
    TOKDEF( crossbar ),
    TOKDEF( curve_points ),
    TOKDEF( curved_edges ),
    TOKDEF( custom ),
    TOKDEF( dash ),
    TOKDEF( dash_dot ),
    TOKDEF( dash_dot_dot ),
    TOKDEF( data ),
    TOKDEF( datasheet ),
    TOKDEF( date ),
    TOKDEF( default ),
    TOKDEF( defaults ),
    TOKDEF( descr ),
    TOKDEF( die_length ),
    TOKDEF( dielectric_constraints ),
    TOKDEF( diff_pair_gap ),
    TOKDEF( diff_pair_width ),
    TOKDEF( dimension ),
    TOKDEF( dimension_precision ),
    TOKDEF( dimension_units ),
    TOKDEF( dnp ),
    TOKDEF( dot ),
    TOKDEF( drawings ),
    TOKDEF( drill ),
    TOKDEF( edge ),
    TOKDEF( edge_clearance ),
    TOKDEF( edge_connector ),
    TOKDEF( edge_cuts_line_width ),
    TOKDEF( edge_plating ),
    TOKDEF( edge_width ),
    TOKDEF( effects ),
    TOKDEF( embedded_files ),
    TOKDEF( embedded_fonts ),
    TOKDEF( enabled ),
    TOKDEF( end ),
    TOKDEF( epsilon_r ),
    TOKDEF( exclude_from_bom ),
    TOKDEF( exclude_from_pos_files ),
    TOKDEF( extension_height ),
    TOKDEF( extension_offset ),
    TOKDEF( external ),
    TOKDEF( fab_layers_line_width ),
    TOKDEF( fab_layers_text_dims ),
    TOKDEF( face ),
    TOKDEF( false ),
    TOKDEF( feature1 ),
    TOKDEF( feature2 ),
    TOKDEF( file ),
    TOKDEF( fill ),
    TOKDEF( fill_segments ),
    TOKDEF( filled_areas_thickness ),
    TOKDEF( filled_polygon ),
    TOKDEF( fillet ),
    TOKDEF( fillet_auto ),
    TOKDEF( fillet_left ),
    TOKDEF( fillet_right ),
    TOKDEF( filling ),
    TOKDEF( filter_ratio ),
    TOKDEF( font ),
    TOKDEF( footprint ),
    TOKDEF( footprints ),
    TOKDEF( format ),
    TOKDEF( fp_arc ),
    TOKDEF( fp_circle ),
    TOKDEF( fp_curve ),
    TOKDEF( fp_line ),
    TOKDEF( fp_poly ),
    TOKDEF( fp_rect ),
    TOKDEF( fp_text ),
    TOKDEF( fp_text_box ),
    TOKDEF( free ),
    TOKDEF( front ),
    TOKDEF( front_inner_back ),
    TOKDEF( full ),
    TOKDEF( general ),
    TOKDEF( generated ),
    TOKDEF( generator ),
    TOKDEF( generator_version ),
    TOKDEF( gr_arc ),
    TOKDEF( gr_bbox ),
    TOKDEF( gr_circle ),
    TOKDEF( gr_curve ),
    TOKDEF( gr_line ),
    TOKDEF( gr_poly ),
    TOKDEF( gr_rect ),
    TOKDEF( gr_text ),
    TOKDEF( gr_text_box ),
    TOKDEF( gr_vector ),
    TOKDEF( grid_origin ),
    TOKDEF( group ),
    TOKDEF( hatch ),
    TOKDEF( hatch_border_algorithm ),
    TOKDEF( hatch_gap ),
    TOKDEF( hatch_min_hole_area ),
    TOKDEF( hatch_orientation ),
    TOKDEF( hatch_position ),
    TOKDEF( hatch_smoothing_level ),
    TOKDEF( hatch_smoothing_value ),
    TOKDEF( hatch_thickness ),
    TOKDEF( header ),
    TOKDEF( height ),
    TOKDEF( hide ),
    TOKDEF( hole_to_hole_min ),
    TOKDEF( host ),
    TOKDEF( href ),
    TOKDEF( id ),
    TOKDEF( image ),
    TOKDEF( inward ),
    TOKDEF( island ),
    TOKDEF( island_area_min ),
    TOKDEF( island_removal_mode ),
    TOKDEF( italic ),
    TOKDEF( junction ),
    TOKDEF( justify ),
    TOKDEF( keep_end_layers ),
    TOKDEF( keep_text_aligned ),
    TOKDEF( keep_upright ),
    TOKDEF( keepout ),
    TOKDEF( kicad_pcb ),
    TOKDEF( knockout ),
    TOKDEF( last_trace_width ),
    TOKDEF( layer ),
    TOKDEF( layers ),
    TOKDEF( leader ),
    TOKDEF( leader_length ),
    TOKDEF( left ),
    TOKDEF( legacy_teardrops ),
    TOKDEF( length_ratio ),
    TOKDEF( length_ratio_decimals ),
    TOKDEF( length_set ),
    TOKDEF( line_spacing ),
    TOKDEF( linear ),
    TOKDEF( links ),
    TOKDEF( locked ),
    TOKDEF( loss_tangent ),
    TOKDEF( margins ),
    TOKDEF( material ),
    TOKDEF( max_error ),
    TOKDEF( max_length ),
    TOKDEF( max_width ),
    TOKDEF( members ),
    TOKDEF( micro ),
    TOKDEF( mid ),
    TOKDEF( min_thickness ),
    TOKDEF( mirror ),
    TOKDEF( mod_edge_width ),
    TOKDEF( mod_text_size ),
    TOKDEF( mod_text_width ),
    TOKDEF( mode ),
    TOKDEF( model ),
    TOKDEF( module ),
    TOKDEF( name ),
    TOKDEF( net ),
    TOKDEF( net_class ),
    TOKDEF( net_name ),
    TOKDEF( net_tie_pad_groups ),
    TOKDEF( nets ),
    TOKDEF( no ),
    TOKDEF( no_connects ),
    TOKDEF( none ),
    TOKDEF( not_allowed ),
    TOKDEF( np_thru_hole ),
    TOKDEF( offset ),
    TOKDEF( opacity ),
    TOKDEF( options ),
    TOKDEF( orientation ),
    TOKDEF( orthogonal ),
    TOKDEF( other ),
    TOKDEF( other_layers_line_width ),
    TOKDEF( other_layers_text_dims ),
    TOKDEF( outline ),
    TOKDEF( outward ),
    TOKDEF( oval ),
    TOKDEF( override_value ),
    TOKDEF( pad ),
    TOKDEF( pad_drill ),
    TOKDEF( pad_prop_bga ),
    TOKDEF( pad_prop_castellated ),
    TOKDEF( pad_prop_fiducial_glob ),
    TOKDEF( pad_prop_fiducial_loc ),
    TOKDEF( pad_prop_heatsink ),
    TOKDEF( pad_prop_mechanical ),
    TOKDEF( pad_prop_testpoint ),
    TOKDEF( pad_size ),
    TOKDEF( pad_to_mask_clearance ),
    TOKDEF( pad_to_paste_clearance ),
    TOKDEF( pad_to_paste_clearance_ratio ),
    TOKDEF( pads ),
    TOKDEF( padstack ),
    TOKDEF( padvia ),
    TOKDEF( page ),
    TOKDEF( paper ),
    TOKDEF( parameters ),
    TOKDEF( path ),
    TOKDEF( pcb_text_size ),
    TOKDEF( pcb_text_width ),
    TOKDEF( pcbplotparams ),
    TOKDEF( pinfunction ),
    TOKDEF( pintype ),
    TOKDEF( placed ),
    TOKDEF( placement ),
    TOKDEF( plugging ),
    TOKDEF( plus ),
    TOKDEF( polygon ),
    TOKDEF( portrait ),
    TOKDEF( position ),
    TOKDEF( precision ),
    TOKDEF( prefer_zone_connections ),
    TOKDEF( prefix ),
    TOKDEF( primitives ),
    TOKDEF( priority ),
    TOKDEF( private_layers ),
    TOKDEF( property ),
    TOKDEF( pts ),
    TOKDEF( radial ),
    TOKDEF( radius ),
    TOKDEF( rect ),
    TOKDEF( rect_delta ),
    TOKDEF( reference ),
    TOKDEF( remove_unused_layers ),
    TOKDEF( render_cache ),
    TOKDEF( rev ),
    TOKDEF( reverse_hatch ),
    TOKDEF( right ),
    TOKDEF( rotate ),
    TOKDEF( roundedtrackscorner ),
    TOKDEF( roundrect ),
    TOKDEF( roundrect_rratio ),
    TOKDEF( row_heights ),
    TOKDEF( rows ),
    TOKDEF( scale ),
    TOKDEF( segment ),
    TOKDEF( segment_width ),
    TOKDEF( segments ),
    TOKDEF( separators ),
    TOKDEF( setup ),
    TOKDEF( shape ),
    TOKDEF( sheetfile ),
    TOKDEF( sheetname ),
    TOKDEF( silk_line_width ),
    TOKDEF( silk_text_dims ),
    TOKDEF( size ),
    TOKDEF( smd ),
    TOKDEF( smoothing ),
    TOKDEF( solder_mask_margin ),
    TOKDEF( solder_mask_min_width ),
    TOKDEF( solder_paste_margin ),
    TOKDEF( solder_paste_margin_ratio ),
    TOKDEF( solder_paste_ratio ),
    TOKDEF( solid ),
    TOKDEF( span ),
    TOKDEF( stackup ),
    TOKDEF( start ),
    TOKDEF( status ),
    TOKDEF( stroke ),
    TOKDEF( style ),
    TOKDEF( suffix ),
    TOKDEF( suppress_zeroes ),
    TOKDEF( table ),
    TOKDEF( table_cell ),
    TOKDEF( tags ),
    TOKDEF( target ),
    TOKDEF( teardrop ),
    TOKDEF( teardrops ),
    TOKDEF( tedit ),
    TOKDEF( tenting ),
    TOKDEF( text_frame ),
    TOKDEF( text_position_mode ),
    TOKDEF( thermal_bridge_angle ),
    TOKDEF( thermal_bridge_width ),
    TOKDEF( thermal_gap ),
    TOKDEF( thermal_width ),
    TOKDEF( thickness ),
    TOKDEF( through_hole ),
    TOKDEF( through_hole_min ),
    TOKDEF( thru ),
    TOKDEF( thru_hole ),
    TOKDEF( thru_hole_only ),
    TOKDEF( title ),
    TOKDEF( title_block ),
    TOKDEF( top ),
    TOKDEF( top_left ),
    TOKDEF( top_right ),
    TOKDEF( trace_clearance ),
    TOKDEF( trace_min ),
    TOKDEF( trace_width ),
    TOKDEF( track_end ),
    TOKDEF( tracks ),
    TOKDEF( trapezoid ),
    TOKDEF( true ),
    TOKDEF( tstamp ),
    TOKDEF( type ),
    TOKDEF( units ),
    TOKDEF( units_format ),
    TOKDEF( unlocked ),
    TOKDEF( user ),
    TOKDEF( user_diff_pair ),
    TOKDEF( user_trace_width ),
    TOKDEF( user_via ),
    TOKDEF( uuid ),
    TOKDEF( uvia_dia ),
    TOKDEF( uvia_drill ),
    TOKDEF( uvia_min_drill ),
    TOKDEF( uvia_min_size ),
    TOKDEF( uvia_size ),
    TOKDEF( uvias_allowed ),
    TOKDEF( value ),
    TOKDEF( version ),
    TOKDEF( via ),
    TOKDEF( via_dia ),
    TOKDEF( via_drill ),
    TOKDEF( via_min_annulus ),
    TOKDEF( via_min_drill ),
    TOKDEF( via_min_size ),
    TOKDEF( via_size ),
    TOKDEF( vias ),
    TOKDEF( virtual ),
    TOKDEF( visible_elements ),
    TOKDEF( width ),
    TOKDEF( width_ratio ),
    TOKDEF( worksheet ),
    TOKDEF( x ),
    TOKDEF( xy ),
    TOKDEF( xyz ),
    TOKDEF( yes ),
    TOKDEF( zone ),
    TOKDEF( zone_45_only ),
    TOKDEF( zone_clearance ),
    TOKDEF( zone_connect ),
    TOKDEF( zone_defaults ),
    TOKDEF( zone_layer_connections ),
    TOKDEF( zone_type ),
    TOKDEF( zones )
};

const unsigned PCB_LEXER::keyword_count = unsigned( sizeof( PCB_LEXER::keywords )/sizeof( PCB_LEXER::keywords[0] ) );


const char* PCB_LEXER::TokenName( T aTok )
{
    const char* ret;

    if( aTok < 0 )
        ret = DSNLEXER::Syntax( aTok );
    else if( (unsigned) aTok < keyword_count )
        ret = keywords[aTok].name;
    else
        ret = "token too big";

    return ret;
}


const KEYWORD_MAP PCB_LEXER::keywords_hash({
    { "add_net", 0 },
    { "addsublayer", 1 },
    { "aligned", 2 },
    { "allow_missing_courtyard", 3 },
    { "allow_soldermask_bridges", 4 },
    { "allow_soldermask_bridges_in_footprints", 5 },
    { "allow_two_segments", 6 },
    { "allowed", 7 },
    { "anchor", 8 },
    { "angle", 9 },
    { "arc", 10 },
    { "arc_segments", 11 },
    { "area", 12 },
    { "arrow1a", 13 },
    { "arrow1b", 14 },
    { "arrow2a", 15 },
    { "arrow2b", 16 },
    { "arrow_direction", 17 },
    { "arrow_length", 18 },
    { "at", 19 },
    { "attr", 20 },
    { "autoplace_cost180", 21 },
    { "autoplace_cost90", 22 },
    { "aux_axis_origin", 23 },
    { "back", 24 },
    { "best_length_ratio", 25 },
    { "best_width_ratio", 26 },
    { "bevelled", 27 },
    { "blind", 28 },
    { "blind_buried_vias_allowed", 29 },
    { "board_only", 30 },
    { "bold", 31 },
    { "border", 32 },
    { "bottom", 33 },
    { "bottom_left", 34 },
    { "bottom_right", 35 },
    { "capping", 36 },
    { "castellated_pads", 37 },
    { "cells", 38 },
    { "center", 39 },
    { "chamfer", 40 },
    { "chamfer_ratio", 41 },
    { "checksum", 42 },
    { "circle", 43 },
    { "class", 44 },
    { "clearance", 45 },
    { "clearance_min", 46 },
    { "color", 47 },
    { "cols", 48 },
    { "column_count", 49 },
    { "column_widths", 50 },
    { "comment", 51 },
    { "company", 52 },
    { "component_class", 53 },
    { "component_classes", 54 },
    { "connect", 55 },
    { "connect_pads", 56 },
    { "convexhull", 57 },
    { "copper_finish", 58 },
    { "copper_line_width", 59 },
    { "copper_text_dims", 60 },
    { "copperpour", 61 },
    { "courtyard_line_width", 62 },
    { "covering", 63 },
    { "creepage", 64 },
    { "cross_hatch", 65 },
    { "crossbar", 66 },
    { "curve_points", 67 },
    { "curved_edges", 68 },
    { "custom", 69 },
    { "dash", 70 },
    { "dash_dot", 71 },
    { "dash_dot_dot", 72 },
    { "data", 73 },
    { "datasheet", 74 },
    { "date", 75 },
    { "default", 76 },
    { "defaults", 77 },
    { "descr", 78 },
    { "die_length", 79 },
    { "dielectric_constraints", 80 },
    { "diff_pair_gap", 81 },
    { "diff_pair_width", 82 },
    { "dimension", 83 },
    { "dimension_precision", 84 },
    { "dimension_units", 85 },
    { "dnp", 86 },
    { "dot", 87 },
    { "drawings", 88 },
    { "drill", 89 },
    { "edge", 90 },
    { "edge_clearance", 91 },
    { "edge_connector", 92 },
    { "edge_cuts_line_width", 93 },
    { "edge_plating", 94 },
    { "edge_width", 95 },
    { "effects", 96 },
    { "embedded_files", 97 },
    { "embedded_fonts", 98 },
    { "enabled", 99 },
    { "end", 100 },
    { "epsilon_r", 101 },
    { "exclude_from_bom", 102 },
    { "exclude_from_pos_files", 103 },
    { "extension_height", 104 },
    { "extension_offset", 105 },
    { "external", 106 },
    { "fab_layers_line_width", 107 },
    { "fab_layers_text_dims", 108 },
    { "face", 109 },
    { "false", 110 },
    { "feature1", 111 },
    { "feature2", 112 },
    { "file", 113 },
    { "fill", 114 },
    { "fill_segments", 115 },
    { "filled_areas_thickness", 116 },
    { "filled_polygon", 117 },
    { "fillet", 118 },
    { "fillet_auto", 119 },
    { "fillet_left", 120 },
    { "fillet_right", 121 },
    { "filling", 122 },
    { "filter_ratio", 123 },
    { "font", 124 },
    { "footprint", 125 },
    { "footprints", 126 },
    { "format", 127 },
    { "fp_arc", 128 },
    { "fp_circle", 129 },
    { "fp_curve", 130 },
    { "fp_line", 131 },
    { "fp_poly", 132 },
    { "fp_rect", 133 },
    { "fp_text", 134 },
    { "fp_text_box", 135 },
    { "free", 136 },
    { "front", 137 },
    { "front_inner_back", 138 },
    { "full", 139 },
    { "general", 140 },
    { "generated", 141 },
    { "generator", 142 },
    { "generator_version", 143 },
    { "gr_arc", 144 },
    { "gr_bbox", 145 },
    { "gr_circle", 146 },
    { "gr_curve", 147 },
    { "gr_line", 148 },
    { "gr_poly", 149 },
    { "gr_rect", 150 },
    { "gr_text", 151 },
    { "gr_text_box", 152 },
    { "gr_vector", 153 },
    { "grid_origin", 154 },
    { "group", 155 },
    { "hatch", 156 },
    { "hatch_border_algorithm", 157 },
    { "hatch_gap", 158 },
    { "hatch_min_hole_area", 159 },
    { "hatch_orientation", 160 },
    { "hatch_position", 161 },
    { "hatch_smoothing_level", 162 },
    { "hatch_smoothing_value", 163 },
    { "hatch_thickness", 164 },
    { "header", 165 },
    { "height", 166 },
    { "hide", 167 },
    { "hole_to_hole_min", 168 },
    { "host", 169 },
    { "href", 170 },
    { "id", 171 },
    { "image", 172 },
    { "inward", 173 },
    { "island", 174 },
    { "island_area_min", 175 },
    { "island_removal_mode", 176 },
    { "italic", 177 },
    { "junction", 178 },
    { "justify", 179 },
    { "keep_end_layers", 180 },
    { "keep_text_aligned", 181 },
    { "keep_upright", 182 },
    { "keepout", 183 },
    { "kicad_pcb", 184 },
    { "knockout", 185 },
    { "last_trace_width", 186 },
    { "layer", 187 },
    { "layers", 188 },
    { "leader", 189 },
    { "leader_length", 190 },
    { "left", 191 },
    { "legacy_teardrops", 192 },
    { "length_ratio", 193 },
    { "length_ratio_decimals", 194 },
    { "length_set", 195 },
    { "line_spacing", 196 },
    { "linear", 197 },
    { "links", 198 },
    { "locked", 199 },
    { "loss_tangent", 200 },
    { "margins", 201 },
    { "material", 202 },
    { "max_error", 203 },
    { "max_length", 204 },
    { "max_width", 205 },
    { "members", 206 },
    { "micro", 207 },
    { "mid", 208 },
    { "min_thickness", 209 },
    { "mirror", 210 },
    { "mod_edge_width", 211 },
    { "mod_text_size", 212 },
    { "mod_text_width", 213 },
    { "mode", 214 },
    { "model", 215 },
    { "module", 216 },
    { "name", 217 },
    { "net", 218 },
    { "net_class", 219 },
    { "net_name", 220 },
    { "net_tie_pad_groups", 221 },
    { "nets", 222 },
    { "no", 223 },
    { "no_connects", 224 },
    { "none", 225 },
    { "not_allowed", 226 },
    { "np_thru_hole", 227 },
    { "offset", 228 },
    { "opacity", 229 },
    { "options", 230 },
    { "orientation", 231 },
    { "orthogonal", 232 },
    { "other", 233 },
    { "other_layers_line_width", 234 },
    { "other_layers_text_dims", 235 },
    { "outline", 236 },
    { "outward", 237 },
    { "oval", 238 },
    { "override_value", 239 },
    { "pad", 240 },
    { "pad_drill", 241 },
    { "pad_prop_bga", 242 },
    { "pad_prop_castellated", 243 },
    { "pad_prop_fiducial_glob", 244 },
    { "pad_prop_fiducial_loc", 245 },
    { "pad_prop_heatsink", 246 },
    { "pad_prop_mechanical", 247 },
    { "pad_prop_testpoint", 248 },
    { "pad_size", 249 },
    { "pad_to_mask_clearance", 250 },
    { "pad_to_paste_clearance", 251 },
    { "pad_to_paste_clearance_ratio", 252 },
    { "pads", 253 },
    { "padstack", 254 },
    { "padvia", 255 },
    { "page", 256 },
    { "paper", 257 },
    { "parameters", 258 },
    { "path", 259 },
    { "pcb_text_size", 260 },
    { "pcb_text_width", 261 },
    { "pcbplotparams", 262 },
    { "pinfunction", 263 },
    { "pintype", 264 },
    { "placed", 265 },
    { "placement", 266 },
    { "plugging", 267 },
    { "plus", 268 },
    { "polygon", 269 },
    { "portrait", 270 },
    { "position", 271 },
    { "precision", 272 },
    { "prefer_zone_connections", 273 },
    { "prefix", 274 },
    { "primitives", 275 },
    { "priority", 276 },
    { "private_layers", 277 },
    { "property", 278 },
    { "pts", 279 },
    { "radial", 280 },
    { "radius", 281 },
    { "rect", 282 },
    { "rect_delta", 283 },
    { "reference", 284 },
    { "remove_unused_layers", 285 },
    { "render_cache", 286 },
    { "rev", 287 },
    { "reverse_hatch", 288 },
    { "right", 289 },
    { "rotate", 290 },
    { "roundedtrackscorner", 291 },
    { "roundrect", 292 },
    { "roundrect_rratio", 293 },
    { "row_heights", 294 },
    { "rows", 295 },
    { "scale", 296 },
    { "segment", 297 },
    { "segment_width", 298 },
    { "segments", 299 },
    { "separators", 300 },
    { "setup", 301 },
    { "shape", 302 },
    { "sheetfile", 303 },
    { "sheetname", 304 },
    { "silk_line_width", 305 },
    { "silk_text_dims", 306 },
    { "size", 307 },
    { "smd", 308 },
    { "smoothing", 309 },
    { "solder_mask_margin", 310 },
    { "solder_mask_min_width", 311 },
    { "solder_paste_margin", 312 },
    { "solder_paste_margin_ratio", 313 },
    { "solder_paste_ratio", 314 },
    { "solid", 315 },
    { "span", 316 },
    { "stackup", 317 },
    { "start", 318 },
    { "status", 319 },
    { "stroke", 320 },
    { "style", 321 },
    { "suffix", 322 },
    { "suppress_zeroes", 323 },
    { "table", 324 },
    { "table_cell", 325 },
    { "tags", 326 },
    { "target", 327 },
    { "teardrop", 328 },
    { "teardrops", 329 },
    { "tedit", 330 },
    { "tenting", 331 },
    { "text_frame", 332 },
    { "text_position_mode", 333 },
    { "thermal_bridge_angle", 334 },
    { "thermal_bridge_width", 335 },
    { "thermal_gap", 336 },
    { "thermal_width", 337 },
    { "thickness", 338 },
    { "through_hole", 339 },
    { "through_hole_min", 340 },
    { "thru", 341 },
    { "thru_hole", 342 },
    { "thru_hole_only", 343 },
    { "title", 344 },
    { "title_block", 345 },
    { "top", 346 },
    { "top_left", 347 },
    { "top_right", 348 },
    { "trace_clearance", 349 },
    { "trace_min", 350 },
    { "trace_width", 351 },
    { "track_end", 352 },
    { "tracks", 353 },
    { "trapezoid", 354 },
    { "true", 355 },
    { "tstamp", 356 },
    { "type", 357 },
    { "units", 358 },
    { "units_format", 359 },
    { "unlocked", 360 },
    { "user", 361 },
    { "user_diff_pair", 362 },
    { "user_trace_width", 363 },
    { "user_via", 364 },
    { "uuid", 365 },
    { "uvia_dia", 366 },
    { "uvia_drill", 367 },
    { "uvia_min_drill", 368 },
    { "uvia_min_size", 369 },
    { "uvia_size", 370 },
    { "uvias_allowed", 371 },
    { "value", 372 },
    { "version", 373 },
    { "via", 374 },
    { "via_dia", 375 },
    { "via_drill", 376 },
    { "via_min_annulus", 377 },
    { "via_min_drill", 378 },
    { "via_min_size", 379 },
    { "via_size", 380 },
    { "vias", 381 },
    { "virtual", 382 },
    { "visible_elements", 383 },
    { "width", 384 },
    { "width_ratio", 385 },
    { "worksheet", 386 },
    { "x", 387 },
    { "xy", 388 },
    { "xyz", 389 },
    { "yes", 390 },
    { "zone", 391 },
    { "zone_45_only", 392 },
    { "zone_clearance", 393 },
    { "zone_connect", 394 },
    { "zone_defaults", 395 },
    { "zone_layer_connections", 396 },
    { "zone_type", 397 },
    { "zones", 398 }
});