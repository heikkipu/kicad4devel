
/* Do not modify this file it was automatically generated by the
 * TokenList2DsnLexer CMake script.
 *
 * Include this file in your lexer class to provide the keywords for
 * your DSN lexer.
 */

#include </opt/kicad/program/master/Tears_Master/build/common/netlist_lexer.h>

using namespace NL_T;

#define TOKDEF(x)    { #x, T_##x }

const KEYWORD NETLIST_LEXER::keywords[] = {
    TOKDEF( alias ),
    TOKDEF( aliases ),
    TOKDEF( class ),
    TOKDEF( code ),
    TOKDEF( comp ),
    TOKDEF( component_classes ),
    TOKDEF( components ),
    TOKDEF( datasheet ),
    TOKDEF( date ),
    TOKDEF( description ),
    TOKDEF( design ),
    TOKDEF( docs ),
    TOKDEF( duplicate_pin_numbers_are_jumpers ),
    TOKDEF( export ),
    TOKDEF( field ),
    TOKDEF( fields ),
    TOKDEF( footprint ),
    TOKDEF( footprints ),
    TOKDEF( fp ),
    TOKDEF( group ),
    TOKDEF( jumper_pin_groups ),
    TOKDEF( lib ),
    TOKDEF( libpart ),
    TOKDEF( libparts ),
    TOKDEF( libraries ),
    TOKDEF( library ),
    TOKDEF( libsource ),
    TOKDEF( name ),
    TOKDEF( names ),
    TOKDEF( net ),
    TOKDEF( nets ),
    TOKDEF( node ),
    TOKDEF( num ),
    TOKDEF( part ),
    TOKDEF( pin ),
    TOKDEF( pinfunction ),
    TOKDEF( pins ),
    TOKDEF( pintype ),
    TOKDEF( property ),
    TOKDEF( ref ),
    TOKDEF( sheetpath ),
    TOKDEF( source ),
    TOKDEF( tool ),
    TOKDEF( tstamp ),
    TOKDEF( tstamps ),
    TOKDEF( uri ),
    TOKDEF( value ),
    TOKDEF( version )
};

const unsigned NETLIST_LEXER::keyword_count = unsigned( sizeof( NETLIST_LEXER::keywords )/sizeof( NETLIST_LEXER::keywords[0] ) );


const char* NETLIST_LEXER::TokenName( T aTok )
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


const KEYWORD_MAP NETLIST_LEXER::keywords_hash({
    { "alias", 0 },
    { "aliases", 1 },
    { "class", 2 },
    { "code", 3 },
    { "comp", 4 },
    { "component_classes", 5 },
    { "components", 6 },
    { "datasheet", 7 },
    { "date", 8 },
    { "description", 9 },
    { "design", 10 },
    { "docs", 11 },
    { "duplicate_pin_numbers_are_jumpers", 12 },
    { "export", 13 },
    { "field", 14 },
    { "fields", 15 },
    { "footprint", 16 },
    { "footprints", 17 },
    { "fp", 18 },
    { "group", 19 },
    { "jumper_pin_groups", 20 },
    { "lib", 21 },
    { "libpart", 22 },
    { "libparts", 23 },
    { "libraries", 24 },
    { "library", 25 },
    { "libsource", 26 },
    { "name", 27 },
    { "names", 28 },
    { "net", 29 },
    { "nets", 30 },
    { "node", 31 },
    { "num", 32 },
    { "part", 33 },
    { "pin", 34 },
    { "pinfunction", 35 },
    { "pins", 36 },
    { "pintype", 37 },
    { "property", 38 },
    { "ref", 39 },
    { "sheetpath", 40 },
    { "source", 41 },
    { "tool", 42 },
    { "tstamp", 43 },
    { "tstamps", 44 },
    { "uri", 45 },
    { "value", 46 },
    { "version", 47 }
});