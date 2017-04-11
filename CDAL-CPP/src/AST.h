#ifndef Node_H
#define Node_H

#include <string.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include "scanner.h"

using namespace std;

class Node ;

/*!  This is the base class for Node objects. Nothing is
derived from this class yet.
*/
class Node {
	public:
		virtual std::string unparse() { 
			return "This should be pure virtual" ;
		} ;

		virtual std::string cppCode() { 
			return "This should be pure virtual" ;
		} ;

		virtual ~Node() { }
		//! deconstructor for the Node object
} ;

/*!  This is the base class for Decl objects. It is
	 not derived from anything and all Decl objects
	 are derived from it.
*/
class Decl: public Node { };

/*!  This is the base class for Stmt objects. It is
	 not derived from anything and all Stmt objects
	 are derived from it.
*/
class Stmt: public Node { } ;

/*!  This is the base class for Expr objects. All Expr-
	 suffixed objects are derived from this class.
*/
class Expr: public Node { } ;

/*! VariableName is used to instantiate the strings
	detected as variable names.
*/
class VariableName: public Expr {
	public:
		VariableName(std::string v) {
			varName = v ;
		}

		std::string unparse() {
			return varName ;
		}
		
	private:
		std::string varName ;

		VariableName(const VariableName &) {};
} ;

/*!  This is the base class for MatrixDecl objects. It is
	 derived from Decl but nothing is derived from it yet.
*/
class MatrixDecl: public Decl {
	public:
		MatrixDecl(VariableName* vn, Expr* e1, Expr* e2, Expr* e3) {
			this->vn = vn ;
			this->e1 = e1 ;
			this->e2 = e2 ;
			this->e3 = e3 ;
		}

	private:
		VariableName* vn ;
		Expr* e1 ;
		Expr* e2 ;
		Expr* e3 ;
} ;

/*!  This is the base class for StandardDecl objects. It is
	 derived from Decl but nothing is derived from it yet.
	 It reads the declarations for standard 
*/
class StandardDecl: public Decl {
	public:
		StandardDecl(std::string str1, std::string str2) {
			s1 = str1 ;
			s2 = str2 ;
		}
		std::string unparse() {
        	std::string s ;
        	s = s1 + s2 + ";" ;
        	return s ;
    }
	private:
		std::string s1 ;
		std::string s2 ;
} ;

/*!  This is the base class for IntDecl objects. It is
	 derived from Decl but nothing is derived from it yet.
*/
class IntDecl: public Decl {
	public:
		IntDecl(VariableName varName) { }
		std::string unparse() {
			std::stringstream ss ;
			ss << " int " << varName << " ; " ;
			return ss.str() ;
		}
	private:
		VariableName* varName ;
} ;

/*!  This is the base class for FloatDecl objects. It is
	 derived from Decl but nothing is derived from it yet.
*/
class FloatDecl: public Decl {
	public:
		FloatDecl(VariableName varName) { }
		std::string unparse() {
			std::stringstream ss ;
			ss << " float " << varName << " ;" ;
			return ss.str() ;
		}
	private:
		VariableName* varName ;
} ;

/*!  This is the base class for StringDecl objects. It is
	 derived from Decl but nothing is derived from it yet.
*/
class StringDecl: public Decl {
	public:
		StringDecl(VariableName varName) { }
		std::string unparse() {
			std::stringstream ss ;
			ss << " string " << varName << " ; " ;
			return ss.str() ;
		}
	private:
		VariableName* varName ;
} ;


/*!  This is the base class for IfStmt objects. It is
	 derived from the Stmt object.
*/
class IfStmt: public Stmt {
	public:
		IfStmt(Expr* e, Stmt* s) {
			expr = e ;
			stmt = s ;
		}

	private:
		Expr* expr ;
		Stmt* stmt ;
		
} ;

/*!  This is the base class for IfElseStmt objects. It is
	 derived from the Stmt object.
*/
class IfElseStmt: public Stmt {
	public:
		IfElseStmt(Expr* e, Stmt* s1, Stmt* s2) {
			expr = e ;
			stmt1 = s1 ;
			stmt2 = s2 ;
		}

	private:
		Expr* expr ;
		Stmt* stmt1 ;
		Stmt* stmt2 ;
		
} ;

/*!  This is the base class for RepeatStmt objects. It is
	 derived from the Stmt object.
*/
class RepeatStmt: public Stmt {	
	public: 
		RepeatStmt(VariableName* v, Expr* e1, Expr* e2, Stmt* s) {
			vn = v ;
			expr1 = e1 ;
			expr2 = e2 ;
			stmt = s ;
		}

