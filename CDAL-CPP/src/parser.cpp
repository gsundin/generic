/* A recursive descent parser with operator precedence.

   Author: Eric Van Wyk
   Modified: Robert Edge 
   Modified: Kevin Thomsen
   Modified: Dan Challou       

   This algorithm is based on the work of Douglas Crockford in "Top
   Down Operator Precedence", a chapter in the book "Beautiful Code".
   Mr. Crockford describes in his chapter how his work is based on an
   algorithm described Vaughan Pratt in "Top Down Operator
   Precedence", presented at the ACM Symposium on Principles of
   Programming Languages.

   Douglas Crockford's chapter is available at 
    http://javascript.crockford.com/tdop/tdop.html

   Vaughan Pratt's paper is available at 
    http://portal.acm.org/citation.cfm?id=512931

   These are both quite interesting works and worth reading if you
   find the problem of parsing to be an interesting one.

   Last modified: Oct 19, 2015.
*/

#include "AST.h"
#include "parser.h"
#include "scanner.h"
#include "extToken.h"
#include <stdio.h>
#include <assert.h>
using namespace std ;

/*! The deconstructor for the parser object.
    It removes the parameters from the object.
*/
Parser::~Parser() {
    if (s) delete s ;

    ExtToken *extTokenToDelete ;
    currToken = tokens ;
    while (currToken) {
        extTokenToDelete = currToken ;
        currToken = currToken->next ;
        delete extTokenToDelete ;
    }

    Token *tokenToDelete ;
    Token *currScannerToken = stokens ;
    while (currScannerToken) {
        tokenToDelete = currScannerToken ;
        currScannerToken = currScannerToken->next ;
        delete tokenToDelete ;
    }

}

/*! The constructor for the parser object.
    It builds the parameters to initialize the object.
*/
Parser::Parser() { 
    currToken = NULL; prevToken = NULL ; tokens = NULL ; 
    s = NULL ; stokens = NULL ; 
}

/*! The parse function. Takes a char array and returns a ParseResult.
*/
/*!\return ParseResult \param text The input text to parse through*/
ParseResult Parser::parse(const char *text) {
    assert (text != NULL) ;

    ParseResult pr ;
    try {
        s = new Scanner() ;
        stokens = s->scan (text) ;        
        tokens = extendTokenList (this, stokens) ;

        assert (tokens != NULL) ;
        currToken = tokens ;
        pr = parseProgram() ;
    }
    catch (string errMsg) {
        pr.ok = false ;
        pr.errors = errMsg ;
        pr.ast = NULL ;
    }
    return pr ;
}

/* 
 * parse methods for non-terminal symbols
 * --------------------------------------
 */

/*! The parseProgram function. Takes nothing and returns a ParseResult.
    This is for the Program declaration, which is the baseline of the AST.
*/
/*!\return ParseResult*/
//! Program
ParseResult Parser::parseProgram() {
    ParseResult pr ;
    // root
    // Program ::= varName '(' ')' '{' Stmts '}' 
    match(variableName) ;

    string name(prevToken->lexeme) ;

    VariableName* varName = new VariableName(name);

    match(leftParen) ;
    match(rightParen) ;
    match(leftCurly) ;

    ParseResult prStmts = parseStmts() ;
    Stmts* s = NULL ;

    if (prStmts.ast) {
        s = dynamic_cast<Stmts*>(prStmts.ast) ;
    }

    match(rightCurly) ;
    match(endOfFile) ;

    pr.ast = new Program(varName, s) ;
    
    return pr ;
}

/*! The parseMatrixDecl function. Takes nothing and returns a ParseResult.
    This is for the added matrix declaration syntax in some of the samples.
*/
/*!\return ParseResult*/
//! MatrixDecl
/// identical purpose of parseDecl, handles special matrix syntax.
ParseResult Parser::parseMatrixDecl() {
    ParseResult pr ;

    match(matrixKwd) ;
    match(variableName) ;

    /// Decl ::= 'matrix' varName '[' Expr ':' Expr ']' varName ':' varName  '=' Expr ';'
    if(attemptMatch(leftSquare)) {
        parseExpr(0) ;
        match(colon) ;
        parseExpr(0) ;
        match(rightSquare) ;
        parseVariableName() ;
        match(colon) ;
        parseVariableName() ;
        match(assign) ;
        parseExpr(0) ;
    }
    /// Decl ::= 'matrix' varName '=' Expr ';'
    else if(attemptMatch(assign)) {
        parseExpr(0) ;
    }
    else {
        throw ((string) "Bad Syntax of Matrix Decl in in parseMatrixDecl") ;
    }   

    match(semiColon) ;

    return pr ;
}

