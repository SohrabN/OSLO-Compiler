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
* File name: scanner.h
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa
* Course: CST 8152 � Compilers, Lab Section: [011, 012, 013, 014]
* Assignment: A22, A32.
* Date: Feb 21 2022
* Professor: Paulo Sousa
* Purpose: This file contains all functionalities from Scanner.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

 /* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
  * to suppress the warnings about using "unsafe" functions like fopen()
  * and standard sting library functions defined in string.h.
  * The define does not have any effect in Borland compiler projects.
  */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

  /* #define NDEBUG        to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Buffer.h"
#endif

#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/*
----------------------------------------------------------------
TO_DO: Global vars definitions
----------------------------------------------------------------
*/

/* Global objects - variables */
/* This buffer is used as a repository for string literals. */
extern BufferPointer stringLiteralTable;	/* String literal table */
oslo_int line;								/* Current line number of the source code */
extern oslo_int errorNumber;				/* Defined in platy_st.c - run-time error number */
static oslo_char keyFlag;

extern oslo_int stateType[];
extern oslo_char* keywordTable[];
extern PTR_ACCFUN finalStateTable[];
extern oslo_int transitionTable[][TABLE_COLUMNS];

/* Local(file) global objects - variables */
static BufferPointer lexemeBuffer;			/* Pointer to temporary lexeme buffer */
static BufferPointer sourceBuffer;			/* Pointer to input source buffer */

/*
 ************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ***********************************************************
 */

oslo_int startScanner(BufferPointer psc_buf) {
	//defensive programming.
	if (psc_buf == NULL) {
		return EXIT_FAILURE;
	}
	/* in case the buffer has been read previously  */
	bRecover(psc_buf);
	bClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS; /*0*/
}

/*
 ************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states
 *		in the Transition Diagram).
 ***********************************************************
 */

