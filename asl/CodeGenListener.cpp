//////////////////////////////////////////////////////////////////////
//
//    CodeGenListener - Walk the parser tree to do
//                             the generation of code
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

#include "CodeGenListener.h"

#include "antlr4-runtime.h"

#include "../common/TypesMgr.h"
#include "../common/SymTable.h"
#include "../common/TreeDecoration.h"
#include "../common/code.h"

#include <cstddef>    // std::size_t

// uncomment the following line to enable debugging messages with DEBUG*
//#define DEBUG_BUILD
#include "../common/debug.h"

// using namespace std;


// Constructor
CodeGenListener::CodeGenListener(TypesMgr       & Types,
				 SymTable       & Symbols,
				 TreeDecoration & Decorations,
				 code           & Code) :
  Types{Types},
  Symbols{Symbols},
  Decorations{Decorations},
  Code{Code} {
}

void CodeGenListener::enterProgram(AslParser::ProgramContext *ctx) {
  DEBUG_ENTER();
  SymTable::ScopeId sc = getScopeDecor(ctx);
  Symbols.pushThisScope(sc);
}
void CodeGenListener::exitProgram(AslParser::ProgramContext *ctx) {
  Symbols.popScope();
  DEBUG_EXIT();
}

void CodeGenListener::enterFunction(AslParser::FunctionContext *ctx) {
  DEBUG_ENTER();
  subroutine subr(ctx->ID()->getText());
  //if at the exam functions can return something other than a basic type, the space to be pushed 
  // should be equal to the return parameter size. Quite unlikely, though.
  if (ctx->basictype()) subr.add_param("_result");
  Code.add_subroutine(subr);
  SymTable::ScopeId sc = getScopeDecor(ctx);
  Symbols.pushThisScope(sc);
  codeCounters.reset();
}
void CodeGenListener::exitFunction(AslParser::FunctionContext *ctx) {
  subroutine & subrRef = Code.get_last_subroutine();
  instructionList code = getCodeDecor(ctx->statements());
  code = code || instruction::RETURN();
  subrRef.set_instructions(code);
  Symbols.popScope();
  DEBUG_EXIT();
}

void CodeGenListener::enterParameters(AslParser::ParametersContext *ctx) {
  DEBUG_ENTER();
}

void CodeGenListener::exitParameters(AslParser::ParametersContext *ctx) {
  int s = ctx->ID().size();
  subroutine & subrRef = Code.get_last_subroutine();
  for (int i=0; i<s; ++i) {
    subrRef.add_param(ctx->ID(i)->getText());
  }
  DEBUG_EXIT();
}

void CodeGenListener::enterDeclarations(AslParser::DeclarationsContext *ctx) {
  DEBUG_ENTER();
}
void CodeGenListener::exitDeclarations(AslParser::DeclarationsContext *ctx) {
  DEBUG_EXIT();
}

void CodeGenListener::enterVariable_decl(AslParser::Variable_declContext *ctx) {
  DEBUG_ENTER();
}
void CodeGenListener::exitVariable_decl(AslParser::Variable_declContext *ctx) {
  subroutine       & subrRef = Code.get_last_subroutine();
  TypesMgr::TypeId        t1 = getTypeDecor(ctx->type());
  std::size_t           size = Types.getSizeOfType(t1);
  int s = ctx->ID().size();
  for (int i=0; i < s; ++i) {
    subrRef.add_var(ctx->ID(i)->getText(), size);  
  }  
  DEBUG_EXIT();
}

void CodeGenListener::enterType(AslParser::TypeContext *ctx) {
  DEBUG_ENTER();
}
void CodeGenListener::exitType(AslParser::TypeContext *ctx) {
  DEBUG_EXIT();
}

void CodeGenListener::enterStatements(AslParser::StatementsContext *ctx) {
  DEBUG_ENTER();
}
void CodeGenListener::exitStatements(AslParser::StatementsContext *ctx) {
  instructionList code;
  for (auto stCtx : ctx->statement()) {
    code = code || getCodeDecor(stCtx);
  }
  putCodeDecor(ctx, code);
  DEBUG_EXIT();
}

