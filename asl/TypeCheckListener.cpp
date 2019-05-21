//////////////////////////////////////////////////////////////////////
//
//    TypeCheckListener - Walk the parser tree to do the semantic
//                        typecheck for the Asl programming language
//
//    Copyright (C) 2018  Universitat Politecnica de Catalunya
//
//    This library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU General Public License
//    as published by the Free Software Foundation; either version 3
//    of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Affero General Public License for more details.
//
//    You should have received a copy of the GNU Affero General Public
//    License along with this library; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
//    contact: José Miguel Rivero (rivero@cs.upc.edu)
//             Computer Science Department
//             Universitat Politecnica de Catalunya
//             despatx Omega.110 - Campus Nord UPC
//             08034 Barcelona.  SPAIN
//
//////////////////////////////////////////////////////////////////////

#include "TypeCheckListener.h"

#include "antlr4-runtime.h"

#include "../common/TypesMgr.h"
#include "../common/SymTable.h"
#include "../common/TreeDecoration.h"
#include "../common/SemErrors.h"

#include <iostream>
#include <string>

// uncomment the following line to enable debugging messages with DEBUG*
//#define DEBUG_BUILD
#include "../common/debug.h"

// using namespace std;


// Constructor
TypeCheckListener::TypeCheckListener(TypesMgr       & Types,
				     SymTable       & Symbols,
				     TreeDecoration & Decorations,
				     SemErrors      & Errors) :
  Types{Types},
  Symbols {Symbols},
  Decorations{Decorations},
  Errors{Errors} {
}

void TypeCheckListener::enterProgram(AslParser::ProgramContext *ctx) {
  DEBUG_ENTER();
  SymTable::ScopeId sc = getScopeDecor(ctx);
  Symbols.pushThisScope(sc);
}
void TypeCheckListener::exitProgram(AslParser::ProgramContext *ctx) {
  if (Symbols.noMainProperlyDeclared())
    Errors.noMainProperlyDeclared(ctx);
  Symbols.popScope();
  Errors.print();
  DEBUG_EXIT();
}

void TypeCheckListener::enterFunction(AslParser::FunctionContext *ctx) {
  DEBUG_ENTER();
  SymTable::ScopeId sc = getScopeDecor(ctx);
  TypesMgr::TypeId t;
  if (Types.isErrorTy(getTypeDecor(ctx))) {
    t = Types.createErrorTy();  
  }
  else 
    t = Symbols.getType(ctx->ID()->getText());
  Symbols.setCurrentFunctionTy(t);
  Symbols.pushThisScope(sc);
  // Symbols.print();
}
void TypeCheckListener::exitFunction(AslParser::FunctionContext *ctx) {
  Symbols.popScope();
  DEBUG_EXIT();
}

void TypeCheckListener::enterDeclarations(AslParser::DeclarationsContext *ctx) {
  DEBUG_ENTER();
}
void TypeCheckListener::exitDeclarations(AslParser::DeclarationsContext *ctx) {
  DEBUG_EXIT();
}

void TypeCheckListener::enterVariable_decl(AslParser::Variable_declContext *ctx) {
  DEBUG_ENTER();
}
void TypeCheckListener::exitVariable_decl(AslParser::Variable_declContext *ctx) {
  DEBUG_EXIT();
}

void TypeCheckListener::enterType(AslParser::TypeContext *ctx) {
  DEBUG_ENTER();
}
void TypeCheckListener::exitType(AslParser::TypeContext *ctx) {
  DEBUG_EXIT();
}

void TypeCheckListener::enterStatements(AslParser::StatementsContext *ctx) {
  DEBUG_ENTER();
}
void TypeCheckListener::exitStatements(AslParser::StatementsContext *ctx) {
  DEBUG_EXIT();
}

