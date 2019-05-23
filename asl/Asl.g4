//////////////////////////////////////////////////////////////////////
//
//    Asl - Another simple language (grammar)
//
//    Copyright (C) 2017  Universitat Politecnica de Catalunya
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
//    contact: José Miguel Rivero (rivero@cs.upc.es)
//             Computer Science Department
//             Universitat Politecnica de Catalunya
//             despatx Omega.110 - Campus Nord UPC
//             08034 Barcelona.  SPAIN
//
//////////////////////////////////////////////////////////////////////

grammar Asl;

//////////////////////////////////////////////////
/// Parser Rules
//////////////////////////////////////////////////

// A program is a list of functions
program 
    : function+ EOF
    ;

function
    : FUNC ID '(' (parameters)? ')' (':' basictype)? declarations statements ENDFUNC
    ;

parameters
    : ID ':' type (',' ID ':' type)* 
    ;

declarations
    : (variable_decl)*
    ;

variable_decl
    : VAR ID (',' ID)* ':' type
    ;

type    
		: basictype 
		| array
    ;

basictype
		: INT
		| BOOL
		| FLOAT
		| CHAR
		;

array
		: ARRAY '[' INTVAL ']' OF basictype 
		;

funcall 
		: ident '(' call_parameters? ')'
    ; 

call_parameters
    : (expr (',' expr)*)
    ;

statements
    : (statement)*
    ;

// The different types of instructions
statement
          // Assignment
        : left_expr ASSIGN expr ';'                              # assignStmt
          // if-then-else statement (else is optional)
        | IF expr THEN statements (ELSE statements)? ENDIF       # ifStmt
          // while statement
        | WHILE expr DO statements EWHILE                        #whileStmt
          // A function/procedure call has a list of arguments in parenthesis (possibly empty)
        | funcall ';'	                                           # funCallStmt 
          // Read a variable
        | READ left_expr ';'                                     # readStmt
          // Write an expression
        | WRITE expr ';'                                         # writeExpr
          // Write a string
        | WRITE STRING ';'                                       # writeString
          //return statment
        | RETURN (expr)? ';'                                     # returnStmt
        ;

// Grammar for left expressions (l-values in C++)
left_expr
        : ident
        | array_access 
        ;

// Grammar for expressions with boolean, relational and aritmetic operators
expr  	
    : op=(NOT|PLUS|MINUS) expr			               	   # unary
		| expr op=(MUL|DIV|MOD) expr 		                   # arithmetic
		| expr op=(PLUS|MINUS) expr 		                   # arithmetic
		| expr op=(EQUAL|DIFF|GRT|GRTE|LST|LSTE) expr  	   # relational
		| expr op=AND expr					                       # boolean
		| expr op=OR 	expr					                       # boolean
		| '(' expr ')'						                         # parenthesis
		| INTVAL							                             # value
		| FLOATVAL							                           # value
		| BOOLVAL							                             # value
		| CHARVAL							                             # value
		| ident 							                             # exprIdent
		| array_access 						                         # exprAAccess
		| funcall 							                           # exprFuncall
		;

array_access
		: ident '[' expr ']'
		;

ident   : ID
        ;

//////////////////////////////////////////////////
/// Lexer Rules
//////////////////////////////////////////////////




ASSIGN    : '=' ;
PLUS      : '+' ;
MINUS     : '-' ;
MUL       : '*' ;
DIV 	    : '/' ;
MOD 	    : '%' ;

EQUAL     : '==' ;
DIFF   	  : '!=' ;
GRT 	    : '>' ;
GRTE	    : '>=' ;
LST       : '<' ;
LSTE	    : '<=' ;
NOT 	    : 'not' ;
AND 	    : 'and' ;
OR 	   	  : 'or'  ;

VAR       : 'var';
INT       : 'int';
FLOAT	    : 'float' ;
BOOL 	    : 'bool' ;
CHAR	    : 'char' ;
ARRAY     : 'array' ;
OF 		    : 'of' ;
IF        : 'if' ;
THEN      : 'then' ;
ENDIF     : 'endif' ;
ELSE      : 'else' ;
WHILE     : 'while' ;
DO        : 'do' ;
EWHILE    : 'endwhile' ;
FUNC      : 'func' ;
ENDFUNC   : 'endfunc' ;
READ      : 'read' ;
WRITE     : 'write' ;
RETURN	  : 'return' ;
BOOLVAL   : ('true'|'false') ;
ID        : ('a'..'z'|'A'..'Z'|'_') ('a'..'z'|'A'..'Z'|'_'|'0'..'9')* ;
INTVAL    : ('0'..'9')+ ;
FLOATVAL  : ('0'..'9')+ '.' ('0'..'9')+ ;
CHARVAL	  : '\'' (ESC_SEQ| ~('\\'|'"')) '\'' ;

// Strings (in quotes) with escape sequences
STRING    : '"' ( ESC_SEQ | ~('\\'|'"') )* '"' ;

fragment
ESC_SEQ   : '\\' ('b'|'t'|'n'|'f'|'r'|'"'|'\''|'\\') ;

// Comments (inline C++-style)
COMMENT   : '//' ~('\n'|'\r')* '\r'? '\n' -> skip ;

// White spaces
WS        : (' '|'\t'|'\r'|'\n')+ -> skip ;
// Alternative description
// WS        : [ \t\r\n]+ -> skip ;

