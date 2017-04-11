/// zhan2736 and sundi024
#include "scanner.h"
#include <string>
#include <regex.h>
#include "regex.h"
#include <stdio.h>
#include <string.h>

using namespace std ;

/// initializes all regexes as Info objects, to be checked in a later function
Info allTokens[45] = {
		Info(makeRegex("^int"), intKwd),
		Info(makeRegex("^float"), floatKwd),
		Info(makeRegex("^bool"), boolKwd),
		Info(makeRegex("^true"), trueKwd),
		Info(makeRegex("^false"), falseKwd),
		Info(makeRegex("^while"), whileKwd),
		Info(makeRegex("^string"), stringKwd),
		Info(makeRegex("^matrix$"), matrixKwd),
		Info(makeRegex("^let$"), letKwd),
		Info(makeRegex("^in$"), inKwd),
		Info(makeRegex("^end$"), endKwd),
		Info(makeRegex("^if$"), ifKwd),
		Info(makeRegex("^then"), thenKwd),
		Info(makeRegex("^else"), elseKwd),
		Info(makeRegex("^repeat"), repeatKwd),
		Info(makeRegex("^print$"), printKwd),
		Info(makeRegex("^to"), toKwd),
		Info(makeRegex("^[(]"), leftParen),
		Info(makeRegex("^[)]"), rightParen),
		Info(makeRegex("[{]"), leftCurly),
		Info(makeRegex("^[}]"), rightCurly),
		Info(makeRegex("^[[]"), leftSquare),
		Info(makeRegex("^[]]"), rightSquare),
		Info(makeRegex("^[;]"), semiColon),
		Info(makeRegex(":="), assign),
		Info(makeRegex("[:]"), colon),
		Info(makeRegex("^[+]"), plusSign),
		Info(makeRegex("^[*]"), star),
		Info(makeRegex("^[-]"), dash),
		Info(makeRegex("^[/]"), forwardSlash),
		Info(makeRegex("<="), lessThanEqual),
		Info(makeRegex("^[<]"), lessThan),
		Info(makeRegex(">="), greaterThanEqual),
		Info(makeRegex("^[>]"), greaterThan),
		Info(makeRegex("=="), equalsEquals),
		Info(makeRegex("!="), notEquals),
		Info(makeRegex("&&"), andOp),
		Info(makeRegex("[||]"), orOp),
		Info(makeRegex("^[!]"), notOp),
		Info(makeRegex("-?[0-9]+\\.[0-9]+"), floatConst),
		Info(makeRegex("^-?([0-9]+)"), intConst),
		//Info(makeRegex("^\"([^\"]|(\\\\\"))*\""), stringConst),
		Info(makeRegex("^\".*\"$"), stringConst),
		Info(makeRegex("[A-Za-z]+"), variableName),
		Info(makeRegex("^[=]"), assign),
		Info(makeRegex("\0"), endOfFile)
	} ;

/// constructor for the Info object, requires parameters
Info::Info (regex_t* inputRegex, tokenType inputTerminal) {
	this->regex = inputRegex ; //initializes the regex as the input
	this->terminal = inputTerminal ; //initializes the terminal as the input
}

/// blank constructor to initialize as "new Token()"
Token::Token ( ) {
	this->lexeme = "" ; //initializes the lexeme as an empty std::string
	this->terminal = endOfFile ;  //initializes the terminal as endOfFile
	this->next = NULL ; //initializes the next Token as NULL
}

/// blank deconstructor to destroy Token objects
Token::~Token() {
}

/// overloads the blank constructor so specific parameters can be input
Token::Token (std::string lexeme, tokenType terminal, Token* next) {
	this->lexeme = lexeme ; //initializes the lexeme
	this->terminal = terminal ; //initializes the terminal
	this->next = next ; //initializes the next Token
}

/// matches the values in the global array and returns the enumerated terminal
tokenEnumType matchWith (char* lexeme) {
	for (int i = 0; i < 45; i++){
		if(matchRegex(allTokens[i].regex, lexeme) > 0) {
			return allTokens[i].terminal ;
		}
	}
	return lexicalError ;
}

/// helper function to delete first character of the input string
char* CutFirst (std::string input) {
	int newSize = strlen(input.c_str()) - 1 ;
	char * output = new char[newSize] ;
	for(int i = 1 ; i < newSize ; i++){
		output[i - 1] = input[i] ;
	}

	return output ;
}