void TypeCheckListener::enterAssignStmt(AslParser::AssignStmtContext *ctx) {
  DEBUG_ENTER();
}
void TypeCheckListener::exitAssignStmt(AslParser::AssignStmtContext *ctx) {
  TypesMgr::TypeId t1 = getTypeDecor(ctx->left_expr());
  TypesMgr::TypeId t2 = getTypeDecor(ctx->expr());
  if ((not Types.isErrorTy(t1)) and (not Types.isErrorTy(t2)) and
      (not Types.copyableTypes(t1, t2)))
    Errors.incompatibleAssignment(ctx->ASSIGN());
  if ((not Types.isErrorTy(t1)) and (not getIsLValueDecor(ctx->left_expr())))
    Errors.nonReferenceableLeftExpr(ctx->left_expr());
  DEBUG_EXIT();
}

void TypeCheckListener::enterIfStmt(AslParser::IfStmtContext *ctx) {
  DEBUG_ENTER();
}
void TypeCheckListener::exitIfStmt(AslParser::IfStmtContext *ctx) {
  TypesMgr::TypeId t1 = getTypeDecor(ctx->expr());
  if ((not Types.isErrorTy(t1)) and (not Types.isBooleanTy(t1)))
    Errors.booleanRequired(ctx);
  DEBUG_EXIT();
}

void TypeCheckListener::enterWhileStmt(AslParser::WhileStmtContext *ctx) {
  DEBUG_ENTER();
}
void TypeCheckListener::exitWhileStmt(AslParser::WhileStmtContext *ctx) {
  TypesMgr::TypeId t1 = getTypeDecor(ctx->expr());
  if ((not Types.isErrorTy(t1)) and (not Types.isBooleanTy(t1)))
    Errors.booleanRequired(ctx);
  DEBUG_EXIT();
}


void TypeCheckListener::enterFunCallStmt(AslParser::FunCallStmtContext *ctx) {
  DEBUG_ENTER();
}
void TypeCheckListener::exitFunCallStmt(AslParser::FunCallStmtContext *ctx) {
  DEBUG_EXIT();
}


void TypeCheckListener::enterReadStmt(AslParser::ReadStmtContext *ctx) {
  DEBUG_ENTER();
}
void TypeCheckListener::exitReadStmt(AslParser::ReadStmtContext *ctx) {
  TypesMgr::TypeId t1 = getTypeDecor(ctx->left_expr());
  if ((not Types.isErrorTy(t1)) and (not Types.isPrimitiveTy(t1)) and
      (not Types.isFunctionTy(t1)))
    Errors.readWriteRequireBasic(ctx);
  if ((not Types.isErrorTy(t1)) and (not getIsLValueDecor(ctx->left_expr())))
    Errors.nonReferenceableExpression(ctx);
  DEBUG_EXIT();
}

void TypeCheckListener::enterWriteExpr(AslParser::WriteExprContext *ctx) {
  DEBUG_ENTER();
}
void TypeCheckListener::exitWriteExpr(AslParser::WriteExprContext *ctx) {
  TypesMgr::TypeId t1 = getTypeDecor(ctx->expr());
  if ((not Types.isErrorTy(t1)) and (not Types.isPrimitiveTy(t1)))
    Errors.readWriteRequireBasic(ctx);
  DEBUG_EXIT();
}

void TypeCheckListener::enterWriteString(AslParser::WriteStringContext *ctx) {
  DEBUG_ENTER();
}
void TypeCheckListener::exitWriteString(AslParser::WriteStringContext *ctx) {
  DEBUG_EXIT();
}

void TypeCheckListener::enterReturnStmt(AslParser::ReturnStmtContext *ctx) {
  DEBUG_ENTER();
}

void TypeCheckListener::exitReturnStmt(AslParser::ReturnStmtContext *ctx) {
  TypesMgr::TypeId t = Symbols.getCurrentFunctionTy();
  //std::cout<<Types.to_string(t)<<std::endl;
  if (not Types.isErrorTy(t)) {
    TypesMgr::TypeId rt = Types.getFuncReturnType(t);
    if (ctx-> expr()) {
      TypesMgr::TypeId et = getTypeDecor(ctx->expr());
      //std::cout<<Types.to_string(et)<<std::endl;
      //std::cout<<Types.to_string(rt)<<std::endl;
      if (not Types.copyableTypes(rt,et))
        Errors.incompatibleReturn(ctx->RETURN());
    }
    else if(not Types.isVoidFunction(t)) 
      Errors.incompatibleReturn(ctx->RETURN());
  }
  DEBUG_EXIT();
}

