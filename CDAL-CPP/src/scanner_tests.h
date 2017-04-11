#include <cxxtest/TestSuite.h>

#include "readInput.h"
#include "scanner.h"
#include "regex.h"
#include <stdio.h>
#define stringify
#include <typeinfo>
using namespace std ;

class ScannerTestSuite : public CxxTest::TestSuite {
    public:
        /** A Scanner object is created in the test_setup_code method.
           This is a test, beginning with "test_", so that it is executed
           by the testing framework.  The scanner is used in some tests of
           the method "scan".
         */
        Scanner *s ;
        void test_setup_code ( ) {
            s = new Scanner() ;
    }


    /// Tests for components and functions used by "scan"
    // --------------------------------------------------

    /* You should test the regular expressions you use to make and match
       regular expressions,and the functions used by makeRegex and
       matchRegex using regex_tests.h.
       However, You will likely need to write several tests to
       adequately test  the functions that are called from "scan".
       Once you are confident that the components used by "scan"
       work properly, then you can run tests on the "scan" method itself.
    */

    // You should place tests for these other functions used by
    // scan below.

    /**
    This single function tests every regex match case we've made
    to ensure we're matching the right things in the real cases.
    */
    void test_regex_terminals ( ) {
        TS_ASSERT (s->scan("bool")->terminal == boolKwd) ;
        TS_ASSERT (s->scan("true")->terminal == trueKwd) ;
        TS_ASSERT (s->scan("false")->terminal == falseKwd) ;
        TS_ASSERT (s->scan("while")->terminal == whileKwd) ;
        TS_ASSERT (s->scan("(")->terminal == leftParen) ;
        TS_ASSERT (s->scan(")")->terminal == rightParen) ;
        TS_ASSERT (s->scan("{")->terminal == leftCurly) ;
        TS_ASSERT (s->scan("}")->terminal == rightCurly) ;
        TS_ASSERT (s->scan("[")->terminal == leftSquare) ;
        TS_ASSERT (s->scan("]")->terminal == rightSquare) ;
        TS_ASSERT (s->scan(";")->terminal == semiColon) ;
        TS_ASSERT (s->scan(":")->terminal == colon) ;
        TS_ASSERT (s->scan("int")->terminal == intKwd) ;
        TS_ASSERT (s->scan("float")->terminal == floatKwd) ;
        TS_ASSERT (s->scan("string")->terminal == stringKwd) ;
        TS_ASSERT (s->scan("matrix")->terminal == matrixKwd) ;
        TS_ASSERT (s->scan("let")->terminal == letKwd) ;
        TS_ASSERT (s->scan("in")->terminal == inKwd) ;
        TS_ASSERT (s->scan("end")->terminal == endKwd) ;
        TS_ASSERT (s->scan("if")->terminal == ifKwd) ;
        TS_ASSERT (s->scan("then")->terminal == thenKwd) ;
        TS_ASSERT (s->scan("else")->terminal == elseKwd) ;
        TS_ASSERT (s->scan("repeat")->terminal == repeatKwd) ;
        TS_ASSERT (s->scan("print")->terminal == printKwd) ;
        TS_ASSERT (s->scan("to")->terminal == toKwd) ;
        TS_ASSERT (s->scan("64")->terminal == intConst) ;
        TS_ASSERT (s->scan("64.0")->terminal == floatConst) ;
        TS_ASSERT (s->scan("\"ello\"")->terminal == stringConst) ;
        TS_ASSERT (s->scan(":=")->terminal == assign) ;
        TS_ASSERT (s->scan("+")->terminal == plusSign) ;
        TS_ASSERT (s->scan("*")->terminal == star) ;
        TS_ASSERT (s->scan("-")->terminal == dash) ;
        TS_ASSERT (s->scan("/")->terminal == forwardSlash) ;
        TS_ASSERT (s->scan("<")->terminal == lessThan) ;
        TS_ASSERT (s->scan("<=")->terminal == lessThanEqual) ;
        TS_ASSERT (s->scan(">")->terminal == greaterThan) ;
        TS_ASSERT (s->scan(">=")->terminal == greaterThanEqual) ;
        TS_ASSERT (s->scan("==")->terminal == equalsEquals) ;
        TS_ASSERT (s->scan("!=")->terminal == notEquals) ;
        TS_ASSERT (s->scan("&&")->terminal == andOp) ;
        TS_ASSERT (s->scan("||")->terminal == orOp) ;
        TS_ASSERT (s->scan("!")->terminal == notOp) ;
        TS_ASSERT (s->scan("")->terminal == endOfFile) ;
    }

    /* Below is one of the tests for these components in the project
       solution created by your instructor.  It uses a helper
       function function called "tokenMaker_tester", which you have
       not been given.  You are expected to design your own components
       for "scan" and your own mechanisms for easily testing them.

        void xtest_TokenMaker_leftCurly ( ) {
            tokenMaker_tester ("{ ", "^\\{", leftCurly, "{" ) ;
        }

        Note that this test is here named "xtest_Token..." The leading
        "x" is so that cxxTest doesn't scan the line above and think
        it is an actual test that isn't commented out.  cxxTest is
        very simple and doesn't even process block comments.
    */


