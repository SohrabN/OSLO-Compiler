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
* File name: scanner.h
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa
* Course: CST 8152 � Compilers, Lab Section: [011, 012, 013, 014]
* Assignment: A22, A32.
* Date: Feb 21 2022
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20   /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1   /* Value for run-time error */

enum TOKENS {
	ERR_T,		/*  0: Error token */
	MNID_T,		/*  1: Method name identifier token */
	STR_T,		/*  2: String literal token */
	LS_T,		/*	3: Literal value */
	LPR_T,		/*  4: Left parenthesis token */
	RPR_T,		/*  5: Right parenthesis token */
	LBR_T,		/*  6: Left brace token */
	RBR_T,		/*  7: Right brace token */
	KW_T,		/*  8: Keyword token */
	EOS_T,		/*  9: End of statement (\n) */
	RTE_T,		/* 10: Run-time error token */
	SEOF_T,		/* 11: Source end-of-file token */
	SCC_OP_T,	/* 12: String concatenation operator token: (++) */
	ASS_OP_T,	/* 13: Assignment operator token */
	ART_OP_T,	/* 14: Arithmetic operator token */
	REL_OP_T,	/* 15: Relational operator token */
	LOG_OP_T,	/* 16: Logical operator token */
	COM_T,		/* 17: Comma token */
	VID_T,		/* 18: Variable token */
	CID_T,		/* 20: Comment ID */
	MINUS_T,	/* 21: Minus token */
	PLUS_T,		/* 22: Plus token */
	COLON_T,	/* 23: Colon token */
	NOT_T		/* 24: Not token (!) */

};

/* Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	oslo_int codeType;						/* integer attributes accessor */
	AriOperator arithmeticOperator;			/* arithmetic operator attribute code */
	RelOperator relationalOperator;			/* relational operator attribute code */
	LogOperator logicalOperator;			/* logical operator attribute code */
	EofOperator seofType;					/* source-end-of-file attribute code */
	oslo_int intValue;						/* integer literal attribute (value) */
	oslo_int keywordIndex;					/* keyword index in the keyword table */
	oslo_int contentString;					/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	oslo_float floatValue;					/* floating-point literal attribute (value) */
	oslo_char idLexeme[VID_LEN + 1];		/* variable identifier token attribute */
	oslo_char errLexeme[ERR_LEN + 1];		/* error token attribite */
} TokenAttribute;

/* Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	oslo_flag flags;			/* Flags information */
	union {
		oslo_int intValue;				/* Integer value */
		oslo_float floatValue;			/* Float value */
		oslo_null* stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	oslo_int code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFF

/*  Special case tokens processed separately one by one in the token-driven part of the scanner
 *  '=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' , ';',
 *  white space, @comment@ , ',' , ';' , '-' , '+' , '*' , '/', ## ,
 *  .&., .|. , .!. , SEOF.
 */

 /* TO_DO: Error states and illegal state */
#define ES  14		/* Error state with no retract */
#define ER  15		/* Error state with retract */
#define IS -1		/* Illegal state */

 /* TO_DO: State transition table definition */
#define TABLE_COLUMNS 15

/* TO_DO: Define lexeme FIXED classes */
/* These constants will be used on nextClass */
#define CHRCOL2 '_'
#define CHRCOL3_1 '('
#define CHRCOL3_2 '{'
#define CHRCOL3_3 '['
#define CHRCOL4_1 ')'
#define CHRCOL4_2 '}'
#define CHRCOL4_3 ']'
#define CHRCOL5 '"'
#define CHRCOL6 '#'
#define CHRCOL7 '\n'
#define CHRCOL8_1 '='
#define CHRCOL8_2 '!'
#define CHRCOL9 '.'
#define CHRCOL10 ' '
#define CHRCOL11 ','
#define CHRCOL12_1 '*'
#define CHRCOL12_2 '+'
#define CHRCOL12_3 '-'
#define CHRCOL12_4 '/'
#define CHRCOL12_5 '>'
#define CHRCOL12_6 '<'

/* These constants will be used on  MID function */
#define MNIDPREFIX1 '('
#define MNIDPREFIX2 ')'

/* TO_DO: Transition table - type of states defined in separate table */
static oslo_int transitionTable[][TABLE_COLUMNS] = {
	/*   [A-z] , [0-9],    _,  {(,    )},	 ",    #,   \n,[!,=],       .,  ' ',  ',' , [*+-/], other,   '\0',
		   L(0),  D(1), U(2),M1(3),M2(4), Q(5), C(6), N(7), A(8),	P(9),SP(10),SC(11), AO(12), O(13),EOF(14)   */
		{     1,	 6,   ES,   ES,   ES,	10,   12,     0,  ES,      ES,   0,     ES,     ES,    ES,    ER}, // S0:		NOAS
		{     1,     1,    1,    3,    2,   ES,   ES,     5,   2,      ES,   5,      2,      2,     6,    ER}, // S1:		NOAS
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,    IS,  IS,      IS,  IS,     IS,     IS,    IS,    IS}, // S2:		ASNR (VID)
		{     3,     3,    3,    3,    4,    3,   12,     3,   3,       3,   3,      3,      3,     3,    ER}, // S3:		NOAS
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,    IS,  IS,      IS,  IS,     IS,     IS,    IS,    IS}, // S4:		ASNR (MNID)
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,    IS,  IS,      IS,  IS,     IS,     IS,    IS,    IS}, // S5:		ASWR (KEY)
		{    ES,     6,   ES,    9,   ES,   ES,   ES,     9,  ES,       7,   9,     ES,      9,    ES,    ER}, // S6:		NOAS
		{    ES,     8,   ES,   ES,   ES,   ES,   ES,    ES,  ES,      ES,  ES,     ES,     ES,    ES,    ER}, // S7:		NOAS
		{    ES,     8,   ES,   ES,   ES,   ES,   ES,     9,  ES,      ES,   9,     ES,      9,    ES,    ER}, // S8:		NOAS
	 	{    IS,    IS,   IS,   IS,   IS,   IS,   IS,    IS,  IS,      IS,  IS,     IS,     IS,    IS,    IS}, // S9:		ASNR (Ls)
		{    10,    10,   10,   10,   10,   11,   10,    10,  10,      10,  10,     ES,     10,    10,    ER}, // S10:		NOAS
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,    IS,  IS,      IS,  IS,     IS,     IS,    IS,    IS}, // S11:		ASNR (SL)
		{    12,    12,   12,   12,   12,   12,   12,    13,  12,      12,  12,     ES,     12,    12,    ER}, // S12:		NOAS
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,    IS,  IS,      IS,  IS,     IS,     IS,    IS,    IS}, // S13:		ASNR (CID)
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,    IS,  IS,      IS,  IS,     IS,     IS,    IS,	  IS}, // S14:		ASNR (Err)
		{    IS,    IS,   IS,   IS,   IS,   IS,   IS,    IS,  IS,      IS,  IS,     IS,     IS,    IS,	  IS}  // S15:		ASWR (Err)
};