void CodeGenListener::enterAssignStmt(AslParser::AssignStmtContext *ctx) {
  DEBUG_ENTER();
}
void CodeGenListener::exitAssignStmt(AslParser::AssignStmtContext *ctx) {
  instructionList  code;
  std::string     addr1 = getAddrDecor(ctx->left_expr());
  std::string     offs1 = getOffsetDecor(ctx->left_expr());
  instructionList code1 = getCodeDecor(ctx->left_expr());
  TypesMgr::TypeId tid1 = getTypeDecor(ctx->left_expr());
  std::string     addr2 = getAddrDecor(ctx->expr());
  std::string     offs2 = getOffsetDecor(ctx->expr());
  instructionList code2 = getCodeDecor(ctx->expr());
  TypesMgr::TypeId tid2 = getTypeDecor(ctx->expr());
  /*
  if (Types.isArrayTy(tid1) and Types.isArrayTy(tid2)) {
    code = code1 || code2;
    std::string iter = "%"+codeCounters.newTEMP();
    std::string elem = "%"+codeCounters.newTEMP();
    int size = Types.getArraySize(tid1);
    for (int i = 0; i < size; ++i) {
      code = code || instruction::ILOAD(iter, std::to_string(i))
                  || instruction::LOADX(elem, addr2, iter)
                  || instruction::XLOAD(addr1, iter, elem);
    }
  }*/
  //Option 2: More compact (in resulting t-code terms): do a while loop
  if (Types.isArrayTy(tid1) and Types.isArrayTy(tid2)) {
    code = code1 || code2;
    int sizeI = Types.getArraySize(tid1);
    std::string iter = "%"+codeCounters.newTEMP();
    std::string incr = "%"+codeCounters.newTEMP();
    std::string elem = "%"+codeCounters.newTEMP();
    std::string temp = "%"+codeCounters.newTEMP();
    std::string size = "%"+codeCounters.newTEMP();
    std::string label = codeCounters.newLabelWHILE();
    std::string labelDo = "doACopy"+label;
    std::string labelEn = "enACopy"+label;
    instructionList iterCheck = instruction::LT(temp, iter, size) 
                             || instruction::FJUMP(temp, labelEn);
    instructionList elemAssgm = instruction::LOADX(elem, addr2, iter)
                             || instruction::XLOAD(addr1, iter, elem);
    instructionList iterIncre = instruction::ADD(iter, iter, incr);
    code = code || instruction::ILOAD(iter, "0")
                || instruction::ILOAD(incr, "1") 
                || instruction::ILOAD(size, std::to_string(sizeI));
    code = code || instruction::LABEL(labelDo);
    code = code || iterCheck || elemAssgm || iterIncre;
    code = code || instruction::UJUMP(labelDo) || instruction::LABEL(labelEn);
  }
  else {
    bool o1 = (offs1 != "");
    bool o2 = (offs2 != "");
    code = code1 || code2;
    if (not o1 and not o2) {
    	code = code || instruction::LOAD(addr1, addr2);
    }
    else if (not o1 and o2) {
      //can't happen because right expressions are always already accessed
    	//code = code || instruction::LOADX(addr1, addr2, offs2);
    }
    else if (o1 and not o2) {
    	code = code || instruction::XLOAD(addr1, offs1, addr2);	
    }
    else  {
      //can't happen for same reason as previous
    	//std::string temp = "%"+codeCounters.newTEMP();
    	//code = code || instruction::LOADX(temp, addr2, offs2);
    	//code = code || instruction::XLOAD(addr1, offs1, temp);			
    }
  }
  putCodeDecor(ctx, code);
  DEBUG_EXIT();
}

void CodeGenListener::enterIfStmt(AslParser::IfStmtContext *ctx) {
  DEBUG_ENTER();
}
void CodeGenListener::exitIfStmt(AslParser::IfStmtContext *ctx) {
  instructionList   code;
  std::string      addr1 = getAddrDecor(ctx->expr());
  instructionList  code1 = getCodeDecor(ctx->expr());
  instructionList  code2 = getCodeDecor(ctx->statements(0));
  std::string      label = codeCounters.newLabelIF();
  std::string labelEndIf = "endif"+label;
  if (ctx->statements().size() < 2) {
  	code = code1 || instruction::FJUMP(addr1, labelEndIf) ||
           code2 || instruction::LABEL(labelEndIf);
  }
  else {
  	std::string      label = codeCounters.newLabelIF();
  	std::string labelElse = "else"+label;
  	instructionList  code3 = getCodeDecor(ctx->statements(1));
  	code = code1 || instruction::FJUMP(addr1, labelElse) ||
           code2 || instruction::UJUMP(labelEndIf) ||
           instruction::LABEL(labelElse) || code3 || 
           instruction::LABEL(labelEndIf);
  }
  putCodeDecor(ctx, code);
  DEBUG_EXIT();
}

