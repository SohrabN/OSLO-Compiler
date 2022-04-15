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
* File name: mainBuffer.c
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013, 014]
* Assignment: A12, A22, A32.
* Date: Jan 01 2022
* Professor: Paulo Sousa
* Purpose: This file contains all functionalities from Parser.
* Function list: (...).
************************************************************
*/

/* TODO: Adjust the function header */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef PARSER_H_
#include "Parser.h"
#endif

/*
************************************************************
 * Process Parser
 ***********************************************************
 */
 /* TODO: This is the function to start the parser - check your program definition */

oslo_null startParser() {
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*
 ************************************************************
 * Match Token
 ***********************************************************
 */
 /* TODO: This is the main code for match - check your definition */
oslo_null matchToken(oslo_int tokenCode, oslo_int tokenAttribute) {
	oslo_int matchFlag = 1;
	switch (lookahead.code) {
	case CID_T:
		while (lookahead.code == tokenCode) {
			//printf("%s%s\n", STR_LANGNAME, ": Comment parsed");
			lookahead = tokenizer();
		}
		break;
	case KW_T:
	default:
		if (lookahead.code != tokenCode)
			matchFlag = 0;
	}
	if (matchFlag && lookahead.code == SEOF_T)
		return;
	if (matchFlag) {
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else
		syncErrorHandler(tokenCode);
}

/*
 ************************************************************
 * Syncronize Error Handler
 ***********************************************************
 */
 /* TODO: This is the function to handler error - adjust basically datatypes */
oslo_null syncErrorHandler(oslo_int syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*
 ************************************************************
 * Print Error
 ***********************************************************
 */
 /* TODO: This is the function to error printing - adjust basically datatypes */
oslo_null printError() {
	Token t = lookahead;
	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case MNID_T:
		printf("%s\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("%s\n", bGetContent(stringLiteralTable, t.attribute.contentString));
		break;
	case KW_T:
		printf("%s\n", keywordTable[t.attribute.codeType]);
		break;
	case LPR_T:
	case RPR_T:
	case LBR_T:
	case RBR_T:
	case EOS_T:
		printf("NA\n");
		break;
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
	}
}

/*
 ************************************************************
 * Program statement
 * BNF: <program> -> PLATYPUS { <opt_statements> }
 * FIRST(<program>)= {KW_T (MAIN)}.
 ***********************************************************
 */
oslo_null program() {
	switch (lookahead.code) 
	{
	case CID_T:
		matchToken(CID_T, NO_ATTR);
		switch (lookahead.code)
		{
		case KW_T:
			if (lookahead.attribute.codeType == 2)
			{
				matchToken(KW_T, NO_ATTR);
		case MNID_T:
			if (strncmp(lookahead.attribute.idLexeme, "main", 4) == 0) {
				matchToken(MNID_T, NO_ATTR);
				matchToken(LPR_T, NO_ATTR);
				matchToken(RPR_T, NO_ATTR);
				matchToken(LBR_T, NO_ATTR);
				if (lookahead.code == EOS_T)
					matchToken(EOS_T, NO_ATTR);
				dataSession();
				codeSession();
				matchToken(RBR_T, NO_ATTR);
				break;
			}
			else {
				printError();
			}
			}
		case SEOF_T:
			; // Empty
			break;
		default:
			printError();
		}
		break;
	case KW_T:
		if (lookahead.attribute.codeType == 2) 
		{
			matchToken(KW_T, NO_ATTR);
			case MNID_T:
				if (strncmp(lookahead.attribute.idLexeme, "main", 4) == 0) {
					matchToken(MNID_T, NO_ATTR);
					matchToken(LPR_T, NO_ATTR);
					matchToken(RPR_T, NO_ATTR);
					matchToken(LBR_T, NO_ATTR);
					if (lookahead.code == EOS_T)
						matchToken(EOS_T, NO_ATTR);
					dataSession();
					codeSession();
					matchToken(RBR_T, NO_ATTR);
					break;
				}
				else {
					printError();
				}
		}
	case SEOF_T:
		; // Empty
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}


/*
 ************************************************************
 * dataSession
 * BNF: <dataSession> -> DATA { <opt_varlist_declarations> }
 * FIRST(<program>)= {KW_T (DATA)}.
 ***********************************************************
 */
oslo_null dataSession() {
	/*matchToken(KW_T, DATA);
	matchToken(LBR_T, NO_ATTR);*/
	optVarListDeclarations();
	//matchToken(RBR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Data Session parsed");
}

/*
 ************************************************************
 * Optional Var List Declarations
 * BNF: <opt_varlist_declarations> -> <varlist_declarations> | e
 * FIRST(<opt_varlist_declarations>) = { e, KW_T (INT), KW_T (FLOAT), KW_T (STRING)}.
 ***********************************************************
 */
oslo_null optVarListDeclarations() {
	switch (lookahead.code) {
	case VID_T:
		varListDeclarations();
		break;
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional Variable List Declarations parsed");
}

oslo_null varListDeclarations() {
	varListDeclaration();
	varListDeclarationsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Variable Lists Declarations parsed");
}

oslo_null varListDeclaration() {
	switch (lookahead.code) {
	case VID_T:
		strcpy(variableTable[variableCount], lookahead.attribute.idLexeme);
		variableCount++;
		matchToken(VID_T, NO_ATTR);
	case ASS_OP_T:
		matchToken(ASS_OP_T, NO_ATTR);
		switch (lookahead.code)
		{
		case KW_T:
			if (lookahead.attribute.codeType == 11) {
				matchToken(KW_T, NO_ATTR);
				if (lookahead.code == EOS_T) {
					matchToken(EOS_T, NO_ATTR);
				}
			}
			break;
		case LS_T:
			matchToken(LS_T, NO_ATTR);
			if (lookahead.code == EOS_T) {
				matchToken(EOS_T, NO_ATTR);
			}
			break;
		case STR_T:
			matchToken(STR_T, NO_ATTR);
			if (lookahead.code == EOS_T) {
				matchToken(EOS_T, NO_ATTR);
			}
		default:
			printError();
		}
		break;
	default:
		printError();
	}
}

oslo_null varListDeclarationsPrime() {
	oslo_int found = 0;
	switch (lookahead.code) {
	case VID_T:
		for (size_t i = 0; i < variableCount; i++)
		{
			//printf("variableTable is: %s\nlookahead.attribute.idLexeme is: %s\nCurrent count is: %d\n", variableTable[i], lookahead.attribute.idLexeme,variableCount);
			if (strcmp(variableTable[i],lookahead.attribute.idLexeme)==0) {
				found = 1;
				break;
			}
		}
		if (!found) {
			varListDeclaration();
			varListDeclarationsPrime();
		}
		//printf("idLexeme is %s\n",lookahead.attribute.idLexeme);
		break;

	default:
		; //empty string
	}
}


/*
 ************************************************************
 * codeSession statement
 * BNF: <codeSession> -> CODE { <opt_statements> }
 * FIRST(<codeSession>)= {KW_T (CODE)}.
 ***********************************************************
 */
oslo_null codeSession() {
	/*matchToken(KW_T, CODE);
	matchToken(LBR_T, NO_ATTR);*/
	optionalStatements();
	//matchToken(RBR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Code Session parsed");
}

/* TODO_205: Continue the development (all non-terminal functions) */

/*
 ************************************************************
 * Optional statement
 * BNF: <opt_statements> -> <statements> | ϵ
 * FIRST(<opt_statements>) = { ϵ , IVID_T, FVID_T, SVID_T, KW_T(IF),
 *				KW_T(WHILE), KW_T(READ), KW_T(WRITE) }
 ***********************************************************
 */
oslo_null optionalStatements() {
	//printf("lookahead.code in optionalStatements is %d \n", lookahead.code);
	switch (lookahead.code) {
	case KW_T:
		//if (lookahead.attribute.codeType == ) {
		statements();
		break;
	case MNID_T:
		statements();
		break;
	case VID_T:
		statements();
		break;
		//}
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional statements parsed");
}

/*
 ************************************************************
 * Statements
 * BNF: <statements> -> <statement><statementsPrime>
 * FIRST(<statements>) = { IVID_T, FVID_T, SVID_T, KW_T(IF),
 *		KW_T(WHILE), KW_T(READ), KW_T(WRITE) }
 ***********************************************************
 */
oslo_null statements() {
	statement();
	statementsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}

/*
 ************************************************************
 * Statements Prime
 * BNF: <statementsPrime>  <statement><statementsPrime> | ϵ
 * FIRST(<statementsPrime>) = { ϵ , IVID_T, FVID_T, SVID_T,
 *		KW_T(IF), KW_T(WHILE), KW_T(READ), KW_T(WRITE) }
 ***********************************************************
 */
oslo_null statementsPrime() {
	switch (lookahead.code) {
	case MNID_T:
		//if ((strncmp(lookahead.attribute.idLexeme, "WRITE&", 6) == 0)) {
		statements();
		break;
		//}
	case VID_T:
		statements();
		break;
	default:
		; //empty string
	}
}

/*
 ************************************************************
 * Single statement
 * BNF: <statement> ->  <assignment statement> | <selection statement> |
 *	<iteration statement> | <input statement> | <output statement>
 * FIRST(<statement>) = { IVID_T, FVID_T, SVID_T, KW_T(IF), KW_T(WHILE),
 *			KW_T(READ), KW_T(WRITE) }
 ***********************************************************
 */
oslo_null statement() {
	switch (lookahead.code) {
	case KW_T:
		switch (lookahead.attribute.codeType) {
		default:
			printError();
		}
		break;
	case MNID_T:
		if (strncmp(lookahead.attribute.idLexeme, "print", 5) == 0) {
		outputStatement();
		}
		break;
	case VID_T:
		matchToken(VID_T, NO_ATTR);
		matchToken(ASS_OP_T, NO_ATTR);
		if (lookahead.code == LS_T) {
			matchToken(LS_T, NO_ATTR);
		}
		//else if (lookahead.code==KW_T)
		//{
		//	if (lookahead.attribute.codeType == 17)
		//	{
		//		matchToken(KW_T, NO_ATTR);
		//		if (lookahead.code == LPR_T)
		//		{
		//			matchToken(LPR_T, NO_ATTR);
		//		}else
		//			printError();
		//		if (lookahead.code == MNID_T) {//THIS SHIFT IS NOT WORKING
		//			if (strncmp(lookahead.attribute.idLexeme, "input", 5) == 0)
		//				matchToken(MNID_T, NO_ATTR);
		//			else
		//				printError();
		//			/*if (lookahead.code == LPR_T) {
		//				matchToken(LPR_T, NO_ATTR);
		//			}
		//			else
		//				printError();
		//			if (lookahead.code == RPR_T) {
		//				matchToken(RPR_T, NO_ATTR);
		//			}
		//			else
		//				printError();
		//			if (lookahead.code == RPR_T) {
		//				matchToken(RPR_T, NO_ATTR);
		//			}
		//			else
		//				printError();
		//			if (lookahead.code == EOS_T)
		//			{
		//				matchToken(EOS_T, NO_ATTR);
		//			}*/


		//		}
		//		
		//		break;
		//	}
		//}else
		//	printError();
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

/*
 ************************************************************
 * Assignment Statement
 * BNF: <assignment statement> -> <assignment expression>
 * FIRST(<assignment statement>) = { IVID_T, FVID_T, SVID_T, KW_T(IF),
 *		KW_T(WHILE), KW_T(READ), KW_T(WRITE) }
 ***********************************************************
 */
oslo_null assignmentStatement() {
	assignmentExpression();
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Assignment statement parsed");
}


/*
 ************************************************************
 * Assignment Expression
 * BNF: <assignment expression> -> <integer_variable> = <arithmetic expression>
 *						| <float_variable> = <arithmetic expression>
 *						| <string_variable>= <string expression>
 * FIRST(<assignment expression>) = { IVID_T, FVID_T, SVID_T }
 ***********************************************************
 */
oslo_null assignmentExpression() {
	switch (lookahead.code) {
	case VID_T:
		arithmeticExpression();
		stringExpression();
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Assignment expression parsed");
}

oslo_null arithmeticExpression() {
	switch (lookahead.code) {
	case ART_OP_T:
		switch (lookahead.attribute.codeType) {
		case OP_ADD:
			matchToken(ART_OP_T, OP_ADD);
			break;
		case OP_SUB:
			matchToken(ART_OP_T, OP_SUB);
			break;
		default:
			printError();
		}
		primaryArithmeticExpression();
		break;
	case LPR_T:
	case VID_T:
	case LS_T:
		additiveArithmeticExpression();
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Arithmetic expression parsed");
}

oslo_null primaryArithmeticExpression() {
	switch (lookahead.code) {
	case LS_T:
		arithmeticExpression();
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": primary arithmetic expression parsed");
}
oslo_null additiveArithmeticExpression() {
	multiplicativeArithmeticExpression();
	additiveArithmeticExpressionPrime();
	printf("%s%s\n", STR_LANGNAME, ": Additive arithmetic expression parsed");
}

oslo_null additiveArithmeticExpressionPrime() {
	switch (lookahead.code) {
	case ART_OP_T:
		switch (lookahead.attribute.codeType) {
		case OP_ADD:
			matchToken(ART_OP_T, OP_ADD);
			break;
		case OP_SUB:
			matchToken(ART_OP_T, OP_SUB);
			break;
		default:
			;
		}
	}
	printf("%s%s\n", STR_LANGNAME, ": additive Arithmetic Expression Prime parsed");
}
oslo_null multiplicativeArithmeticExpression() {
	switch (lookahead.code) {
	case LPR_T:
	case VID_T:
	case LS_T:
		primaryArithmeticExpression();
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ":  multiplicative Arithmetic Expression parsed");
}
oslo_null multiplicativeArithmeticExpressionPrime() {
	switch (lookahead.code) {
	case ART_OP_T:
		switch (lookahead.attribute.codeType) {
		case OP_MUL:
			matchToken(ART_OP_T, OP_MUL);
			break;
		case OP_DIV:
			matchToken(ART_OP_T, OP_DIV);
			break;
		default:
			;
		}
	}
	printf("%s%s\n", STR_LANGNAME, ": multiplicative Arithmetic Expression Prime parsed");
}

oslo_null stringExpression() {
	primaryStringExpression();
	stringExpressionPrime();
	printf("%s%s\n", STR_LANGNAME, ": String expression parsed");
}

oslo_null stringExpressionPrime() {
	switch (lookahead.code) {
	case VID_T:
		matchToken(VID_T, NO_ATTR);
		break;
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		break;
	default:
		;
	}
}

oslo_null primaryStringExpression() {
	switch (lookahead.code) {
	case VID_T:
		matchToken(VID_T, NO_ATTR);
		break;
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": primary string expression parsed");
}

/*
 ************************************************************
 * Output Statement
 * BNF: <output statement> -> WRITE (<output statementPrime>);
 * FIRST(<output statement>) = { KW_T(WRITE) }
 ***********************************************************
 */
oslo_null outputStatement() {
	matchToken(MNID_T, NO_ATTR);
	matchToken(LPR_T, NO_ATTR);
	outputVariableList();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
}

/*
 ************************************************************
 * Output Variable List
 * BNF: <opt_variable list> -> <variable list> | ϵ
 * FIRST(<opt_variable_list>) = { IVID_T, FVID_T, SVID_T, ϵ }
 ***********************************************************
 */
oslo_null outputVariableList() {
	switch (lookahead.code) {
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		//printf("%s\n", "SOFIA: Output list (string literal) parsed");
		break;
	default:
		//printf("%s\n", "SOFIA: Output list (empty) parsed");
		;
	}
	printf("%s%s\n", STR_LANGNAME, ": Output variable list parsed");
}
