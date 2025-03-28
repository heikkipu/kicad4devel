
/* Do not modify this file it was automatically generated by the
 * TokenList2DsnLexer CMake script.
 *
 * Include this file in your lexer class to provide the keywords for
 * your DSN lexer.
 */

#include </opt/kicad/program/master/Tears_Master/build/eeschema/schematic_lexer.h>

using namespace TSCHEMATIC_T;

#define TOKDEF(x)    { #x, T_##x }

const KEYWORD SCHEMATIC_LEXER::keywords[] = {
    TOKDEF( alternate ),
    TOKDEF( anchor ),
    TOKDEF( angles ),
    TOKDEF( arc ),
    TOKDEF( at ),
    TOKDEF( atomic ),
    TOKDEF( background ),
    TOKDEF( bezier ),
    TOKDEF( bidirectional ),
    TOKDEF( bitmap ),
    TOKDEF( bold ),
    TOKDEF( border ),
    TOKDEF( bottom ),
    TOKDEF( bus ),
    TOKDEF( bus_alias ),
    TOKDEF( bus_entry ),
    TOKDEF( cells ),
    TOKDEF( center ),
    TOKDEF( checksum ),
    TOKDEF( circle ),
    TOKDEF( clock ),
    TOKDEF( clock_low ),
    TOKDEF( color ),
    TOKDEF( cols ),
    TOKDEF( column_count ),
    TOKDEF( column_widths ),
    TOKDEF( comment ),
    TOKDEF( company ),
    TOKDEF( convert ),
    TOKDEF( cross_hatch ),
    TOKDEF( data ),
    TOKDEF( date ),
    TOKDEF( default ),
    TOKDEF( default_instance ),
    TOKDEF( diameter ),
    TOKDEF( diamond ),
    TOKDEF( directive_label ),
    TOKDEF( dnp ),
    TOKDEF( do_not_autoplace ),
    TOKDEF( dot ),
    TOKDEF( duplicate_pin_numbers_are_jumpers ),
    TOKDEF( edge_clock_high ),
    TOKDEF( effects ),
    TOKDEF( embedded_files ),
    TOKDEF( embedded_fonts ),
    TOKDEF( end ),
    TOKDEF( exclude_from_sim ),
    TOKDEF( extends ),
    TOKDEF( external ),
    TOKDEF( face ),
    TOKDEF( fields_autoplaced ),
    TOKDEF( file ),
    TOKDEF( fill ),
    TOKDEF( font ),
    TOKDEF( footprint ),
    TOKDEF( free ),
    TOKDEF( generator ),
    TOKDEF( generator_version ),
    TOKDEF( global ),
    TOKDEF( global_label ),
    TOKDEF( hatch ),
    TOKDEF( header ),
    TOKDEF( hide ),
    TOKDEF( hierarchical_label ),
    TOKDEF( hint_alt_swap ),
    TOKDEF( hint_pin_swap ),
    TOKDEF( host ),
    TOKDEF( href ),
    TOKDEF( id ),
    TOKDEF( image ),
    TOKDEF( in_bom ),
    TOKDEF( input ),
    TOKDEF( input_low ),
    TOKDEF( instances ),
    TOKDEF( inverted ),
    TOKDEF( inverted_clock ),
    TOKDEF( iref ),
    TOKDEF( italic ),
    TOKDEF( jumper_pin_groups ),
    TOKDEF( junction ),
    TOKDEF( justify ),
    TOKDEF( kicad_sch ),
    TOKDEF( kicad_symbol_lib ),
    TOKDEF( label ),
    TOKDEF( left ),
    TOKDEF( length ),
    TOKDEF( lib_id ),
    TOKDEF( lib_name ),
    TOKDEF( lib_symbols ),
    TOKDEF( line ),
    TOKDEF( line_spacing ),
    TOKDEF( local ),
    TOKDEF( margins ),
    TOKDEF( members ),
    TOKDEF( mid ),
    TOKDEF( mirror ),
    TOKDEF( name ),
    TOKDEF( netclass_flag ),
    TOKDEF( no ),
    TOKDEF( no_connect ),
    TOKDEF( non_logic ),
    TOKDEF( none ),
    TOKDEF( number ),
    TOKDEF( offset ),
    TOKDEF( on_board ),
    TOKDEF( open_collector ),
    TOKDEF( open_emitter ),
    TOKDEF( outline ),
    TOKDEF( output ),
    TOKDEF( output_low ),
    TOKDEF( page ),
    TOKDEF( paper ),
    TOKDEF( passive ),
    TOKDEF( path ),
    TOKDEF( pin ),
    TOKDEF( pin_del ),
    TOKDEF( pin_merge ),
    TOKDEF( pin_names ),
    TOKDEF( pin_numbers ),
    TOKDEF( pin_rename ),
    TOKDEF( polyline ),
    TOKDEF( portrait ),
    TOKDEF( power ),
    TOKDEF( power_in ),
    TOKDEF( power_out ),
    TOKDEF( private ),
    TOKDEF( project ),
    TOKDEF( property ),
    TOKDEF( property_del ),
    TOKDEF( pts ),
    TOKDEF( radius ),
    TOKDEF( rectangle ),
    TOKDEF( reference ),
    TOKDEF( required ),
    TOKDEF( rev ),
    TOKDEF( reverse_hatch ),
    TOKDEF( right ),
    TOKDEF( round ),
    TOKDEF( row_heights ),
    TOKDEF( rows ),
    TOKDEF( rule_area ),
    TOKDEF( scale ),
    TOKDEF( separators ),
    TOKDEF( shape ),
    TOKDEF( sheet ),
    TOKDEF( sheet_instances ),
    TOKDEF( show_name ),
    TOKDEF( size ),
    TOKDEF( span ),
    TOKDEF( start ),
    TOKDEF( stroke ),
    TOKDEF( symbol ),
    TOKDEF( symbol_instances ),
    TOKDEF( table ),
    TOKDEF( table_cell ),
    TOKDEF( text ),
    TOKDEF( text_box ),
    TOKDEF( thickness ),
    TOKDEF( title ),
    TOKDEF( title_block ),
    TOKDEF( top ),
    TOKDEF( tri_state ),
    TOKDEF( type ),
    TOKDEF( unconnected ),
    TOKDEF( unit ),
    TOKDEF( unit_name ),
    TOKDEF( unspecified ),
    TOKDEF( uuid ),
    TOKDEF( value ),
    TOKDEF( version ),
    TOKDEF( width ),
    TOKDEF( wire ),
    TOKDEF( x ),
    TOKDEF( xy ),
    TOKDEF( y ),
    TOKDEF( yes )
};