void CodeGenListener::enterWhileStmt(AslParser::WhileStmtContext *ctx) {
  DEBUG_ENTER();
}

void CodeGenListener::exitWhileStmt(AslParser::WhileStmtContext *ctx) {
  instructionList   code;
  std::string      addr1 = getAddrDecor(ctx->expr());
  instructionList  code1 = getCodeDecor(ctx->expr());
  instructionList  code2 = getCodeDecor(ctx->statements());
  std::string      label = codeCounters.newLabelWHILE();
  std::string labelDoWhile = "doWhile"+label;
  std::string labelEndWhile = "endWhile"+label;
  code = instruction::LABEL(labelDoWhile) ||
         code1 || 
         instruction::FJUMP(addr1, labelEndWhile) ||
         code2 || 
         instruction::UJUMP(labelDoWhile) ||
         instruction::LABEL(labelEndWhile);
  putCodeDecor(ctx, code);
  DEBUG_EXIT();
}

void CodeGenListener::enterFunCallStmt(AslParser::FunCallStmtContext *ctx) {
  DEBUG_ENTER();
}
void CodeGenListener::exitFunCallStmt(AslParser::FunCallStmtContext *ctx) {
  putCodeDecor(ctx, getCodeDecor(ctx->funcall()));
  DEBUG_EXIT();
}

void CodeGenListener::enterExprFuncall(AslParser::ExprFuncallContext *ctx) {
  DEBUG_ENTER();
}

void CodeGenListener::exitExprFuncall(AslParser::ExprFuncallContext *ctx) {
  putCodeDecor(ctx, getCodeDecor(ctx->funcall()));
  putAddrDecor(ctx, getAddrDecor(ctx->funcall()));
  DEBUG_EXIT();
}

void CodeGenListener::enterFuncall(AslParser::FuncallContext *ctx) {
  DEBUG_ENTER();
}

void CodeGenListener::exitFuncall(AslParser::FuncallContext *ctx) {
  instructionList code = instructionList();
  TypesMgr::TypeId tf = getTypeDecor(ctx->ident());
  if (not Types.isVoidFunction(tf)) code = code || instruction::PUSH();
  int params = Types.getNumOfParameters(tf);
  for (int i = 0; i< params ; ++i) {
    std::string addrPar = getAddrDecor(ctx->call_parameters()->expr(i));
    instructionList codePar = getCodeDecor(ctx->call_parameters()->expr(i));
    code = code || codePar;
    TypesMgr::TypeId tPar = Types.getParameterType(tf,i);
    if (Types.isFloatTy(tPar) and
        Types.isIntegerTy(getTypeDecor(ctx->call_parameters()->expr(i)))) {
      std::string tempF = "%"+codeCounters.newTEMP(); 
      code = code || instruction::FLOAT(tempF, addrPar);
      addrPar = tempF;
    }
    else if (Types.isArrayTy(tPar)) {
      std::string tempF = "%"+codeCounters.newTEMP();
      code = code || instruction::ALOAD(tempF, addrPar);
      addrPar = tempF;
    }
    code = code || instruction::PUSH(addrPar);
  }
  std::string name = ctx->ident()->getText();
  code = code || instruction::CALL(name);
  for (int i = 0; i< params ; ++i) {
    code = code || instruction::POP();
  }
  if (not Types.isVoidFunction(tf)) {
    std::string temp = "%"+codeCounters.newTEMP(); 
    code = code || instruction::POP(temp);
    putAddrDecor(ctx, temp);
  }
  putCodeDecor(ctx, code);
  DEBUG_EXIT();
}

