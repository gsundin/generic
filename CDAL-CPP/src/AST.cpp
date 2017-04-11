#ifndef Node_H
#define Node_H

#include <string>
#include <string.h>
#include <iostream>
#include <sstream>
#include <math.h>
#include <typeinfo>
#include <stdio.h>

#include "scanner.h"
#include <stdlib.h>
#include "AST.h"

using namespace std;

MatrixDecl::MatrixDecl(VariableName* vn, Expr* e1, Expr* e2, Expr* e3){
	this->vn = vn;
	this->e1 = e1;
	this->e2 = e2;
	this->e3 = e3;
}

std::string MatrixDecl::unparse(){
	std::stringstream ss;
	ss <<"matrix "<< vn <<" [ "<<e1<<" : "<<e2<<" ] "<<" = "<<e3<<" ;";
	return ss.str();
}

TwoStmts::TwoStmts(Stmt * s, Stmts * ss) {
    this->s = s;
    this->ss = ss;
}

std::string TwoStmts::unparse(){
  	std::stringstream str;
  	//printf("%s\n", (!ss) ? "Empty": "Filled");
    // str << s->unparse() << ss->unparse() ;
    return str.str() ;
}

PrintStmt::PrintStmt(Stmt* s){
	this->s = s;
}

std::string PrintStmt::unparse(){
	std::stringstream ss;
	ss << "print ( " << s->unparse() << " ) ;";
	return ss.str();
}

std::string semiColonStmt::unparse(){
	std::stringstream ss;
	ss<<";";
	return ss.str();
}

Program::Program(VariableName* v, Stmts* s): varName(v), stmts(s){} ;

std::string Program::unparse() {
	return varName->unparse() + " () {" + stmts->unparse() + " } ";
}

LetExpr::LetExpr(Stmts *s, Expr *e) {
	    stmts = s;
	    ex = e;
}

std::string LetExpr::unparse(){
  	std::stringstream ss;
  	ss << "let " << stmts->unparse() <<" in "<< ex->unparse()<<" end";
  	return ss.str();
}

NotExpr::NotExpr(Expr *e){
	ex = e;
}

AdditionExpr::AdditionExpr(Expr *e1, Expr *e2){
	this->e1 = e1;
	this->e2 = e2;
}

std::string AdditionExpr::unparse(){
	std::stringstream ss;
	ss << e1->unparse() <<" + "<< e2->unparse();
	return ss.str();
}


SubtractionExpr::SubtractionExpr(Expr *e1, Expr *e2){
	this->e1 = e1;
	this->e2 = e2;
}

std::string SubtractionExpr::unparse(){
	std::stringstream ss;
	ss << e1->unparse() <<" - "<< e2->unparse();
	return ss.str();
}

MultExpr::MultExpr(Expr *e1, Expr *e2){
	this->e1 = e1;
	this->e2 = e2;
}

std::string MultExpr::unparse(){
	std::stringstream ss;
	ss << e1->unparse() <<" * "<< e2->unparse();
	return ss.str();
}

DivExpr::DivExpr(Expr *e1, Expr *e2){
	this->e1 = e1;
	this->e2 = e2;
}

std::string DivExpr::unparse(){
  	std::stringstream ss;
  	ss << e1->unparse() <<" * "<< e2->unparse();
  	return ss.str();
}

GreaterExpr::GreaterExpr(Expr *e1, Expr *e2){
			this->e1 = e1;
			this->e2 = e2;
		}

std::string GreaterExpr::unparse(){
	  	std::stringstream ss;
	  	ss << e1->unparse() <<" > "<< e2->unparse();
	  	return ss.str();
}

GreaterEqExpr::GreaterEqExpr(Expr *e1, Expr *e2){
	this->e1 = e1;
	this->e2 = e2;
}

std::string GreaterEqExpr::unparse(){
	std::stringstream ss;
	ss << e1->unparse() <<" >= "<< e2->unparse();
	return ss.str();
}

LesserExpr::LesserExpr(Expr *e1, Expr *e2){
	this->e1 = e1;
	this->e2 = e2;
}

std::string LesserExpr::unparse(){
	std::stringstream ss;
	ss << e1->unparse() <<" < "<< e2->unparse();
	return ss.str();
}

LesserEqExpr::LesserEqExpr(Expr *e1, Expr *e2){
	this->e1 = e1;
	this->e2 = e2;
}

std::string LesserEqExpr::unparse(){
	std::stringstream ss;
	ss << e1->unparse() <<" <= "<< e2->unparse();
	return ss.str();
}

EqEqExpr::EqEqExpr(Expr *e1, Expr *e2){
	this->e1 = e1;
	this->e2 = e2;
}

std::string EqEqExpr::unparse(){
	std::stringstream ss;
	ss << e1->unparse() <<" == "<< e2->unparse();
	return ss.str();
}

NotEqExpr::NotEqExpr(Expr *e1, Expr *e2){
	this->e1 = e1;
	this->e2 = e2;
}

std::string NotEqExpr::unparse(){
	std::stringstream ss;
	ss << e1->unparse() <<" != "<< e2->unparse();
	return ss.str();
}

AndExpr::AndExpr(Expr *e1, Expr *e2){
	this->e1 = e1;
	this->e2 = e2;
}

std::string AndExpr::unparse(){
	std::stringstream ss;
	ss << e1->unparse() <<" && "<< e2->unparse();
	return ss.str();
}

OrExpr::OrExpr(Expr *e1, Expr *e2){
	this->e1 = e1;
	this->e2 = e2;
}

std::string OrExpr::unparse(){
	std::stringstream ss;
	ss << e1->unparse() <<" || "<< e2->unparse();
	return ss.str();
}

IntegerStmt::IntegerStmt(int i){
	this->i = i;
}
std::string IntegerStmt::unparse(){
	std::stringstream ss;
	ss<<i;
	return ss.str();
}

FloatStmt::FloatStmt(float f){
	this->f = f;
}
std::string FloatStmt::unparse(){
	std::stringstream ss;
	ss<<f;
	return ss.str();
}	

TrueStmt::TrueStmt(bool b){
	this->b = b;
}	
std::string TrueStmt::unparse(){
	std::stringstream ss;
	ss<< b;
	return ss.str();
}
FalseStmt::FalseStmt(bool b){
	this->b = b;
}
std::string FalseStmt::unparse(){
	std::stringstream ss;
	ss<< b;
	return ss.str();

}	
#endif