	private:
		VariableName* vn ;
		Expr* expr1 ;
		Expr* expr2 ;
		Stmt* stmt ;
} ;

/*!  This is the base class for Stmts objects. It is
	 derived from the Node class, and is the basis for all stmts.
*/
class Stmts: public Node { } ;

/*!  This is the base class for CurlyStmts objects. It is
	 derived from Stmts but nothing is derived from it yet.
*/
class CurlyStmts : public Stmts {
	public:
		CurlyStmts(Stmts*  s) {
			sts = s ;
		}

		std::string unparse() {
			std::stringstream ss ;
			ss << "{ " << sts->unparse() << " }" ;
			return ss.str() ;
		}

	private:
		Stmts* sts ;
} ;

/*!  This is the base class for EmptyStmts. It is derived from the 
	 Stmts class.
*/
class EmptyStmts: public Stmts {
	public:
		EmptyStmts() { }

		std::string unparse() { return "" ;}
} ;

/*!  This is the base class for TwoStmts. It is derived from Stmts.
 	 It is how we took in stmts and stmt connected
*/
class TwoStmts: public Stmts {
	public:

	TwoStmts(Stmt*  s, Stmts*  ss) {
    	this->s1 = s1 ;
    	this->s2 = s2 ;
  	}

	std::string unparse() {
  		std::string str ;
  		//printf("%s\n", (!ss) ? "Empty": "Filled");
    	return str ;
  	}

	private:
		Stmt* s1 ;
  		Stmts* s2 ;
} ;

/*!  This is the base class for PrintStmt. It is derived from Stmts
	 but nothing is derived from it yet.
*/
class PrintStmt: public Stmts {
	public:	
		PrintStmt(Stmt* s) {
			this->s = s ;
		}

	private:
		Stmt* s ;
} ;

/*!  This is the base class for semiColonStmt objects, it is derived from Stmt.
	 It handles a stmt with only a semicolon.
*/
class semiColonStmt: public Stmt {
	public:
		semiColonStmt() { }

		std::string unparse() {
			std::string s ;
			s = " ;" ;
			return s ;
		}
} ;

/*!  Program is the root and the base class for the Program start 
	 and is the beginning for the entire program. It is derived 
	 from the Node class.
*/
class Program: public Node {
	public:
		Program(VariableName* v, Stmts* s): varName(v), stmts(s) { } ;

		std::string unparse() {
			return varName->unparse() + "() {" + stmts->unparse() + "}" ;
		}

	private:
		VariableName* varName ;
		Stmts* stmts ;

		Program(): varName(NULL), stmts(NULL) { } ;

		Program(const Program &) { } ;
} ;

/*!  This is the base class for let expressions and LetExpr objects.
	 It is derived from the Expr class.
*/
class LetExpr: public Expr {
 	public:
		LetExpr(Stmts* s, Expr* e) {
	    	stmts = s ;
	    	ex = e ;
		}

	private:
		Stmts* stmts ;
		Expr* ex ;
	} ;

/*!  This is the base class for Not Expression, and NotExpr objects.
	 It is derived from the Expr class.
*/

class NotExpr: public Expr {
	public:
		NotExpr(Expr* e) {
			ex = e ;
		}
	private:	
		Expr* ex ;
} ;

/*!  This is the base class for the Addition Expressions and the 
	 AdditionExpr object.
*/
class AdditionExpr: public Expr {
	public:
		AdditionExpr(Expr* e1, Expr* e2) {
			this->e1 = e1 ;
			this->e2 = e2 ;
		}

	private:
		Expr* e1 ;
		Expr* e2 ;
} ;
/*!  This is the base class for the Subtraction Expressions and the SubtractionExpr object.
*/
class SubtractionExpr: public Expr {
	public:
		SubtractionExpr(Expr* e1, Expr* e2) {
			this->e1 = e1 ;
			this->e2 = e2 ;
		}

	private:
		Expr* e1 ;
		Expr* e2 ;
} ;

/*!  This is the base class for the Multiplication Expressions and the MultExpr object.
It is derived from the Expr class.
*/
class MultExpr: public Expr {
	public:
		MultExpr(Expr* e1, Expr* e2) {
			this->e1 = e1 ;
			this->e2 = e2 ;
		}

	private:
		Expr* e1 ;
		Expr* e2 ;
} ;
/*!  This is the base class for the Division Expressions and the DivExpr object.
It is derived from the Expr class.
*/
class DivExpr: public Expr {
	public:
		DivExpr(Expr* e1, Expr* e2) {
			this->e1 = e1 ;
			this->e2 = e2 ;
		}