/* Define accepting states types */
#define NOAS	0		/* not accepting state */
#define ASNR	1		/* accepting state with no retract */
#define ASWR	2		/* accepting state with retract */

/* TO_DO: Define list of acceptable states */
static oslo_int stateType[] = {
	NOAS, /* 00 */
	NOAS, /* 01 */
	ASWR, /* 02 (VID) */
	NOAS, /* 03 */
	ASNR, /* 04 (MID) */
	ASWR, /* 05 (KEY) */
	NOAS, /* 06 */
	NOAS, /* 07 */
	NOAS, /* 08 */
	ASNR, /* 09 (Ls) */
	NOAS, /* 10 */
	ASNR, /* 11 (SL) */
	NOAS, /* 12 */
	ASNR, /* 13 (CID) */
	ASNR, /* 14 (Err) */
	ASNR  /* 15 (Err) */
};

/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
oslo_int startScanner(BufferPointer psc_buf);
static oslo_int nextClass(oslo_char c);			/* character class function */
static oslo_int nextState(oslo_int, oslo_char);		/* state machine function */

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* TO_DO: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(oslo_char* lexeme);

/* Declare accepting states functions */
Token funcVID(oslo_char lexeme[]);
Token funcMID(oslo_char lexeme[]);
Token funcKEY(oslo_char lexeme[]);
Token funcLs(oslo_char lexeme[]);
Token funcSL(oslo_char lexeme[]);
Token funcCID(oslo_char lexeme[]);
Token funcErr(oslo_char lexeme[]);

/*
 * Accepting function (action) callback table (array) definition
 * If you do not want to use the typedef, the equvalent declaration is:
 */

 /* TO_DO: Define final state table */
static PTR_ACCFUN finalStateTable[] = {
	NULL,		/* -    [00] */
	NULL,		/* -    [01] */
	funcVID,	/* VID	[02] */
	NULL,		/* -    [03] */
	funcMID,	/* MID  [04] */
	funcKEY,	/* KEY  [05] */
	NULL,		/* -    [06] */
	NULL,		/* -    [07] */
	NULL,		/* -    [08] */
	funcLs,		/* Ls   [09] */
	NULL,		/* -    [10] */
	funcSL,		/* -    [11] */
	NULL,		/* -    [12] */
	funcCID,	/* CID    [13] */
	funcErr,	/* ERR1 [14] */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* Define the number of Keywords from the language */
#define KWT_SIZE 18

/* Define the list of keywords */
static oslo_char* keywordTable[KWT_SIZE] = {
	"and",		//0
	"break",	//1
	"def",		//2
	"if",		//3
	"elif",		//4
	"else",		//5
	"False",	//6
	"for",		//7
	"from",		//8
	"import",	//9
	"is",		//10
	"None",		//11
	"not",		//12
	"or",		//13
	"return",	//14
	"True",		//15
	"while",	//16
	"int"		//17
};

#endif