/**
This method inserts spaces between all of the single-character regexes
in order to separate them for parsing. It also checks to ensure that the
single-character regexes are not part of a word (variable or string). This
is a countermeasure to fix the issues with parsing single characters.
*/
char* clean(std::string input) {
	std::string currentString ;
	bool suffix = false ;
	bool comment = false ;

	for(int i=0; input[i] != '\0'; i++) {
		suffix = false ;
		char a = input[i] ;
		char b = input[i - 1] ;
		char c = input[i + 1] ;
		char d = input[i + 2] ;
		char e = input[i - 2] ;

		if(i > 0) {
			if(comment && a != '\n') {
				continue ;
			} else {
				comment = false ;

				if (a == '(' || a == ')'|| a == '{' || a == '}' || a == '[' || a == ']' || a == ':' || a == '+' || a == '<' || a == '>' || a == '!' || a == ';') {
					if (b != ' ') {
						currentString += ' ' ;
					}
				}

				if((b == '[' && isdigit(a)) || (b == '(' && c == '\"' && d == '.')) {
					currentString+= ' ' ;
				}

				if (isalpha(a) && !isalpha(b) && a != ' ' && b != '\"' && b != '-' && b != '.' && b != '/' && b != '_') {
					//case in which return char
					if(a == 'n' && c =='\"' && e=='\"'){

					}else{
						currentString += ' ' ;	
					}
					
				}

				if (((isalnum(b) || b == ')') && a == '*') || (isalnum(a) && b == '+')) {
				    currentString += ' ' ;
				}

				if ((isalnum(a) && b == '-' && a != 'l')  || (isalnum(b) && a == '-' && b != 'i')) {
			        currentString += ' ' ;
				}

				if(a == '\"' && b == '(' && d == 'n'){
					currentString += ' ' ;
				}

				/*This part is to fix the spacing on ("\n") portions*/
				//if (b == '(' && a == '\"' && c == '\\' && d == 'n'){
				//    currentString+= ' ' ;
				//}

				if (a == ':') {
					if(b != ' ') {
						currentString += ' ' ;
					}
					if(c != '=') {
						suffix = true ;
					}
				}

				if(a == '(' && isalnum(c)) {
					suffix = true ;
				}

	            if (a == ' ' && b == '(' && c == '\"') {
	                continue ;
	            }

	            if(a == '/' && c == '/') {
	            	comment = true ;
	            	continue ;
	            }
	        }
		}
		currentString += a ;

		if(suffix) {
			currentString += ' ' ;
		}

	}
	currentString += '\0' ;
	const char* currentStringConst = currentString.c_str() ;
	int outputSize = strlen(currentStringConst) ;

	char* outputString = new char[outputSize] ;
	strcpy(outputString, currentStringConst) ;

	return outputString ;
}

/**
This is the main method of scanner.cpp. The scan function takes in
a text file as a large string, converts it into 1..n words delimited
by blank spaces and block comments, and matches those words with the
global regex array to create Token objects.
*/

Token* Scanner::scan (const char* input) {

	/// initialization of the reused variables
	Token* headToken = new Token() ;
	Token* currentToken = new Token() ;
	char * spaceDelimitedString = NULL ;
	headToken = currentToken ;
	int inputSize = strlen(input) + 1 ;

	/// currentTerminal is initially set to intKwd arbitrarily
	tokenEnumType currentTerminal = intKwd ;

	/// if is empty, return an endOfFile token
	if (input[0] == '\0'){
		headToken->lexeme = std::string(input, inputSize) ;
		headToken->terminal = endOfFile ;
		return headToken ;
	}

	/// convert the input from char* to char[]
	char* inputString = clean(input);

	/// creates the string array, delimited by spaces and new lines
	spaceDelimitedString = strtok (inputString, " \n\t") ;

	/// while spaceDelimitedString still has strings, return a pointer to them
	while (spaceDelimitedString != NULL) {
		std::string currentString(spaceDelimitedString, strlen(spaceDelimitedString)) ;

		/// check if there's a block comment initiation
        if (strcmp("/*", spaceDelimitedString) == 0) {
            spaceDelimitedString = strtok(NULL, " \n\t") ;

            while(spaceDelimitedString != NULL) {
                if(strcmp("*/", spaceDelimitedString) == 0) {
                    spaceDelimitedString = strtok(NULL, " \n\t") ;
                    break;
                }
                else {
                    spaceDelimitedString = strtok(NULL, " \n\t") ;
                }
            }
            continue ;
        }

         if (strcmp("//", spaceDelimitedString) == 0) {
            spaceDelimitedString = strtok(NULL, " \n\t") ;

            while(spaceDelimitedString != NULL) {
                if(strcmp("\n", spaceDelimitedString) == 0) {
                    spaceDelimitedString = strtok(NULL, " \n\t") ;
                    break;
                }
                else {
                    spaceDelimitedString = strtok(NULL, " \n\t") ;
                }
            }
            continue ;
        }

        /// set the current terminal and lexeme because they aren't comments or white space
		currentToken->lexeme = currentString ;
		currentToken->terminal = matchWith(spaceDelimitedString) ;


		 currentTerminal = currentToken->terminal ;


		/// the lexeme of the token should be blank if the terminal is a lexical error
		if (currentToken->terminal == lexicalError) {
			int lexicalErrorLoopCounter = 1 ;
			currentToken->lexeme = currentString[0] ;
			Token* nextToken = new Token() ;
			currentToken->next = nextToken ;
			currentToken = nextToken ;
			int partialStringSize = strlen(spaceDelimitedString) ;

			/// splits the remaining string into individual tokens via delimiting by spaces and Token types
			while(lexicalErrorLoopCounter < partialStringSize) {
				std::string newString = currentString.substr(lexicalErrorLoopCounter) ;
				currentToken->lexeme = newString ;
				char* currentChar = new char[inputSize] ;
				strcpy(currentChar, newString.c_str()) ;
				currentToken->terminal = matchWith(currentChar) ;

				/// if the terminal is a lexical error, set the current Token to reflect it
				if(currentToken->terminal == lexicalError){
					currentToken->lexeme = currentChar[0] ;
					Token* nextToken = new Token() ;
					currentToken->next = nextToken ;
					currentToken = nextToken ;
				}
				/// otherwise, break out of the while loop
				else {
					break ;
				}
				lexicalErrorLoopCounter++ ;
			}
		}

    	spaceDelimitedString = strtok (NULL, " \n") ;

	/// declares nextToken as a blank token each iteration of the while loop
	Token* nextToken = new Token() ;

    currentToken->next = nextToken ;
    currentToken = nextToken ;
  }

	currentToken->lexeme = "" ;
	currentToken->terminal = endOfFile ;

  return headToken ;
}