/*! The parseStandardDecl function. Takes nothing and returns a ParseResult.
    This is for the declaration of variables to ints, floats, and strings.
*/
/*!\return ParseResult*/
//!standardDecl 
///Decl ::= integerKwd varName | floatKwd varName | stringKwd varName
ParseResult Parser::parseStandardDecl() {
    ParseResult pr ;
    
    ParseResult prType = ParseResult();

    if (attemptMatch(intKwd)) {
        // Type ::= intKwd
    } 
    else if (attemptMatch(floatKwd)) {
        // Type ::= floatKwd
    }
    else if (attemptMatch(stringKwd)) {
        // Type ::= stringKwd
    }
    else if (attemptMatch(boolKwd)) {
        // Type ::= boolKwd
    }
    std::string str1 = prevToken->lexeme ;
    match(variableName) ;
    std::string str2 = prevToken->lexeme ;
    match(semiColon) ;
    pr.ast = new StandardDecl(str1, str2) ;
    return pr ;
}

/*! The parseDecl function. Takes nothing and returns a ParseResult.
    This is for the AST to find the type of declaration.
*/
/*!\return ParseResult*/
//! Decl
ParseResult Parser::parseDecl () {
    ParseResult pr ;
    /// Decl :: matrix variableName ....
    if(nextIs(matrixKwd)){
        pr = parseMatrixDecl() ;
    } 
    /// Decl ::= Type variableName semiColon
    else{
        pr = parseStandardDecl() ;
    }
    return pr ;
}

/*! The parseStmts function. Takes nothing and returns a ParseResult.
    This is to decide which Stmt's to break the Stmts into.
*/
/*!\return ParseResult*/
//! Stmts
ParseResult Parser::parseStmts () {
    ParseResult pr ;
    if (!nextIs(rightCurly) && !nextIs(inKwd)) {
        /// Stmts ::= Stmt Stmts
        ParseResult prStmt = parseStmt() ;
        ParseResult prStmts = parseStmts() ;
        pr.ast = new TwoStmts((Stmt*)prStmt.ast, (Stmts*)prStmts.ast) ;
    }
    else {
        /// Stmts ::=
        // nothing to match.
        pr.ast = new EmptyStmts() ;
    }
    return pr ;
}