Token tokenizer(void) {


	Token currentToken = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	oslo_char c;	/* input symbol */
	oslo_int state = 0;		/* initial state of the FSM */
	oslo_int lexStart;		/* start offset of a lexeme in the input char buffer (array) */
	oslo_int lexEnd;		/* end offset of a lexeme in the input char buffer (array)*/

	oslo_int lexLength;		/* token length */
	oslo_int i;				/* counter */
	oslo_char newc;			/* new char */

	while (1) { /* endless loop broken by token returns it will generate a warning */
		c = bGetChar(sourceBuffer);

		/* ------------------------------------------------------------------------
			Part 1: Implementation of token driven scanner.
			Every token is possessed by its own dedicated code
			-----------------------------------------------------------------------
		*/

		/* TO_DO: All patterns that do not require accepting functions */
		switch (c) {

			/* Cases for spacexs */
		case ' ':
			break;
		case '  ':
			break;
		case '\t':
			break;
		case '\f':
			break;
		case '\n':
			line++;
			currentToken.code = EOS_T;
			return currentToken;
		case ':':
			currentToken.code = COLON_T;
			break;

			/* Cases for symbols */
		case '=':
			currentToken.code = ASS_OP_T;
			return currentToken;
		case '!':
			currentToken.code = NOT_T;
			return currentToken;
		case ',':
			currentToken.code = COM_T;
			return currentToken;
		case MNIDPREFIX1:
			currentToken.code = LPR_T;
			return currentToken;
		case MNIDPREFIX2:
			currentToken.code = RPR_T;
			return currentToken;
		case '{':
			currentToken.code = LBR_T;
			return currentToken;
		case '}':
			currentToken.code = RBR_T;
			return currentToken;
			/*case '#':
				currentToken.code = CID_T;
				return currentToken;*/
		case '+':
			newc = bGetChar(sourceBuffer);
			if (newc == '"') {
				currentToken.code = SCC_OP_T;
				return currentToken;
			}
			bRetract(sourceBuffer);
			currentToken.code = ART_OP_T;
			currentToken.attribute.codeType = OP_ADD;
			return currentToken;
		case '-':
			newc = bGetChar(sourceBuffer);
			if (newc == '"') {
				currentToken.code = SCC_OP_T;
				return currentToken;
			}
			bRetract(sourceBuffer);
			currentToken.code = ART_OP_T;
			currentToken.attribute.codeType = OP_SUB;
			return currentToken;
		case '/':
			newc = bGetChar(sourceBuffer);
			bRetract(sourceBuffer);
			currentToken.code = ART_OP_T;
			currentToken.attribute.codeType = OP_DIV;
			return currentToken;
		case '*':
			newc = bGetChar(sourceBuffer);
			bRetract(sourceBuffer);
			currentToken.code = ART_OP_T;
			currentToken.attribute.codeType = OP_MUL;
			return currentToken;
		case '>':
			newc = bGetChar(sourceBuffer);
			bRetract(sourceBuffer);
			currentToken.code = REL_OP_T;
			currentToken.attribute.codeType = OP_GT;
			return currentToken;
		case '<':
			newc = bGetChar(sourceBuffer);
			bRetract(sourceBuffer);
			currentToken.code = REL_OP_T;
			currentToken.attribute.codeType = OP_LT;
			return currentToken;
			/* AND operator */
		case '&':
			if (bGetChar(sourceBuffer) == '&') {
				currentToken.code = LOG_OP_T;
				currentToken.attribute.codeType = OP_AND;
				return currentToken;
			}
			/* OR operator */
		case '|':
			if (bGetChar(sourceBuffer) == '|') {
				currentToken.code = LOG_OP_T;
				currentToken.attribute.codeType = OP_OR;
				return currentToken;
			}
			/* Comments */
			/*case '#':
				newc = bGetChar(sourceBuffer);
				do {
					c = bGetChar(sourceBuffer);
					if (c == CHARSEOF0 || c == CHARSEOF255) {
						bRetract(sourceBuffer);
						currentToken.code = CID_T;
						return currentToken;
					}
				} while (c != '#' && c != CHARSEOF0 && c != CHARSEOF255 && c !='\n');
				break;*/
				/* Cases for END OF FILE */
		case CHARSEOF0:
			currentToken.code = SEOF_T;
			currentToken.attribute.seofType = SEOF_0;
			return currentToken;
		case CHARSEOF255:
			currentToken.code = SEOF_T;
			currentToken.attribute.seofType = SEOF_255;
			return currentToken;

			/* ------------------------------------------------------------------------
				Part 2: Implementation of Finite State Machine (DFA) or Transition Table driven Scanner
				Note: Part 2 must follow Part 1 to catch the illegal symbols
				-----------------------------------------------------------------------
			*/


		default: // general case
			state = nextState(state, c);
			lexStart = bGetReadPos(sourceBuffer) - 1;
			bSetMark(sourceBuffer, lexStart);
			int pos = 0;
			while (stateType[state] == NOAS) {
				c = bGetChar(sourceBuffer);
				state = nextState(state, c);
				pos++;
			}
			if (stateType[state] == ASWR)
				bRetract(sourceBuffer);
			lexEnd = bGetReadPos(sourceBuffer);
			lexLength = lexEnd - lexStart;
			lexemeBuffer = bCreate((short)lexLength + 2, 0, MODE_FIXED);
			if (!lexemeBuffer) {
				fprintf(stderr, "Scanner error: Can not create buffer\n");
				exit(1);
			}
			bRestore(sourceBuffer);
			for (i = 0; i < lexLength; i++)
				bAddChar(lexemeBuffer, bGetChar(sourceBuffer));
			bAddChar(lexemeBuffer, BUFFER_EOF);
			currentToken = (*finalStateTable[state])(bGetContent(lexemeBuffer, 0));
			bDestroy(lexemeBuffer);
			return currentToken;
		} // switch

	} //while

} // tokenizer


