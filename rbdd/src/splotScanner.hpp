// $Id: scanner.h 17 2007-08-19 18:51:39Z tb $

#ifndef SPLOTSCANNER_H
#define SPLOTSCANNER_H

// Flex expects the signature of yylex to be defined in the macro YY_DECL, and
// the C++ parser expects it to be declared. We can factor both as follows.

#ifndef __FLEX_LEXER_H
#undef yyFlexLexer  	
#define yyFlexLexer splotFlexLexer
#include "FlexLexer.h"
#endif

using namespace std;

/** Scanner is a derived class to add some extra function to the scanner
 * class. Flex itself creates a class named yyFlexLexer, which is renamed using
 * macros to KconfFlexLexer. However we change the context of the generated
 * yylex() function to be contained within the Scanner class. This is required
 * because the yylex() defined in KconfFlexLexer has no parameters. */
    

class splotScanner : public splotFlexLexer {
    
//private:
    
  //  std::string filename;

public:
    int parseSplot(string inputfile);
    
     
};

#endif // SPLOTSCANNER_H