/*! The parseStmt function. Takes nothing and returns a ParseResult.
    This follows directly from parseStmts and decides which Stmt to turn into.
    There are many different cases for the statement breakup.
*/
/*!\return ParseResult*/
//! Stmt
ParseResult Parser::parseStmt () {
    ParseResult pr ;

    ///Stmt ::= Decl
    if(nextIs(intKwd)||nextIs(floatKwd)||nextIs(matrixKwd)||nextIs(stringKwd)||nextIs(boolKwd)){
        pr = parseDecl() ;
    }
    ///Stmt ::= '{' Stmts '}'
    else if (attemptMatch(leftCurly)) {
        ParseResult prStmts = parseStmts() ; 
        match(rightCurly) ;
        pr.ast = new CurlyStmts((Stmts*)prStmts.ast) ;
    }
    ///Stmt ::= 'if' '(' Expr ')' Stmt
    ///Stmt ::= 'if' '(' Expr ')' Stmt 'else' Stmt
    else if (attemptMatch(ifKwd)) {
        match(leftParen) ;
        ParseResult e = parseExpr(0) ;
        //printf("%s\n", ((Expr *)e.ast)? "'e' exists":"'e' doesn't exist");
        match(rightParen) ;
        ParseResult p1 = parseStmt() ;
        //printf("%s\n", (Expr*)p1.ast? "'p1' exists":"'p1' doesn't exist");
        
        //If the else keyword is detected...
        if(attemptMatch(elseKwd)) {
            ParseResult p2 = parseStmt() ;
            pr.ast = new IfElseStmt((Expr*)e.ast, (Stmt*)p1.ast, (Stmt*)p2.ast) ;
        }else{
            pr.ast = new IfStmt((Expr*)e.ast, (Stmt*)p1.ast) ;
        }

    }
    ///Stmt ::= varName '=' Expr ';'  | varName '[' Expr ':' Expr ']' '=' Expr ';'
    else if  (attemptMatch(variableName)) {
        if (attemptMatch(leftSquare)) {
            parseExpr(0) ;
            match(colon) ;
            parseExpr(0) ;
            match(rightSquare) ;
        }
        match(assign) ;
        parseExpr(0) ;
        match(semiColon) ;

    }
    ///Stmt ::= 'print' '(' Expr ')' ';'
    else if (attemptMatch(printKwd)) {
        match (leftParen) ;
        parseExpr(0) ;
        match (rightParen) ;
        match (semiColon) ;
    }
    ///Stmt ::= 'repeat' '(' varName '=' Expr 'to' Expr ')' Stmt
    else if (attemptMatch(repeatKwd)) {
        match (leftParen) ;
        match (variableName) ;
        match (assign) ;
        parseExpr (0) ;
        match (toKwd) ;
        parseExpr (0) ;
        match (rightParen) ;
        parseStmt () ;
    }
    ///Stmt ::= 'while' '(' Expr ')' Stmt
    else if (attemptMatch(whileKwd)) {
        match(leftParen) ;
        parseExpr(0) ;
        match(rightParen) ;
        parseStmt() ;
    }
    ///Stmt ::= ';
    else if (attemptMatch(semiColon)) {
        // parsed a skip
    }
    else {
        throw (makeErrorMsg(currToken->terminal) + " while parsing a statement") ;
    }
    /// Stmt ::= variableName assign Expr semiColon
    return pr ;
}

/*! The parse parseStandardDecl function. Takes nothing and returns a ParseResult.
    This is for the declaration of variables to ints, floats, and strings.
    There are many different cases for expression breakup.
*/
/*!\return ParseResult*/
//! Expr
ParseResult Parser::parseExpr (int rbp) {
    /** Examine current token, without consuming it, to call its
       associated parse methods.  The ExtToken objects have 'nud' and
       'led' methods that are dispatchers that call the appropriate
       parse methods.*/
    ParseResult left = currToken->nud() ;
   
    while (rbp < currToken->lbp() ) {
        left = currToken->led(left) ;
    }

    return left ;
}

/**
* parse methods for Expr productions
* ----------------------------------
*/
/*!\return ParseResult*/
/// Expr ::= trueKwd
ParseResult Parser::parseTrueKwd() {
    ParseResult pr ;
    match ( trueKwd ) ;
    return pr ;
}

/// Expr ::= trueKwd
/*!\return ParseResult*/
ParseResult Parser::parseFalseKwd() {
    ParseResult pr ;
    match ( falseKwd ) ;
    return pr ;
}

/// Expr ::= intConst
/*!\return ParseResult*/
ParseResult Parser::parseIntConst() {
    ParseResult pr ;
    match ( intConst ) ;
    return pr ;
}

/// Expr ::= floatConst
/*!\return ParseResult*/
ParseResult Parser::parseFloatConst() {
    ParseResult pr ;
    match ( floatConst ) ;
    return pr ;
}

/// Expr ::= stringConst
/*!\return ParseResult*/
ParseResult Parser::parseStringConst() {
    ParseResult pr ;
    match ( stringConst ) ;
    return pr ;
}

/// Expr ::= variableName .....
/*!\return ParseResult*/
ParseResult Parser::parseVariableName() {
    ParseResult pr ;
    match ( variableName ) ;
    if(attemptMatch(leftSquare)) {
        parseExpr(0) ;
        match(colon) ;
        parseExpr(0) ;
        match(rightSquare) ;
    }
    ///Expr ::= varableName '(' Expr ')'        //NestedOrFunctionCall
    else if(attemptMatch(leftParen)) {
        parseExpr(0) ;
        match(rightParen) ;
    }
    ///Expr := variableName
    else {
        // variable 
    }
    return pr ;
}