void CodeGenListener::enterReadStmt(AslParser::ReadStmtContext *ctx) {
  DEBUG_ENTER();
}
void CodeGenListener::exitReadStmt(AslParser::ReadStmtContext *ctx) {
  instructionList  code;
  std::string     addr1 = getAddrDecor(ctx->left_expr());
  std::string     offs1 = getOffsetDecor(ctx->left_expr());
  instructionList code1 = getCodeDecor(ctx->left_expr());
  TypesMgr::TypeId tid1 = getTypeDecor(ctx->left_expr());
  if (offs1 != "") {
    std::string     addrAux = "%"+codeCounters.newTEMP();
    if (Types.isIntegerTy(tid1)) code = code1 || instruction::READI(addrAux);
    else if (Types.isFloatTy(tid1)) code = code1 || instruction::READF(addrAux);
    else if (Types.isCharacterTy(tid1)) code = code1 || instruction::READC(addrAux);
    else if (Types.isBooleanTy(tid1)) code = code1 || instruction::READI(addrAux);
    code = code || instruction::XLOAD(addr1,offs1,addrAux);
  }
  else {
    if (Types.isIntegerTy(tid1)) code = code1 || instruction::READI(addr1);
    else if (Types.isFloatTy(tid1)) code = code1 || instruction::READF(addr1);
    else if (Types.isCharacterTy(tid1)) code = code1 || instruction::READC(addr1);
    else if (Types.isBooleanTy(tid1)) code = code1 || instruction::READI(addr1);
  }
  putCodeDecor(ctx, code);
  DEBUG_EXIT();
}

void CodeGenListener::enterWriteExpr(AslParser::WriteExprContext *ctx) {
  DEBUG_ENTER();
}
void CodeGenListener::exitWriteExpr(AslParser::WriteExprContext *ctx) {
  instructionList code;
  std::string     addr1 = getAddrDecor(ctx->expr());
  std::string     addrAux = "%"+codeCounters.newTEMP();
  std::string     offs1 = getOffsetDecor(ctx->expr());
  instructionList code1 = getCodeDecor(ctx->expr());
  TypesMgr::TypeId tid1 = getTypeDecor(ctx->expr());
  if (offs1 != "") {
  	code1 = code1 || instruction::LOADX(addrAux, addr1, offs1);
  }
  else {
  	addrAux = addr1;
  }
  if (Types.isIntegerTy(tid1)) {
    code = code1 || instruction::WRITEI(addrAux);
  }
  else if (Types.isFloatTy(tid1)) {
    code = code1 || instruction::WRITEF(addrAux); 
  }
  else if (Types.isCharacterTy(tid1)) {
    code = code1 || instruction::WRITEC(addrAux); 
  }
  else code = code1 || instruction::WRITEI(addrAux);

  putCodeDecor(ctx, code);
  DEBUG_EXIT();
}

void CodeGenListener::enterWriteString(AslParser::WriteStringContext *ctx) {
  DEBUG_ENTER();
}
void CodeGenListener::exitWriteString(AslParser::WriteStringContext *ctx) {
  instructionList code;
  std::string s = ctx->STRING()->getText();
  std::string temp = "%"+codeCounters.newTEMP();
  int i = 1;
  while (i < int(s.size())-1) {
    if (s[i] != '\\') {
      code = code ||
	     instruction::CHLOAD(temp, s.substr(i,1)) ||
	     instruction::WRITEC(temp);
      i += 1;
    }
    else {
      assert(i < int(s.size())-2);
      if (s[i+1] == 'n') {
        code = code || instruction::WRITELN();
        i += 2;
      }
      else if (s[i+1] == 't' or s[i+1] == '"' or s[i+1] == '\\') {
        code = code ||
               instruction::CHLOAD(temp, s.substr(i,2)) ||
	       instruction::WRITEC(temp);
        i += 2;
      }
      else {
        code = code ||
               instruction::CHLOAD(temp, s.substr(i,1)) ||
	       instruction::WRITEC(temp);
        i += 1;
      }
    }
  }
  putCodeDecor(ctx, code);
  DEBUG_EXIT();
}

void CodeGenListener::enterReturnStmt(AslParser::ReturnStmtContext *ctx) {
  DEBUG_ENTER();
}

void CodeGenListener::exitReturnStmt(AslParser::ReturnStmtContext *ctx) {
  instructionList code;
  instructionList codeExpr = getCodeDecor(ctx->expr());
  std::string temp = getAddrDecor(ctx->expr());
  code = codeExpr || instruction::LOAD("_result", temp) || instruction::RETURN();
  putAddrDecor(ctx, temp);
  putOffsetDecor(ctx, "");
  putCodeDecor(ctx, code);

  DEBUG_EXIT();
}