/*
 ************************************************************
 * Get Next State
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	(*) assert() is a macro that expands to an if statement;
	if test evaluates to false (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:
	(*) Assertion failed: test, file filename, line linenum.
	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUG is used - see the top of the file.
 ***********************************************************
 */


oslo_int nextState(oslo_int state, oslo_char c) {
	//printf("%c\n", c);
	/*if (c == '\n')
		printf("hello\n");*/
	oslo_int col;
	oslo_int next;
	col = nextClass(c);
	/*if (c == '\n')
		keyFlag = 0;
	if (col == 5 && keyFlag == 1) {
		col = 1;
	}*/
	next = transitionTable[state][col];
	if (DEBUG)
		printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
	assert(next != IS);
	if (DEBUG)
		if (next == IS) {
			printf("Scanner Error: Illegal state:\n");
			printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
			exit(1);
		}
	return next;
}

/*
 ************************************************************
 * Get Next Token Class
	* Create a function to return the column number in the transition table:
	* Considering an input char c, you can identify the "class".
	* For instance, a letter should return the column for letters, etc.
 ***********************************************************
 */
 /* TO_DO: Use your column configuration */

 /* Adjust the logic to return next column in TT */
 /*	[A-z](0), [0-9](1),	_(2), &(3), "(4), SEOF(5), other(6) */

oslo_int nextClass(oslo_char c) {
	oslo_int val = -1;
	switch (c) {
	case CHRCOL2:
		val = 2;
		break;
	case CHRCOL3_1:
	case CHRCOL3_2:
	case CHRCOL3_3:
		val = 3;
		break;
	case CHRCOL4_1:
	case CHRCOL4_2:
	case CHRCOL4_3:
		val = 4;
		break;
	case CHRCOL5:
		val = 5;
		break;
	case CHRCOL6:
		val = 6;
		break;
	case CHRCOL7:
		val = 7;
		break;
	case CHRCOL8_1:
	case CHRCOL8_2:
		val = 8;
		break;
	case CHRCOL9:
		val = 9;
		break;
	case CHRCOL10:
		val = 10;
		break;
	case CHRCOL11:
		val = 11;
		break;
	case CHRCOL12_1:
	case CHRCOL12_2:
	case CHRCOL12_3:
	case CHRCOL12_4:
	case CHRCOL12_5:
	case CHRCOL12_6:
		val = 12;
		break;
	case CHARSEOF0:
	case CHARSEOF255:
		val = 14;
		break;
	default:
		if (isalpha(c))
			val = 0;
		else if (isdigit(c))
			val = 1;
		else
			val = 13;
	}
	return val;
}


/*
 ************************************************************
 * Acceptance State Function ID
 *		In this function, the pattern for IDs must be recognized.
 *		Since keywords obey the same pattern, is required to test if
 *		the current lexeme matches with KW from language.
 *	- Remember to respect the limit defined for lexemes (VID_LEN) and
 *	  set the lexeme to the corresponding attribute (vidLexeme).
 *    Remember to end each token with the \0.
 *  - Suggestion: Use "strncpy" function.
 ***********************************************************
 */
 /* QUESTION: WHEN I GET HERE DO I HAVE = IN MY lexeme char array? */