void TypeCheckListener::enterLeft_expr(AslParser::Left_exprContext *ctx) {
  DEBUG_ENTER();
}
void TypeCheckListener::exitLeft_expr(AslParser::Left_exprContext *ctx) {
  TypesMgr::TypeId t1; 
  bool b;
  if (ctx->array_access()) {
  	t1 = getTypeDecor(ctx->array_access());
  	b = getIsLValueDecor(ctx->array_access());
  }
  else {
  	t1 = getTypeDecor(ctx->ident());
  	b = getIsLValueDecor(ctx->ident());
  }
  putTypeDecor(ctx,t1);
  putIsLValueDecor(ctx, b);
}

void TypeCheckListener::enterUnary(AslParser::UnaryContext *ctx) {
  DEBUG_ENTER();
}

void TypeCheckListener::exitUnary(AslParser::UnaryContext *ctx) {
  TypesMgr::TypeId t = getTypeDecor(ctx->expr());
  if (ctx->NOT()){
    if (not Types.isBooleanTy(t) and not Types.isErrorTy(t) ) {
      Errors.incompatibleOperator(ctx->op);
    }
    putTypeDecor(ctx, Types.createBooleanTy());
    putIsLValueDecor(ctx, getIsLValueDecor(ctx->expr()));
  }
  else {
    if (not Types.isNumericTy(t) and not Types.isErrorTy(t)) Errors.incompatibleOperator(ctx->op);
    putTypeDecor(ctx, t);
    putIsLValueDecor(ctx, getIsLValueDecor(ctx->expr()));
  }
  DEBUG_EXIT();
}

void TypeCheckListener::enterArithmetic(AslParser::ArithmeticContext *ctx) {
  DEBUG_ENTER();
}
void TypeCheckListener::exitArithmetic(AslParser::ArithmeticContext *ctx) {
  TypesMgr::TypeId t1 = getTypeDecor(ctx->expr(0));
  TypesMgr::TypeId t2 = getTypeDecor(ctx->expr(1));
  bool incompatible = false;
  if ((not Types.isErrorTy(t1)) and (not Types.isNumericTy(t1)))
    incompatible = true;
  if ((not Types.isErrorTy(t2)) and (not Types.isNumericTy(t2)))
    incompatible = true;
  if (ctx->MOD() and (((not Types.isErrorTy(t1)) and (not Types.isIntegerTy(t1))) or
                     ((not Types.isErrorTy(t2)) and (not Types.isIntegerTy(t2)))))
    incompatible = true;
  if (incompatible)
    Errors.incompatibleOperator(ctx->op);
  TypesMgr::TypeId t; 
  if (Types.isFloatTy(t1) or (Types.isFloatTy(t2)))
    t = Types.createFloatTy();
  else
    t = Types.createIntegerTy();
  putTypeDecor(ctx, t);
  putIsLValueDecor(ctx, false);
  DEBUG_EXIT();
}

void TypeCheckListener::enterRelational(AslParser::RelationalContext *ctx) {
  DEBUG_ENTER();
}
void TypeCheckListener::exitRelational(AslParser::RelationalContext *ctx) {
  TypesMgr::TypeId t1 = getTypeDecor(ctx->expr(0));
  TypesMgr::TypeId t2 = getTypeDecor(ctx->expr(1));
  std::string oper = ctx->op->getText();
  if ((not Types.isErrorTy(t1)) and (not Types.isErrorTy(t2)) and
      (not Types.comparableTypes(t1, t2, oper)))
    Errors.incompatibleOperator(ctx->op);
  TypesMgr::TypeId t = Types.createBooleanTy();
  putTypeDecor(ctx, t);
  putIsLValueDecor(ctx, false);
  DEBUG_EXIT();
}