void CodeGenListener::enterLeft_expr(AslParser::Left_exprContext *ctx) {
  DEBUG_ENTER();
}
void CodeGenListener::exitLeft_expr(AslParser::Left_exprContext *ctx) {
  if (ctx->array_access()) {
  	putAddrDecor(ctx, getAddrDecor(ctx->array_access()));
	  putOffsetDecor(ctx, getOffsetDecor(ctx->array_access()));
	  putCodeDecor(ctx, getCodeDecor(ctx->array_access()));
  }
  else {
	  putAddrDecor(ctx, getAddrDecor(ctx->ident()));
	  putOffsetDecor(ctx, getOffsetDecor(ctx->ident()));
	  putCodeDecor(ctx, getCodeDecor(ctx->ident()));
	}
	DEBUG_EXIT();
}

void CodeGenListener::enterUnary(AslParser::UnaryContext *ctx) {
  DEBUG_ENTER();
}

void CodeGenListener::exitUnary(AslParser::UnaryContext *ctx) {
  std::string     addr = getAddrDecor(ctx->expr());
  instructionList code = getCodeDecor(ctx->expr());
  std::string temp = "%"+codeCounters.newTEMP();
  if (ctx->NOT()) {
    code = code || instruction::NOT(temp, addr);
  }
  else if (ctx->PLUS()) {
    addr = temp;
  }
  else if (ctx-> MINUS()) {
    if (Types.isIntegerTy(getTypeDecor(ctx->expr())))
      code = code || instruction::NEG(temp, addr);
    else 
      code = code || instruction::FNEG(temp, addr);
  }
  putAddrDecor(ctx, temp);
  putOffsetDecor(ctx, "");
  putCodeDecor(ctx, code);
  DEBUG_EXIT();
}

void CodeGenListener::enterArithmetic(AslParser::ArithmeticContext *ctx) {
  DEBUG_ENTER();
}
void CodeGenListener::exitArithmetic(AslParser::ArithmeticContext *ctx) {
  std::string     addr1 = getAddrDecor(ctx->expr(0));
  instructionList code1 = getCodeDecor(ctx->expr(0));
  std::string     addr2 = getAddrDecor(ctx->expr(1));
  instructionList code2 = getCodeDecor(ctx->expr(1));
  instructionList code  = code1 || code2;
  std::string temp = "%"+codeCounters.newTEMP();
  TypesMgr::TypeId t  = getTypeDecor(ctx);
  if (Types.isIntegerTy(t)) {
    if (ctx->MUL()) {
      code = code || instruction::MUL(temp, addr1, addr2);
    }
    if (ctx->DIV()) { 
      code = code || instruction::DIV(temp, addr1, addr2);
    }
    if (ctx->MOD()) { 
      std::string temp1 = "%"+codeCounters.newTEMP();
      std::string temp2 = "%"+codeCounters.newTEMP();
      code = code || instruction::DIV(temp1, addr1, addr2);
      code = code || instruction::MUL(temp2, temp1, addr2);
      code = code || instruction::SUB(temp, addr1, temp2);
    }
    if (ctx->PLUS()) { 
      code = code || instruction::ADD(temp, addr1, addr2);
    }
    if (ctx->MINUS()) { 
      code = code || instruction::SUB(temp, addr1, addr2);
    }
  }
  else {
    TypesMgr::TypeId t1 = getTypeDecor(ctx->expr(0));
    TypesMgr::TypeId t2 = getTypeDecor(ctx->expr(1));
    std::string faddr1,faddr2;
    if(Types.isIntegerTy(t1)) {
      faddr1 = "%"+codeCounters.newTEMP();
      code = code || instruction::FLOAT(faddr1,addr1);
      faddr2 = addr2;
    }
    else if(Types.isIntegerTy(t2)) {
      faddr2 = "%"+codeCounters.newTEMP();
      code = code || instruction::FLOAT(faddr2,addr2); 
      faddr1 = addr1;
    }
    else {
      faddr1 = addr1;
      faddr2 = addr2;
    }
    if (ctx->MUL()) {
      code = code || instruction::FMUL(temp, faddr1, faddr2);
    }
    if (ctx->DIV()) { 
      code = code || instruction::FDIV(temp, faddr1, faddr2);
    }
    if (ctx->PLUS()) { 
      code = code || instruction::FADD(temp, faddr1, faddr2);
    }
    if (ctx->MINUS()) { 
      code = code || instruction::FSUB(temp, faddr1, faddr2);
    }
  }
  putAddrDecor(ctx, temp);
  putOffsetDecor(ctx, "");
  putCodeDecor(ctx, code);
  DEBUG_EXIT();
}