Token funcVID(oslo_char lexeme[]) {
	Token currentToken = { 0 };
	size_t length = strlen(lexeme);
	//printf("IN funcVID, lexeme is %s\n", lexeme);
	//if (lexeme[length - 1] == ')')
	//{
	//	if (!isalpha(lexeme[0])) {
	//		currentToken.code = ERR_T;
	//		return currentToken;
	//	}
	//	for (size_t i = 1; i < length - 2; i++)
	//	{
	//		if (!isdigit(lexeme[i]) && !isalpha(lexeme[i])) {
	//			currentToken.code = ERR_T;
	//			return currentToken;
	//		}

	//	}
	//	//lexeme[length - 1] = ' ';
	//	//bRetract(sourceBuffer);
	//	currentToken.code = VID_T;
	//	strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
	//	currentToken.attribute.idLexeme[VID_LEN] = CHARSEOF0;
	//	return currentToken;
	//}
	//else 
	if (lexeme[length - 1] == ',') {
		for (size_t i = 1; i < length - 1; i++)
		{
			if (!isdigit(lexeme[i]) && !isalpha(lexeme[i])) {
				currentToken.code = ERR_T;
				return currentToken;
			}

		}
		//bRetract(sourceBuffer);
		currentToken.code = VID_T;
		//lexeme[length - 1] = ' ';
		strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
		currentToken.attribute.idLexeme[VID_LEN] = CHARSEOF0;
		return currentToken;
	}
	else if (lexeme[length - 1] == '}' || lexeme[length - 1] == ']') {
		currentToken.code = VID_T;
		//lexeme[length - 1] = ' ';
		strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
		currentToken.attribute.idLexeme[VID_LEN] = CHARSEOF0;
		return currentToken;
	}
	else
	{
		if (length >= VID_LEN) {
			currentToken.code = ERR_T;
			return currentToken;
		}
		char lastch = lexeme[length - 1];
		if (!isalpha(lexeme[0])) {
			currentToken.code = ERR_T;
			return currentToken;
		}
		//length-1 because of '='
		for (size_t i = 1; i < length - 1; i++)
		{
			if (!isdigit(lexeme[i]) && !isalpha(lexeme[i])) {
				currentToken.code = ERR_T;
				return currentToken;
			}

		}
		currentToken.code = VID_T;
		//lexeme[length - 1] = ' ';
		strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
		currentToken.attribute.idLexeme[VID_LEN] = CHARSEOF0;
		return currentToken;
	}
}



Token funcMID(oslo_char lexeme[]) {
	Token currentToken = { 0 };
	size_t length = strlen(lexeme);
	size_t firstPerentesis = 0, firstSquareBrackets = 0, firstCurlyBrackets = 0;
	size_t secondPerentesis = 0, secondSquareBrackets = 0, secondCurlyBrackets = 0;
	oslo_char flagLeftParenthesis = 0, flagLeftSquareBrackets = 0, flagLeftCurlyBrackets = 0;
	oslo_char flagRightParenthesis = 0, flagRightSquareBrackets = 0, flagRightCurlyBrackets = 0;
	for (size_t i = 0; i < length; i++)
	{
		if (i > 0 && lexeme[i] == '(') {
			flagLeftParenthesis = 1;
			firstPerentesis = i;
			i = length;
		}
		if (i > 0 && lexeme[i] == '[') {
			flagLeftSquareBrackets = 1;
			firstSquareBrackets = i;
			i = length;
		}
		if (i > 0 && lexeme[i] == '{') {
			flagLeftCurlyBrackets = 1;
			firstCurlyBrackets = i;
			i = length;
		}
	}
	for (size_t i = 0; i < length; i++)
	{
		if (flagLeftParenthesis == 1 && i > 1 && lexeme[i] == ')') {
			flagRightParenthesis = 1;
			secondPerentesis = i;
		}
		if (flagLeftSquareBrackets == 1 && i > 1 && lexeme[i] == ']') {
			flagRightSquareBrackets = 1;
			secondSquareBrackets = i;
		}
		if (flagLeftCurlyBrackets == 1 && i > 1 && lexeme[i] == '}') {
			flagRightCurlyBrackets = 1;
			secondCurlyBrackets = i;
		}
	}
	if (flagLeftParenthesis == 1 && flagRightParenthesis == 1)
	{
		size_t i = 0;
		while (isalpha(lexeme[i]))
		{
			i++;
		}
		
		currentToken = funcKEY(lexeme);
		if (currentToken.code == ERR_T) {
			currentToken.code = MNID_T;
			if (firstPerentesis < VID_LEN)
			{
				strncpy(currentToken.attribute.idLexeme, lexeme, firstPerentesis);
				currentToken.attribute.idLexeme[firstPerentesis] = CHARSEOF0;
				for (size_t i = 0; i <= secondPerentesis - firstPerentesis; i++)
				{
					bRetract(sourceBuffer);
				}

			}
			else
			{
				strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
				currentToken.attribute.idLexeme[VID_LEN] = CHARSEOF0;
			}
		}
		else {
			for (size_t i = 0; i <= secondPerentesis - firstPerentesis; i++)
			{
				bRetract(sourceBuffer);
			}
		}

	}
	else if ((flagLeftSquareBrackets == 1 && flagRightSquareBrackets == 1) || (flagLeftCurlyBrackets == 1 && flagRightCurlyBrackets == 1)) {
		oslo_char* temp = NULL;
		temp = (oslo_char*)malloc(strlen(lexeme) + 1);
		if (flagLeftSquareBrackets == 1 && flagRightSquareBrackets == 1) {
			strncpy(temp, lexeme, firstSquareBrackets);
			temp[firstSquareBrackets] = '\0';
			for (size_t i = 0; i <= secondSquareBrackets - firstSquareBrackets; i++)
			{
				bRetract(sourceBuffer);
			}
		}
		else if (flagLeftCurlyBrackets == 1 && flagRightCurlyBrackets == 1) {
			strncpy(temp, lexeme, firstCurlyBrackets);
			temp[firstCurlyBrackets] = '\0';
			for (size_t i = 0; i <= secondCurlyBrackets - firstCurlyBrackets; i++)
			{
				bRetract(sourceBuffer);
			}
		}
		currentToken = funcVID(temp);
		free(temp);
		return currentToken;
	}
	else
	{

		currentToken.code = ERR_T;



		//}

	//}
	/*else if (flagLeftParenthesis != 1) {

	}*/
	//printf("here1");
	//
	}
	return currentToken;
}



