/* A recursive descent parser with operator precedence.

   Author: Eric Van Wyk

   This algorithms is based on the work of Douglas Crockford in "Top
   Down Operator Precedence", a chapter in the book "Beautiful Code".
   Mr. Crockford describes in his chapter how his work comes from that
   of Based on algorithms described Vaughan Pratt in "Top Down Operator
   Precedence", presented at the ACM Symposium on Principles of
   Programming Languages.

   Douglas Crockford's chapter is available at 
    http://javascript.crockford.com/tdop/tdop.html

   Vaughan Pratt's paper is available at 
    http://portal.acm.org/citation.cfm?id=512931

   These are both quite interesting works and worth reading if you
   find the problem of parsing to be an interesting one.

   Last modified: March 5, 2013.
*/

#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "parseResult.h"

#include <string>

class ExtToken ;

class Parser {

public:
    Parser() ;
    ~Parser() ;

    // starting method for all other parses:
    ParseResult parse (const char *text) ;
    
    // methods for the nonterminals:
    ParseResult parseProgram () ;
    ParseResult parseDecl () ;
    ParseResult parseStandardDecl () ;
    ParseResult parseMatrixDecl () ;
    ParseResult parseStmts () ;
    ParseResult parseStmt () ;
    ParseResult parseExpr (int rbp) ;

    // methods for parsing productions for Expr:
    ParseResult parseTrueKwd () ;
    ParseResult parseFalseKwd () ;
    ParseResult parseIntConst () ;
    ParseResult parseFloatConst () ;
    ParseResult parseStringConst () ;
    ParseResult parseCharConst () ;
    ParseResult parseVariableName () ;
    ParseResult parseNestedExpr () ;
    ParseResult parseNotExpr() ;
    ParseResult parseLetExpr() ;
    ParseResult parseIfExpr() ;
    ParseResult parseAddition ( ParseResult left ) ;
    ParseResult parseMultiplication ( ParseResult left ) ;
    ParseResult parseSubtraction ( ParseResult left ) ;
    ParseResult parseDivision ( ParseResult left ) ;

    // methods for parsing statements of equality:
    ParseResult parseEqualEquals ( ParseResult left ) ;
    ParseResult parseLessThanEquals ( ParseResult left ) ;
    ParseResult parseGreaterThanEquals ( ParseResult left ) ;
    ParseResult parseNotEquals ( ParseResult left ) ;
    ParseResult parseLessThan ( ParseResult left ) ;
    ParseResult parseGreaterThan ( ParseResult left ) ;

    // single method for parsing statements of relationality:
    ParseResult parseRelationalExpr ( ParseResult left ) ;

    // Helper function used by the parser.
    void match (tokenType tt) ;
    bool attemptMatch (tokenType tt) ;
    bool nextIs (tokenType tt) ;
    void nextToken () ;

    //! Used for identifying the terminal in parsing and errors
    std::string terminalDescription ( tokenType terminal ) ;

    //! Creates a message to display whenever there is an error
    std::string makeErrorMsg ( tokenType terminal ) ;

    //! Creates an "expected... found..." error message
    std::string makeErrorMsgExpected ( tokenType terminal ) ;

    //! Overloaded constructor of makeErrorMsg that takes only a const char*
    std::string makeErrorMsg ( const char *msg ) ;

    ExtToken *tokens ;
    ExtToken *currToken ;
    ExtToken *prevToken ;

    Token *stokens ;
    Scanner *s ;
} ;

#endif /* PARSER_H */