void CodeGenListener::enterRelational(AslParser::RelationalContext *ctx) {
  DEBUG_ENTER();
}
void CodeGenListener::exitRelational(AslParser::RelationalContext *ctx) {
  std::string     addr1 = getAddrDecor(ctx->expr(0));
  instructionList code1 = getCodeDecor(ctx->expr(0));
  std::string     addr2 = getAddrDecor(ctx->expr(1));
  instructionList code2 = getCodeDecor(ctx->expr(1));
  instructionList code  = code1 || code2;
  // TypesMgr::TypeId t1 = getTypeDecor(ctx->expr(0));
  // TypesMgr::TypeId t2 = getTypeDecor(ctx->expr(1));
  std::string temp = "%"+codeCounters.newTEMP();
  if (ctx->EQUAL()){
    code = code || instruction::EQ(temp, addr1, addr2);
  }
  if (ctx->DIFF()){
    code = code || instruction::EQ(temp, addr1, addr2);
    code = code || instruction::NOT(temp, temp);
  }
  if (ctx->GRT()){
    code = code || instruction::LT(temp, addr2, addr1);
  }
  if (ctx->GRTE()){
    code = code || instruction::LE(temp, addr2, addr1);
  }
  if (ctx->LST()){
    code = code || instruction::LT(temp, addr1, addr2);
  }
  if (ctx->LSTE()){
    code = code || instruction::LE(temp, addr1, addr2);
  }

  putAddrDecor(ctx, temp);
  putOffsetDecor(ctx, "");
  putCodeDecor(ctx, code);
  DEBUG_EXIT();
}

void CodeGenListener::enterBoolean(AslParser::BooleanContext *ctx) {
  DEBUG_ENTER();
}

void CodeGenListener::exitBoolean(AslParser::BooleanContext *ctx) {
  std::string     addr1 = getAddrDecor(ctx->expr(0));
  instructionList code1 = getCodeDecor(ctx->expr(0));
  std::string     addr2 = getAddrDecor(ctx->expr(1));
  instructionList code2 = getCodeDecor(ctx->expr(1));
  instructionList code  = code1 || code2;
  std::string temp = "%"+codeCounters.newTEMP();
  if (ctx->AND()) {
    code = code || instruction::AND(temp, addr1, addr2);
  }
  if (ctx->OR()) {
    code = code || instruction::OR(temp, addr1, addr2);
  }
  putAddrDecor(ctx, temp);
  putOffsetDecor(ctx, "");
  putCodeDecor(ctx, code);
  DEBUG_EXIT();
}

void CodeGenListener::enterParenthesis(AslParser::ParenthesisContext *ctx) {
  DEBUG_ENTER();
}

void CodeGenListener::exitParenthesis(AslParser::ParenthesisContext *ctx) {
  std::string     addr = getAddrDecor(ctx->expr());
  instructionList code = getCodeDecor(ctx->expr());

  putAddrDecor(ctx, addr);
  putOffsetDecor(ctx, "");
  putCodeDecor(ctx, code);
  DEBUG_EXIT();
}


void CodeGenListener::enterValue(AslParser::ValueContext *ctx) {
  DEBUG_ENTER();
}
void CodeGenListener::exitValue(AslParser::ValueContext *ctx) {
  instructionList code;
  std::string temp = "%"+codeCounters.newTEMP();
  if (ctx->INTVAL())
    code = instruction::ILOAD(temp, ctx->INTVAL()->getText());
  else if (ctx->CHARVAL()) { 
    std::string c = ctx->CHARVAL()->getText();
    c = c.substr(1, c.size()-2);
    code = instruction::CHLOAD(temp, c);
  } 
  else if (ctx->FLOATVAL())
    code = instruction::FLOAD(temp, ctx->FLOATVAL()->getText());
  else if (ctx->BOOLVAL()) {
    std::string val = ctx->BOOLVAL()->getText();
    if (val == "true") code = instruction::ILOAD(temp, "1");
    else code = instruction::ILOAD(temp, "0");
  }
  putAddrDecor(ctx, temp);
  putOffsetDecor(ctx, "");
  putCodeDecor(ctx, code);
  DEBUG_EXIT();
}

void CodeGenListener::enterExprIdent(AslParser::ExprIdentContext *ctx) {
  DEBUG_ENTER();
}
void CodeGenListener::exitExprIdent(AslParser::ExprIdentContext *ctx) {
  putAddrDecor(ctx, getAddrDecor(ctx->ident()));
  putOffsetDecor(ctx, getOffsetDecor(ctx->ident()));
  putCodeDecor(ctx, getCodeDecor(ctx->ident()));
  DEBUG_EXIT();
}