    /* You must have at least one separate test case for each terminal
       symbol.  Thus, you need a test that will pass or fail based
       solely on the regular expression (and its corresponding code)
       for each terminal symbol.

       This requires a test case for each element of the enumerated
       type tokenType.  This may look something like the sample test
       shown in the comment above.
    */



    /// Tests for "scan"
    // --------------------------------------------------

    /** Below are some helper functions and sample tests for testing the
       "scan" method on Scanner.

    bool tokenMaker_tester (string token, string pattern){
        if matchRegex();
    }
    */

    /// Test that a list of tokens has no lexicalError tokens.
    bool noLexicalErrors (Token *tks) {
        Token *currentToken = tks ;
        while (currentToken != NULL) {
            if (currentToken->terminal == lexicalError) {
                printf("problem: %s\n",currentToken->lexeme.c_str()) ;
                return false ;
            }
            else {
                currentToken = currentToken->next ;
            }
        }
        return true ;
    }

    /** A quick, but inaccurate, test for scanning files.  It only
       checks that no lexical errors occurred, not that the right
       tokens were returned.
    */
    void scanFileNoLexicalErrors (const char* filename) {
        char *text =  readInputFromFile (filename)  ;
        TS_ASSERT (text) ;
        Token *tks = s->scan (text) ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT (noLexicalErrors(tks)) ;
    }

    /** This function checks that the terminal fields in the list of
       tokens matches a list of terminals.
    */
    bool sameTerminals (Token *tks, int numTerms, tokenType *ts) {
        Token *currentToken = tks ;
        int termIndex = 0 ;
        while (currentToken != NULL && termIndex < numTerms) {
            //printf("%i: %i is matched with %i\n",termIndex,currentToken->terminal , ts[termIndex]);
            if (currentToken->terminal != ts[termIndex]) {
            printf("%i: %i should be %i\n",termIndex,currentToken->terminal , ts[termIndex]);
            fflush(stdout) ;
                return false ;
            }
            else {
                ++ termIndex ;
                currentToken = currentToken->next ;
            }
        }
        return true ;
    }


    /** Below are the provided test files that your code should also
       pass.

       You may initially want to rename these tests to "xtest_..." so
       that the CxxTest framework does not see it as a test and won't
       run it as one.  This way you can focus on the tests that you'll
       write above for the individual terminal types first. Then focus
       on these tests.

    */

    /// The "endOfFile" token is always the last one in the list of tokens.
    void test_scan_empty ( ) {
        Token *tks = s->scan ("  ") ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
        TS_ASSERT (tks->next == NULL) ;
    }

    /// tests a string consisting of only comments and 
    void test_scan_empty_comment ( ) {
        Token *tks = s->scan (" /* a comment */ ") ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
        TS_ASSERT (tks->next == NULL) ;
    }

    /// When a lexical error occurs, the scanner creates a token with a
    /// single-character lexeme and lexicalError as the terminal.
   void test_scan_lexicalErrors ( ) {
        Token *tks = s->scan ("$&1  ") ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, lexicalError) ;
        TS_ASSERT_EQUALS (tks->lexeme, "$") ;

        tks = tks->next ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, lexicalError) ;
        TS_ASSERT_EQUALS (tks->lexeme, "&") ;
        tks = tks->next ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, intConst) ;
        TS_ASSERT_EQUALS (tks->lexeme, "1") ;
        tks = tks->next ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
        TS_ASSERT (tks->next == NULL) ;
    }


    /// A test for scanning numbers and a variable.
    void test_scan_nums_vars ( ) {
        Token *tks = s->scan (" 123 x 12.34 ") ;
        TS_ASSERT (tks != NULL) ;
        tokenType ts[] = { intConst, variableName, floatConst, endOfFile } ;
        TS_ASSERT (sameTerminals(tks, 4, ts)) ;
    }



    /** This test checks that the scanner returns a list of tokens with
       the correct terminal fields.  It doesn't check that the lexemes
       are correct.
     */

    void test_scan_bad_syntax_good_tokens ( ) {
        const char *filename = "../samples/bad_syntax_good_tokens.dsl" ;
        char *text =  readInputFromFile (filename)  ;
        TS_ASSERT (text) ;
        Token *tks = s->scan (text) ;
        TS_ASSERT (tks != NULL) ;
        tokenType ts[] = { 
            intConst, intConst, intConst, intConst, 

            stringConst, stringConst, stringConst,

            floatConst, floatConst, floatConst,

                    matrixKwd,



            semiColon, colon,
            leftCurly, leftParen, rightCurly, rightParen,

            plusSign, star, dash, forwardSlash, 

            equalsEquals, lessThanEqual, 
            greaterThanEqual, notEquals, 
            assign,

            variableName, variableName, variableName, variableName, 
            variableName, variableName, variableName,


            intKwd, floatKwd,  stringKwd, 

            endOfFile
       } ;
        int count = 38;
        TS_ASSERT (sameTerminals(tks, count, ts )) ;
    }


    /// tests the forestloss sample file provided by the instructor
    void test_scan_sample_forestLoss ( ) {
        scanFileNoLexicalErrors ("../samples/forest_loss_v2.dsl") ;
    }
} ;