/// Expr ::= leftParen Expr rightParen
/*!\return ParseResult*/
ParseResult Parser::parseNestedExpr() {
    ParseResult pr ;
    match ( leftParen ) ;
    parseExpr(0) ; 
    match(rightParen) ;
    return pr ;
}

///Expr ::= 'if' Expr 'then' Expr 'else' Expr
/*!\return ParseResult*/  
ParseResult Parser::parseIfExpr() {  
     ParseResult pr ; 
    
    match(ifKwd) ;
    parseExpr(0) ;
    match(thenKwd) ;
    parseExpr(0) ;
    match(elseKwd) ;
    parseExpr(0) ;

    return pr ;
}

/// Expr ::= 'let' Stmts 'in' Expr 'end' 
/*!\return ParseResult*/
ParseResult Parser::parseLetExpr() {
   ParseResult pr ;
   match(letKwd) ;
   parseStmts() ;
   match(inKwd) ;
   parseExpr(0) ;
   match(endKwd) ;

   return pr ;
}

/// Expr ::= '!' Expr 
/*!\return ParseResult*/
ParseResult Parser::parseNotExpr() {
    ParseResult pr ;
    match(notOp) ;
    parseExpr(0); 
    return pr ;

}

/// Expr ::= Expr plusSign Expr
/*!\return ParseResult \param prLeft The left parse result in the current parent of the AST*/
ParseResult Parser::parseAddition (ParseResult prLeft) {
    // parser has already matched left expression 
    ParseResult pr ;

    Expr* left = NULL ;
    Expr* right = NULL ;

    if (prLeft.ast) {
        left = dynamic_cast<Expr*>(prLeft.ast) ; 
    }
    match (plusSign) ;
    ParseResult prRight = parseExpr(prevToken->lbp());
    if (prRight.ast) {
        right = dynamic_cast<Expr*>(prRight.ast) ; 
    }
    pr.ast = new AdditionExpr(left, right) ;
    
    return pr ;
}

/// Expr ::= Expr star Expr
/*!\return ParseResult \param prLeft The left parse result in the current parent of the AST*/
ParseResult Parser::parseMultiplication ( ParseResult prLeft ) {
    // parser has already matched left expression 
    ParseResult pr ;

    

    match (star) ;
    parseExpr(prevToken->lbp()) ; 
    return pr ;
}

/// Expr ::= Expr dash Expr
/*!\return ParseResult \param prLeft The left parse result in the current parent of the AST*/
ParseResult Parser::parseSubtraction(ParseResult prLeft) {
    // parser has already matched left expression 
    ParseResult pr ;
    match (dash) ;
    parseExpr(prevToken->lbp()) ; 
    return pr ;
}

/// Expr ::= Expr forwardSlash Expr
/*!\return ParseResult \param prLeft The left parse result in the current parent of the AST*/
ParseResult Parser::parseDivision(ParseResult prLeft) {
    // parser has already matched left expression 
    ParseResult pr ;
    match (forwardSlash) ;
    parseExpr(prevToken->lbp()) ; 
    return pr ;
}

/// Expr ::= Expr equalEquals Expr
/*!\return ParseResult \param prLeft The left parse result in the current parent of the AST*/
ParseResult Parser::parseEqualEquals(ParseResult prLeft) {
    // parser has already matched left expression 
    ParseResult pr ;
    match (equalsEquals) ;
    parseExpr(prevToken->lbp()) ; 
    return pr ;
}

/// Expr ::= Expr lessThanEquals Expr
/*!\return ParseResult \param prLeft The left parse result in the current parent of the AST*/
ParseResult Parser::parseLessThanEquals(ParseResult prLeft) {
    // parser has already matched left expression 
    ParseResult pr ;
    match (lessThanEqual) ;
    parseExpr(prevToken->lbp()) ; 
    return pr ;
}

/// Expr ::= Expr greaterThanEquals Expr
/*!\return ParseResult \param prLeft The left parse result in the current parent of the AST*/
ParseResult Parser::parseGreaterThanEquals(ParseResult prLeft) {
    // parser has already matched left expression 
    ParseResult pr ;
    match (greaterThanEqual) ;
    parseExpr(prevToken->lbp()) ; 
    return pr ;
}

