/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2022
* Author: Svillen Ranev - Paulo Sousa
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
sofia_int syntaxErrorNumber = 0;
extern Buffer* stringLiteralTable;
extern sofia_int line;
extern Token tokenizer();
extern sofia_chr* keywordTable[];

#define STR_LANGNAME "Sofia"

/* TO_DO: Create ALL constants for keywords (sequence given in table.h) */

/* Constants */
enum KEYWORDS {
	NO_ATTR = -1,
	DATA,
	CODE,
	INT,
	FLOAT,
	STRING,
	IF,
	THEN,
	ELSE,
	WHILE,
	DO
};

/* Function definitions */
sofia_nul startParser();
sofia_nul matchToken(sofia_int, sofia_int);
sofia_nul syncErrorHandler(sofia_int);
sofia_nul printError();

/* TODO: Place ALL non-terminal function declarations */
sofia_nul assignmentExpression();
sofia_nul assignmentStatement();
sofia_nul codeSession();
sofia_nul dataSession();
sofia_nul optVarListDeclarations();
sofia_nul optionalStatements();
sofia_nul outputStatement();
sofia_nul outputVariableList();
sofia_nul program();
sofia_nul statement();
sofia_nul statements();
sofia_nul statementsPrime();

#endif