	private:
		Expr* e1 ;
		Expr* e2 ;
} ;

/*!  This is the base class for the Greater Than Expressions and the GreaterExpr object.
It is derived from the Expr class.
*/
class GreaterExpr: public Expr {
	public:
		GreaterExpr(Expr* e1, Expr* e2) {
			this->e1 = e1 ;
			this->e2 = e2 ;
		}

	private:
		Expr* e1 ;
		Expr* e2 ;
} ;

/*!  This is the base class for the Greater Equals Expressions and the GreaterEqExpr object.
It is derived from the Expr class.
*/
class GreaterEqExpr: public Expr {
	public:
		GreaterEqExpr(Expr* e1, Expr* e2) {
			this->e1 = e1 ;
			this->e2 = e2 ;
		}

	private:
		Expr* e1 ;
		Expr* e2 ;
} ;

/*!  This is the base class for the Lesser than Expressions and the LesserExpr object.
It is derived from the Expr class.
*/
class LesserExpr: public Expr {
	public:
		LesserExpr(Expr* e1, Expr* e2) {
			this->e1 = e1 ;
			this->e2 = e2 ;
		}

	private:
		Expr* e1 ;
		Expr* e2 ;
} ;

/*!  This is the base class for the Lesser than Equal Expressions and 
	 the LesserEqExpr object. It is derived from the Expr class.
*/
class LesserEqExpr: public Expr {
	public:
		LesserEqExpr(Expr* e1, Expr* e2) {
			this->e1 = e1 ;
			this->e2 = e2 ;
		}

	private:
		Expr* e1 ;
		Expr* e2 ;
} ;

/*!  This is the base class for the EqualEqual Expressions and the 
	 EqEqExpr object. It is derived from the Expr class.
*/
class EqEqExpr: public Expr {
	public:
		EqEqExpr(Expr* e1, Expr* e2) {
			this->e1 = e1 ;
			this->e2 = e2 ;
		}

	private:
		Expr* e1 ;
		Expr* e2 ;
} ;

/*!  This is the base class for the Not Equal Expressions and the 
	 NotEqExpr object. It is derived from the Expr class.
*/
class NotEqExpr: public Expr {
	public:
		NotEqExpr(Expr* e1, Expr* e2) { 
			this->e1 = e1 ;
			this->e2 = e2 ;
		}

	private:
		Expr* e1 ;
		Expr* e2 ;
} ;

/*!  This is the base class for the And Expressions and the AndExpr 
	 object. It is derived from the Expr class.
*/
class AndExpr: public Expr {
	public:
		AndExpr(Expr* e1, Expr* e2) {
			this->e1 = e1 ;
			this->e2 = e2 ;
		}

	private:
		Expr* e1 ;
		Expr* e2 ;
} ;

/*!  This is the base class for the Or Expressions and the OrExpr object.
	 It is derived from the Expr class.
*/
class OrExpr: public Expr {
	public:
		OrExpr(Expr* e1, Expr* e2) {
			this->e1 = e1 ;
			this->e2 = e2 ;
		}

	private:
		Expr* e1 ;
		Expr* e2 ;
} ;

/*!  This is the base class for the Integer Statements and the IntegerStmt 
	 object. It is derived from the Stmt class.
*/
class IntegerStmt: public Stmt {
	public:
		IntegerStmt(int i) {
			this->i = i ;
		}

	private:
		int i ;
} ;

/*!  This is the base class for the Float Statements and the FloatStmt 
	 object. It is derived from the Stmt class.
*/
class FloatStmt: public Stmt {
	public:
		FloatStmt(float f) {
			this->f = f ;
		}
	
	private:
		float f ;
} ;

/*!  This is the base class for the String Statements and the StringStmt 
	 object. It is derived from the Stmt class.
*/
class StringStmt: public Stmt {
	public:
		StringStmt(std::string s) {
			this->s = s ;
		}
		
	private:
		std::string s ;
} ;

/*!  This is the base class for the True Statements and the TrueStmt object.
	 It is derived from the Stmt class.
*/
class TrueStmt: public Stmt {
	public:
		TrueStmt(bool b) {
			this->b = b ;
		}	
	
	private:
		bool b ;
} ;

/*!  This is the base class for the False Statements and the FalseStmt object.
	 It is derived from the Stmt class.
*/
class FalseStmt: public Stmt {
	public:
		FalseStmt(bool b) {
			this->b = b ;
		}
		
	private:
		bool b ;
} ;

#endif