/// Expr ::= Expr notEquals Expr
/*!\return ParseResult \param prLeft The left parse result in the current parent of the AST*/
ParseResult Parser::parseNotEquals(ParseResult prLeft) {
    // parser has already matched left expression 
    ParseResult pr ;
    match (notEquals) ;
    parseExpr(prevToken->lbp()) ; 
    return pr ;
}

/// Expr ::= Expr lessThan Expr
/*!\return ParseResult \param prLeft The left parse result in the current parent of the AST*/
ParseResult Parser::parseLessThan (ParseResult prLeft) {
    // parser has already matched left expression 
    ParseResult pr ;
    match (lessThan) ;
    parseExpr(prevToken->lbp()); 
    return pr ;
}

/// Expr ::= Expr greaterThan Expr
/*!\return ParseResult \param prLeft The left parse result in the current parent of the AST*/
ParseResult Parser::parseGreaterThan(ParseResult prLeft) {
    // parser has already matched left expression 
    ParseResult pr ;
    match (greaterThan) ;
    parseExpr(prevToken->lbp()) ; 
    return pr ;
}

/** Notice that for relational operators we use just one parse
   function.  This shows another possible means for implementing
   expressions, as opposed to the method used for arithmetic
   expressions in which each operation has its own parse method.  It
   will depend on what we do in iteration 3 in building an abstract
   syntax tree to decide which method is better.
 */

/*! The parseRelationalExpr function. Takes a ParseResult and returns a ParseResult.
    This is for the expressions requiring only one parse function.
*/
/*!\return ParseResult \param prLeft The left parse result in the current parent of the AST*/
ParseResult Parser::parseRelationalExpr(ParseResult prLeft) {
    // parser has already matched left expression 
    ParseResult pr ;

    nextToken() ;
    // just advance token, since examining it in parseExpr caused
    // this method being called.
    string op = prevToken->lexeme ;

    parseExpr(prevToken->lbp()) ; 
    return pr ;
}

//! Helper function used by the parser.
/*!\param tt The input tokenType we're trying to match with*/
void Parser::match(tokenType tt) {
    if (!attemptMatch(tt)) {
        throw(makeErrorMsgExpected(tt)) ;
    }
}

/*!\return bool \param tt The input tokenType we're trying to match with*/
bool Parser::attemptMatch (tokenType tt) {
    if (currToken->terminal == tt) { 
    //cout << currToken->lexeme << " " << currToken->terminal << "\n" ;
        nextToken() ;
        return true ;
    }
    return false ;
}

/*!\return bool \param tt The input token that we're matching with*/
bool Parser::nextIs (tokenType tt) {
    return currToken->terminal == tt ;
}

/*!Throws an error if nextToken is in an uninitialized state*/
void Parser::nextToken () {
    if (currToken == NULL) 
        throw(string("Internal Error: should not call nextToken in unitialized state")) ;
    else 
    if (currToken->terminal == endOfFile && currToken->next == NULL) {
        prevToken = currToken ;
    } else if (currToken->terminal != endOfFile && currToken->next == NULL) {
        throw(makeErrorMsg("Error: tokens end with endOfFile")) ;
    } else {
        prevToken = currToken ;
        currToken = currToken->next ;
    }
}

/*!\return string \param terminal The terminal that we're describing*/
string Parser::terminalDescription(tokenType terminal) {
    Token *dummyToken = new Token("",terminal, NULL) ;
    ExtToken *dummyExtToken = extendToken(this, dummyToken) ;
    string s = dummyExtToken->description() ;
    delete dummyToken ;
    delete dummyExtToken ;
    return s ;
}

/*!\return string \param terminal The terminal for the error output*/
string Parser::makeErrorMsgExpected(tokenType terminal) {
    string s = (string) "Expected " + terminalDescription(terminal) +
        " but found " + currToken->description() ;
    return s ;
}

/*!\return string \param terminal The terminal that we're describing (that wasn't expected)*/
string Parser::makeErrorMsg(tokenType terminal) {
    string s = "Unexpected symbol " + terminalDescription(terminal) ;
    return s ;
}

/*!\return string \param msg The message to return for thrown errors*/
string Parser::makeErrorMsg(const char *msg) {
    return msg ;
}