void TypeCheckListener::enterBoolean(AslParser::BooleanContext *ctx) {
  DEBUG_ENTER();
}
void TypeCheckListener::exitBoolean(AslParser::BooleanContext *ctx) {
  TypesMgr::TypeId t1 = getTypeDecor(ctx->expr(0));
  TypesMgr::TypeId t2 = getTypeDecor(ctx->expr(1));
  std::string oper = ctx->op->getText();
  if (((not Types.isErrorTy(t1)) and (not Types.isBooleanTy(t1))) or
      ((not Types.isErrorTy(t2)) and (not Types.isBooleanTy(t2))))
    Errors.incompatibleOperator(ctx->op);
  TypesMgr::TypeId t = Types.createBooleanTy();
  putTypeDecor(ctx, t);
  putIsLValueDecor(ctx, false);
  DEBUG_EXIT();
}

void TypeCheckListener::enterParenthesis(AslParser::ParenthesisContext *ctx) {
  DEBUG_ENTER();
}

void TypeCheckListener::exitParenthesis(AslParser::ParenthesisContext *ctx) {
  putTypeDecor(ctx, getTypeDecor(ctx->expr()));
  putIsLValueDecor(ctx, getIsLValueDecor(ctx->expr()));
  DEBUG_EXIT();
}

void TypeCheckListener::enterValue(AslParser::ValueContext *ctx) {
  DEBUG_ENTER();
}
void TypeCheckListener::exitValue(AslParser::ValueContext *ctx) {
  TypesMgr::TypeId t; 
  
  if (ctx->INTVAL())
    t = Types.createIntegerTy();
  else if (ctx->CHARVAL()) 
    t = Types.createCharacterTy();
  else if (ctx->FLOATVAL()) 
    t = Types.createFloatTy();
  else if (ctx->BOOLVAL()) 
    t = Types.createBooleanTy();
  putTypeDecor(ctx, t);
  putIsLValueDecor(ctx, false);
  DEBUG_EXIT();
}

void TypeCheckListener::enterExprIdent(AslParser::ExprIdentContext *ctx) {
  DEBUG_ENTER();
}
void TypeCheckListener::exitExprIdent(AslParser::ExprIdentContext *ctx) {
  TypesMgr::TypeId t1 = getTypeDecor(ctx->ident());
  putTypeDecor(ctx, t1);
  bool b = getIsLValueDecor(ctx->ident());
  putIsLValueDecor(ctx, b);
  DEBUG_EXIT();
}

void TypeCheckListener::enterExprAAccess(AslParser::ExprAAccessContext *ctx){
  DEBUG_ENTER();
}

void TypeCheckListener::exitExprAAccess(AslParser::ExprAAccessContext *ctx){
  TypesMgr::TypeId t1 = getTypeDecor(ctx->array_access());
  putTypeDecor(ctx, t1);
  bool b = getIsLValueDecor(ctx->array_access());
  putIsLValueDecor(ctx, b);
  DEBUG_EXIT();
}


void TypeCheckListener::enterArray_access(AslParser::Array_accessContext *ctx){
  DEBUG_ENTER();
}

void TypeCheckListener::exitArray_access(AslParser::Array_accessContext *ctx){
  TypesMgr::TypeId t = getTypeDecor(ctx->ident());
  TypesMgr::TypeId ta = getTypeDecor(ctx->expr());
  TypesMgr::TypeId tr;

  if (not Types.isArrayTy(t)) {
    if (not Types.isErrorTy(t)) Errors.nonArrayInArrayAccess(ctx);
    tr = Types.createErrorTy();
    //std::cout<<"error in array"<<std::endl;
  }
  else tr = Types.getArrayElemType(t);
  if (not Types.isIntegerTy(ta))
    Errors.nonIntegerIndexInArrayAccess(ctx->expr());
  putTypeDecor(ctx, tr);
  putIsLValueDecor(ctx, getIsLValueDecor(ctx->ident()));
  DEBUG_EXIT();
}

void TypeCheckListener::enterFuncall(AslParser::FuncallContext *ctx) {
  DEBUG_ENTER();
}