const unsigned SCHEMATIC_LEXER::keyword_count = unsigned( sizeof( SCHEMATIC_LEXER::keywords )/sizeof( SCHEMATIC_LEXER::keywords[0] ) );


const char* SCHEMATIC_LEXER::TokenName( T aTok )
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


const KEYWORD_MAP SCHEMATIC_LEXER::keywords_hash({
    { "alternate", 0 },
    { "anchor", 1 },
    { "angles", 2 },
    { "arc", 3 },
    { "at", 4 },
    { "atomic", 5 },
    { "background", 6 },
    { "bezier", 7 },
    { "bidirectional", 8 },
    { "bitmap", 9 },
    { "bold", 10 },
    { "border", 11 },
    { "bottom", 12 },
    { "bus", 13 },
    { "bus_alias", 14 },
    { "bus_entry", 15 },
    { "cells", 16 },
    { "center", 17 },
    { "checksum", 18 },
    { "circle", 19 },
    { "clock", 20 },
    { "clock_low", 21 },
    { "color", 22 },
    { "cols", 23 },
    { "column_count", 24 },
    { "column_widths", 25 },
    { "comment", 26 },
    { "company", 27 },
    { "convert", 28 },
    { "cross_hatch", 29 },
    { "data", 30 },
    { "date", 31 },
    { "default", 32 },
    { "default_instance", 33 },
    { "diameter", 34 },
    { "diamond", 35 },
    { "directive_label", 36 },
    { "dnp", 37 },
    { "do_not_autoplace", 38 },
    { "dot", 39 },
    { "duplicate_pin_numbers_are_jumpers", 40 },
    { "edge_clock_high", 41 },
    { "effects", 42 },
    { "embedded_files", 43 },
    { "embedded_fonts", 44 },
    { "end", 45 },
    { "exclude_from_sim", 46 },
    { "extends", 47 },
    { "external", 48 },
    { "face", 49 },
    { "fields_autoplaced", 50 },
    { "file", 51 },
    { "fill", 52 },
    { "font", 53 },
    { "footprint", 54 },
    { "free", 55 },
    { "generator", 56 },
    { "generator_version", 57 },
    { "global", 58 },
    { "global_label", 59 },
    { "hatch", 60 },
    { "header", 61 },
    { "hide", 62 },
    { "hierarchical_label", 63 },
    { "hint_alt_swap", 64 },
    { "hint_pin_swap", 65 },
    { "host", 66 },
    { "href", 67 },
    { "id", 68 },
    { "image", 69 },
    { "in_bom", 70 },
    { "input", 71 },
    { "input_low", 72 },
    { "instances", 73 },
    { "inverted", 74 },
    { "inverted_clock", 75 },
    { "iref", 76 },
    { "italic", 77 },
    { "jumper_pin_groups", 78 },
    { "junction", 79 },
    { "justify", 80 },
    { "kicad_sch", 81 },
    { "kicad_symbol_lib", 82 },
    { "label", 83 },
    { "left", 84 },
    { "length", 85 },
    { "lib_id", 86 },
    { "lib_name", 87 },
    { "lib_symbols", 88 },
    { "line", 89 },
    { "line_spacing", 90 },
    { "local", 91 },
    { "margins", 92 },
    { "members", 93 },
    { "mid", 94 },
    { "mirror", 95 },
    { "name", 96 },
    { "netclass_flag", 97 },
    { "no", 98 },
    { "no_connect", 99 },
    { "non_logic", 100 },
    { "none", 101 },
    { "number", 102 },
    { "offset", 103 },
    { "on_board", 104 },
    { "open_collector", 105 },
    { "open_emitter", 106 },
    { "outline", 107 },
    { "output", 108 },
    { "output_low", 109 },
    { "page", 110 },
    { "paper", 111 },
    { "passive", 112 },
    { "path", 113 },
    { "pin", 114 },
    { "pin_del", 115 },
    { "pin_merge", 116 },
    { "pin_names", 117 },
    { "pin_numbers", 118 },
    { "pin_rename", 119 },
    { "polyline", 120 },
    { "portrait", 121 },
    { "power", 122 },
    { "power_in", 123 },
    { "power_out", 124 },
    { "private", 125 },
    { "project", 126 },
    { "property", 127 },
    { "property_del", 128 },
    { "pts", 129 },
    { "radius", 130 },
    { "rectangle", 131 },
    { "reference", 132 },
    { "required", 133 },
    { "rev", 134 },
    { "reverse_hatch", 135 },
    { "right", 136 },
    { "round", 137 },
    { "row_heights", 138 },
    { "rows", 139 },
    { "rule_area", 140 },
    { "scale", 141 },
    { "separators", 142 },
    { "shape", 143 },
    { "sheet", 144 },
    { "sheet_instances", 145 },
    { "show_name", 146 },
    { "size", 147 },
    { "span", 148 },
    { "start", 149 },
    { "stroke", 150 },
    { "symbol", 151 },
    { "symbol_instances", 152 },
    { "table", 153 },
    { "table_cell", 154 },
    { "text", 155 },
    { "text_box", 156 },
    { "thickness", 157 },
    { "title", 158 },
    { "title_block", 159 },
    { "top", 160 },
    { "tri_state", 161 },
    { "type", 162 },
    { "unconnected", 163 },
    { "unit", 164 },
    { "unit_name", 165 },
    { "unspecified", 166 },
    { "uuid", 167 },
    { "value", 168 },
    { "version", 169 },
    { "width", 170 },
    { "wire", 171 },
    { "x", 172 },
    { "xy", 173 },
    { "y", 174 },
    { "yes", 175 }
});