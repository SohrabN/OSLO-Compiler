/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2022
* Author: Sohrab Najafzadeh
* Professors: Paulo Sousa / George Kriger / Abdullah Kadri
************************************************************
*/

/*
************************************************************
* File name: parser.h
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013, 014]
* Assignment: A12, A22, A32.
* Date: Jan 01 2022
* Professor: Paulo Sousa
* Purpose: This file is the main header for Parser (.h)
*************************************************************/

#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif
#ifndef BUFFER_H_
#include "Buffer.h"
#endif
#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global vars */
static Token lookahead;
oslo_int syntaxErrorNumber = 0;
extern Buffer* stringLiteralTable;
extern oslo_int line;
extern Token tokenizer();
extern oslo_char* keywordTable[];
oslo_char variableTable[100][100];
oslo_int variableCount = 0;

#define STR_LANGNAME "OSLO"

/* TO_DO: Create ALL constants for keywords (sequence given in table.h) */

/* Constants */
enum KEYWORDS {
	NO_ATTR = -1,
	AND,
	BREAK,
	DEF,
	IF,
	ELIF,
	ELSE,
	FALSE,
	FOR,
	FROM,
	IMPORT,
	is,
	NONE,
	NOT,
	OR,
	RETURN,
	TRUE,
	WHILE,
	INT
};

/* Function definitions */
oslo_null startParser();
oslo_null matchToken(oslo_int, oslo_int);
oslo_null syncErrorHandler(oslo_int);
oslo_null printError();

/* TODO: Place ALL non-terminal function declarations */
oslo_null assignmentExpression();
oslo_null assignmentStatement();
oslo_null codeSession();
oslo_null dataSession();
oslo_null optVarListDeclarations();
oslo_null optionalStatements();
oslo_null outputStatement();
oslo_null outputVariableList();
oslo_null program();
oslo_null statement();
oslo_null statements();
oslo_null statementsPrime();
oslo_null arithmeticExpression();
oslo_null stringExpression();
oslo_null primaryStringExpression();
oslo_null stringExpressionPrime();
oslo_null varListDeclarations();
oslo_null varListDeclaration();
oslo_null varListDeclarationsPrime();
oslo_null primaryArithmeticExpression();
oslo_null additiveArithmeticExpression();
oslo_null multiplicativeArithmeticExpression();
oslo_null additiveArithmeticExpressionPrime();
#endif
