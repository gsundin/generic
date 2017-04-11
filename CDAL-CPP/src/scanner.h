 /* A small meaningless comment */
#ifndef SCANNER_H
#define SCANNER_H

#include <regex.h>
#include <string>

using namespace std ;

/* This enumerated type is used to keep track of what kind of
   construct was matched.
 */

enum tokenEnumType {

	intKwd, floatKwd, boolKwd,
	trueKwd, falseKwd, whileKwd,
	stringKwd, matrixKwd,
	letKwd, inKwd, endKwd, ifKwd, thenKwd, elseKwd,
	repeatKwd,  printKwd, toKwd, //0-16

	// Constants
	intConst, floatConst, stringConst, //17-19

	// Names
	variableName , //20

	// Punctuation
	leftParen, rightParen,
	leftCurly, rightCurly,
	leftSquare, rightSquare,

	semiColon, colon, //21-28

	//Operators
	assign,
	plusSign, star, dash, forwardSlash,
	lessThan, lessThanEqual, greaterThan, greaterThanEqual,
	equalsEquals, notEquals,
	andOp, orOp, notOp, //29-42

	// Special terminal types
	endOfFile, //43
	lexicalError //44
} ;

typedef enum tokenEnumType tokenType ;

// Below you need to write your class definitions for Token and Scanner.
class Token {
	public:
	  std::string lexeme ;
	  tokenType terminal ;
	  Token* next ;
	  Token() ;
	  ~Token() ;
	  Token(std::string lexeme, tokenType terminal, Token* next) ;

	  int row, col ;
} ;

class Info {
	public:
		regex_t* regex ; //the makeRegex statement
		tokenType terminal ; //the terminal of the token
		Info(regex_t*, tokenType) ;
} ;

class Scanner {
	//scanner
	public:
		Token *scan (const char *) ;
} ;

#endif /* SCANNER_H */
