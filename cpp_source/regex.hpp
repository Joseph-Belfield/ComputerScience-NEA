#include "./regex_and_arrayFunctions.hpp"

string stem_finder(string moleculeName)
{
    // initializes match variable and regex variable
    smatch match;

    // searches for all valid names split into (prefix:stem:suffix) - current regex is placeholder
    regex reg(R"(^((?:-?\d+(?:,\d+)*-)?(?:MULTIPLIER)?(?:fluoro|chloro|bromo|iodo|hydroxy|mercapto|oxo|formyl|carboxy|pheno|amido|imino))*((?:cyclo)?(?:STEM|(?:benzene)))((?:ane|an)?(?:(?:-?\d+(?:,\d+)*-)?(?:MULTIPLIER)?(?:(?:ene|en)|(?:yne|yn)|(?:ol|thiol|one|al|oic acid|carboxylic acid|amide|imide))){0,3})$)");

    // searches the entire string for a match with regex, then saves matches in match
    regex_match(moleculeName, match, reg);
    
    // returns string 'null' if the molecule name is invalid
    if (match.empty() == 1) {return "null";}

    // stores the stem as seperate string - write regex so its split into prefix - stem - suffix
    string stem = match.str(2);

    // returns the stem string
    return stem;
}