Token funcLs(oslo_char lexeme[]) {
	//printf("%s\n", lexeme);
	Token currentToken = { 0 };
	if (lexeme != NULL) {
		size_t length = strlen(lexeme);
		oslo_char flagPoint = 0;
		if (!isdigit(lexeme[0]))
		{
			currentToken.code = ERR_T;
			return currentToken;
		}
		//length-1 because last char must be \n
		int lastNum = -1;
		for (int i = 1; i < length - 1; i++)
		{

			if (isdigit(lexeme[i]))
			{
				lastNum = i;
			}

		}
		//printf("lastNum is %d\n", lastNum);
		for (int i = 1; i <= lastNum; i++)
		{
			if (!isdigit(lexeme[i]) && lexeme[i] != '.' && lexeme[i] != '\n')
			{
				printf("Here5");
				currentToken.code = ERR_T;
				return currentToken;
			}
			else if (lexeme[i] == '.' && flagPoint == 0) {
				flagPoint = 1;
				if (!isdigit(lexeme[i + 1]))
				{
					printf("Here4");
					currentToken.code = ERR_T;
					return currentToken;
				}
			}
			else if (lexeme[i] == '.' && flagPoint == 1) {
				printf("Here3");
				currentToken.code = ERR_T;
				return currentToken;
			}
		}
		/*if (lexeme[length - 1] != '\n') {
			printf("Here2");
			currentToken.code = ERR_T;
			return currentToken;
		}*/
		//else 
			if (flagPoint==0) {
			bRetract(sourceBuffer);
			currentToken.code = LS_T;
			currentToken.attribute.intValue = (oslo_int)atoi(lexeme);
			currentToken.idAttribute.flags = 1;
			//currentToken.attribute.floatValue = -1;
			return currentToken;
		}
		else
		{
			bRetract(sourceBuffer);
			//bRetract(sourceBuffer);
			currentToken.code = LS_T;
			currentToken.attribute.floatValue = (oslo_float)atof(lexeme);
			currentToken.idAttribute.flags = 2;
			return currentToken;
		}
	}
	else
	{
		printf("Here1");
		currentToken.code = ERR_T;
		return currentToken;
	}

}