void CodeGenListener::enterExprAAccess(AslParser::ExprAAccessContext *ctx) {
  DEBUG_ENTER();
}

void CodeGenListener::exitExprAAccess(AslParser::ExprAAccessContext *ctx) {
  std::string temp = "%"+codeCounters.newTEMP();  
  std::string addr = getAddrDecor(ctx->array_access());
  std::string offs = getOffsetDecor(ctx->array_access());
  instructionList code = getCodeDecor(ctx->array_access());
  code = code || instruction::LOADX(temp, addr, offs);
  putAddrDecor(ctx, temp);
  putOffsetDecor(ctx, "");
  putCodeDecor(ctx, code);
  DEBUG_EXIT();
}

void CodeGenListener::enterArray_access(AslParser::Array_accessContext *ctx) {
  DEBUG_ENTER();
}

void CodeGenListener::exitArray_access(AslParser::Array_accessContext *ctx) {

  putAddrDecor(ctx, getAddrDecor(ctx->ident()));
  std::string addrA = getAddrDecor(ctx->expr());
  putOffsetDecor(ctx, addrA);
  instructionList code1 = getCodeDecor(ctx->ident());
  instructionList code2 = getCodeDecor(ctx->expr());
  putCodeDecor(ctx, code1 || code2);
  DEBUG_EXIT();
}



void CodeGenListener::enterIdent(AslParser::IdentContext *ctx) {
  DEBUG_ENTER();
}
void CodeGenListener::exitIdent(AslParser::IdentContext *ctx) {
  std::string ident = ctx->ID()->getText();
  TypesMgr::TypeId t = Symbols.getType(ident);
  bool b = Symbols.isParameterClass(ident);
  if (Types.isArrayTy(t) and b) {
    std::string temp = "%"+codeCounters.newTEMP();
    instructionList code = instruction::LOAD(temp, ident);  
    putAddrDecor(ctx, temp);  
    putOffsetDecor(ctx, "");
    putCodeDecor(ctx, code); 
  }
  else {
  putAddrDecor(ctx, ctx->ID()->getText());
  putOffsetDecor(ctx, "");
  putCodeDecor(ctx, instructionList());
  DEBUG_EXIT();
  }
}

// void CodeGenListener::enterEveryRule(antlr4::ParserRuleContext *ctx) {
//   DEBUG_ENTER();
// }
// void CodeGenListener::exitEveryRule(antlr4::ParserRuleContext *ctx) {
//   DEBUG_EXIT();
// }
// void CodeGenListener::visitTerminal(antlr4::tree::TerminalNode *node) {
//   DEBUG(">>> visit " << node->getSymbol()->getLine() << ":" << node->getSymbol()->getCharPositionInLine() << " CodeGen TerminalNode");
// }
// void CodeGenListener::visitErrorNode(antlr4::tree::ErrorNode *node) {
// }


// Getters for the necessary tree node atributes:
//   Scope, Type, Addr, Offset and Code
SymTable::ScopeId CodeGenListener::getScopeDecor(antlr4::ParserRuleContext *ctx) {
  return Decorations.getScope(ctx);
}
TypesMgr::TypeId CodeGenListener::getTypeDecor(antlr4::ParserRuleContext *ctx) {
  return Decorations.getType(ctx);
}
std::string CodeGenListener::getAddrDecor(antlr4::ParserRuleContext *ctx) {
  return Decorations.getAddr(ctx);
}
std::string  CodeGenListener::getOffsetDecor(antlr4::ParserRuleContext *ctx) {
  return Decorations.getOffset(ctx);
}
instructionList CodeGenListener::getCodeDecor(antlr4::ParserRuleContext *ctx) {
  return Decorations.getCode(ctx);
}

// Setters for the necessary tree node attributes:
//   Addr, Offset and Code
void CodeGenListener::putAddrDecor(antlr4::ParserRuleContext *ctx, const std::string & a) {
  Decorations.putAddr(ctx, a);
}
void CodeGenListener::putOffsetDecor(antlr4::ParserRuleContext *ctx, const std::string & o) {
  Decorations.putOffset(ctx, o);
}
void CodeGenListener::putCodeDecor(antlr4::ParserRuleContext *ctx, const instructionList & c) {
  Decorations.putCode(ctx, c);
}