void TypeCheckListener::exitFuncall(AslParser::FuncallContext *ctx) {
  TypesMgr::TypeId t1 = getTypeDecor(ctx->ident());
  if (Types.isFunctionTy(t1)) {
    int npF, npC;
    if (ctx->call_parameters())
      npC = ctx->call_parameters()->expr().size();
    else 
      npC = 0;
    npF = Types.getNumOfParameters(t1);
    if (npF != npC)
      Errors.numberOfParameters(ctx);
    else {
      for (int i=0; i < npF; ++i) {
        TypesMgr::TypeId pF= Types.getParameterType(t1, i);
        TypesMgr::TypeId pC= getTypeDecor(ctx->call_parameters()->expr(i));
        if (not Types.copyableTypes(pF,pC))
          Errors.incompatibleParameter(ctx->call_parameters()->expr(i),i+1,ctx);
      }
    }
    putTypeDecor(ctx, Types.getFuncReturnType(t1));
  }
  else {
    if (not Types.isErrorTy(t1))
      Errors.isNotCallable(ctx->ident());
    TypesMgr::TypeId e = Types.createErrorTy();
    putTypeDecor(ctx, e);
  } 
  bool b = getIsLValueDecor(ctx->ident());
  putIsLValueDecor(ctx,b);
  DEBUG_EXIT();
}



void TypeCheckListener::enterExprFuncall(AslParser::ExprFuncallContext *ctx) {
  DEBUG_ENTER();
}
void TypeCheckListener::exitExprFuncall(AslParser::ExprFuncallContext *ctx) {
  TypesMgr::TypeId t1 = getTypeDecor(ctx->funcall());
  TypesMgr::TypeId tf = getTypeDecor(ctx->funcall()->ident());
  if (Types.isFunctionTy(tf) and Types.isVoidFunction(tf)) {
    Errors.isNotFunction(ctx->funcall()->ident());
    putTypeDecor(ctx, Types.createErrorTy());
  }
  else putTypeDecor(ctx, t1);
  bool b = getIsLValueDecor(ctx->funcall());
  putIsLValueDecor(ctx,b);
  DEBUG_EXIT();
}

void TypeCheckListener::enterIdent(AslParser::IdentContext *ctx) {
  DEBUG_ENTER();
}
void TypeCheckListener::exitIdent(AslParser::IdentContext *ctx) {
  std::string ident = ctx->getText();
  if (Symbols.findInStack(ident) == -1) {
    Errors.undeclaredIdent(ctx->ID());
    TypesMgr::TypeId te = Types.createErrorTy();
    putTypeDecor(ctx, te);
    putIsLValueDecor(ctx, true);
  }
  else {
    TypesMgr::TypeId t1 = Symbols.getType(ident);
    putTypeDecor(ctx, t1);
    if (Symbols.isFunctionClass(ident))
      putIsLValueDecor(ctx, false);
    else
      putIsLValueDecor(ctx, true);
  }
  DEBUG_EXIT();
}

// void TypeCheckListener::enterEveryRule(antlr4::ParserRuleContext *ctx) {
//   DEBUG_ENTER();
// }
// void TypeCheckListener::exitEveryRule(antlr4::ParserRuleContext *ctx) {
//   DEBUG_EXIT();
// }
// void TypeCheckListener::visitTerminal(antlr4::tree::TerminalNode *node) {
//   DEBUG("visitTerminal");
// }
// void TypeCheckListener::visitErrorNode(antlr4::tree::ErrorNode *node) {
// }


// Getters for the necessary tree node atributes:
//   Scope, Type ans IsLValue
SymTable::ScopeId TypeCheckListener::getScopeDecor(antlr4::ParserRuleContext *ctx) {
  return Decorations.getScope(ctx);
}
TypesMgr::TypeId TypeCheckListener::getTypeDecor(antlr4::ParserRuleContext *ctx) {
  return Decorations.getType(ctx);
}
bool TypeCheckListener::getIsLValueDecor(antlr4::ParserRuleContext *ctx) {
  return Decorations.getIsLValue(ctx);
}

// Setters for the necessary tree node attributes:
//   Scope, Type ans IsLValue
void TypeCheckListener::putScopeDecor(antlr4::ParserRuleContext *ctx, SymTable::ScopeId s) {
  Decorations.putScope(ctx, s);
}
void TypeCheckListener::putTypeDecor(antlr4::ParserRuleContext *ctx, TypesMgr::TypeId t) {
  Decorations.putType(ctx, t);
}
void TypeCheckListener::putIsLValueDecor(antlr4::ParserRuleContext *ctx, bool b) {
  Decorations.putIsLValue(ctx, b);
}