Token funcSL(oslo_char lexeme[]) {
	Token currentToken = { 0 };
	oslo_int i = 0, len = (oslo_int)strlen(lexeme);
	if (lexeme != NULL) {
		size_t length = strlen(lexeme);
		if (lexeme[0] != '"' && lexeme[length - 1] != '"')
		{
			//printf("Here 1");
			currentToken.code = ERR_T;
			return currentToken;
		}
		for (i = 1; i < len - 1; i++) {
			if (lexeme[i] == '\n')
				line++;
			if (!bAddChar(stringLiteralTable, lexeme[i])) {
				currentToken.code = RTE_T;
				strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
				errorNumber = RTE_CODE;
				return currentToken;
			}
		}
		if (!bAddChar(stringLiteralTable, CHARSEOF0)) {
			currentToken.code = RTE_T;
			strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
			errorNumber = RTE_CODE;
			return currentToken;
		}
		currentToken.code = STR_T;
		return currentToken;
	}
	else
	{
		//printf("Here 3");
		currentToken.code = ERR_T;
		return currentToken;
	}
}

/*
************************************************************
 * Acceptance State Function SL
 *		Function responsible to identify SL (string literals).
 * - The lexeme must be stored in the String Literal Table
 *   (stringLiteralTable). You need to include the literals in
 *   this structure, using offsets. Remember to include \0 to
 *   separate the lexemes. Remember also to incremente the line.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for SL */

Token funcCID(oslo_char lexeme[]) {
	Token currentToken = { 0 };
	oslo_int i = 0, len = (oslo_int)strlen(lexeme);
	currentToken.attribute.contentString = bGetWritePos(stringLiteralTable);
	if (lexeme[0] == '#')
	{
		if (lexeme[len - 1] == '\n') {
			currentToken.code = CID_T;
			return currentToken;
		}
		else
		{
			currentToken.code = ERR_T;
			return currentToken;
		}
	}
	else
	{
		currentToken.code = ERR_T;
		return currentToken;
	}


}


/*
************************************************************
 * This function checks if one specific lexeme is a keyword.
 * - Tip: Remember to use the keywordTable to check the keywords.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for Keywords */

Token funcKEY(oslo_char lexeme[]) {
	Token currentToken = { 0 };
	oslo_int kwindex = -1, j = 0;
	oslo_int len = (int)strlen(lexeme);
	for (int i = 0; i < len; i++)
	{
		if (!isalpha(lexeme[i]))
		{
			j = i;
			i = len;
		}
	}
	/*size_t i = 0;
	oslo_char* tempPtr = (oslo_char*)malloc(j * sizeof(oslo_char));
	for (i = 0; i <= j; i++)
	{
		tempPtr[i] = lexeme[i];
	}
	tempPtr[i] = '\0';*/
	oslo_char tempPtr[100] = " ";
	strncpy(tempPtr, lexeme, j);
	tempPtr[j + 1] = '\0';
	//printf("Currently in funcKEY, j is %d", j);
	for (j = 0; j < KWT_SIZE; j++) {
		if (!strcmp(tempPtr, &keywordTable[j][0]))
			kwindex = j;
		if (!strcmp(lexeme, &keywordTable[j][0]))
			kwindex = j;
	}
	if (kwindex != -1) {

		currentToken.code = KW_T;
		currentToken.attribute.codeType = kwindex;
		//strcpy(currentToken.attribute.idLexeme, lexeme);
	}
	else {
		currentToken = funcVID(lexeme);
		if (currentToken.code == ERR_T) {
			currentToken = funcErr(lexeme);
		}
		return currentToken;
	}
	return currentToken;
}

/*
************************************************************
 * Acceptance State Function Error
 *		Function responsible to deal with ERR token.
 * - This function uses the errLexeme, respecting the limit given
 *   by ERR_LEN. If necessary, use three dots (...) to use the
 *   limit defined. The error lexeme contains line terminators,
 *   so remember to increment line.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for Errors */

Token funcErr(oslo_char lexeme[]) {
	Token currentToken = { 0 };
	oslo_int i = 0, len = (oslo_int)strlen(lexeme);
	if (len > ERR_LEN) {
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = CHARSEOF0;
		strcat(currentToken.attribute.errLexeme, "...");
	}
	else {
		strcpy(currentToken.attribute.errLexeme, lexeme);
	}
	for (i = 0; i < len; i++) {
		if (lexeme[i] == '\n') {
			line++;
			currentToken.code = ERR_T;
			return currentToken;
		}
	}
	return currentToken;
}


/*
 ************************************************************
 * The function prints the token returned by the scanner
 ***********************************************************
 */

oslo_null printToken(Token t) {
	extern oslo_char* keywordTable[]; /* link to keyword table in */
	switch (t.code) {
	case RTE_T:
		printf("RTE_T\t\t%s", t.attribute.errLexeme);
		/* Call here run-time error handling component */
		if (errorNumber) {
			printf("%d", errorNumber);
			exit(errorNumber);
		}
		printf("\n");
		break;
	case ERR_T:
		printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case MNID_T:
		printf("MNID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case VID_T:
		printf("VID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("STR_T\t\t%d\t ", (short)t.attribute.codeType);
		printf("%s\n", bGetContent(stringLiteralTable,
			(short)t.attribute.codeType));
		break;
	case LPR_T:
		printf("LPR_T\t\t(\n");
		break;
	case RPR_T:
		printf("RPR_T\t\t)\n");
		break;
	case LBR_T:
		printf("LBR_T\t\t{\n");
		break;
	case RBR_T:
		printf("RBR_T\t\t}\n");
		break;
	case KW_T:
		printf("KW_T\t\t%s\n", keywordTable[t.attribute.codeType]);
		break;
	case ASS_OP_T:
		printf("ASS_OP_T\t=\n");
		break;
	case EOS_T:
		printf("EOS_T\t\t\\n\n");
		break;
	case CID_T:
		printf("CID_T\t\t#\n");
		break;
	case COM_T:
		printf("COM_T\t\t,\n");
		break;
	case NOT_T:
		printf("NOT_T\t\t!\n");
		break;
	case ART_OP_T:
		if (t.attribute.codeType == OP_SUB)
			printf("ART_OP_T\t-\n");
		else if (t.attribute.codeType == OP_ADD)
			printf("ART_OP_T\t+\n");
		else if (t.attribute.codeType == OP_MUL)
			printf("OP_MUL\t\t*\n");
		else if (t.attribute.codeType == OP_DIV)
			printf("OP_DIV\t\t/\n");
		break;
	case LS_T:
		if(t.attribute.floatValue==0 && t.attribute.intValue == 0)
			printf("LS_T\t\t%d\n", t.attribute.intValue);
		else if(t.idAttribute.flags==2)
			printf("LS_T\t\t%f\n", t.attribute.floatValue);
		else if (t.idAttribute.flags == 1)
			printf("LS_T\t\t%d\n", t.attribute.intValue);
		break;
	case COLON_T:
		printf("COLON_T\t\t:\n");
		break;
	case REL_OP_T:
		if (t.attribute.codeType == OP_GT)
			printf("OP_GT\t\t>\n");
		else if (t.attribute.codeType == OP_LT)
			printf("OP_LT\t\t<\n");
		break;
	default:
		printf("Scanner error: invalid token code: %d\n", t.code);
	}
}
