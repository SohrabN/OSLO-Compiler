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
* File name: compilers.h
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013, 014]
* Assignment: A12, A22, A32.
* Date: Jan 01 2022
* Professor: Paulo Sousa
* Purpose: This file defines the functions called by main function.
* Function list: mainBuffer(), mainScanner(), mainParser().
*************************************************************/

#ifndef COMPILERS_H_
#define COMPILERS_H_

#define DEBUG 0

/* Logical constants - adapt for your language */
#define OSLO_TRUE	1
#define OSLO_FALSE 0

/* You should add your own constant definitions here */
#define OSLO_MAX_SIZE				SHRT_MAX-1	/* maximum capacity */ 

/*
------------------------------------------------------------
Data types definitions
------------------------------------------------------------
*/
/* TO_DO: Define your typedefs */
typedef char			oslo_char;
typedef int				oslo_int;
typedef long			oslo_long;
typedef float			oslo_float;
typedef double			oslo_double;
typedef void			oslo_null;
typedef char*			oslo_string;
typedef unsigned char	oslo_bool;
typedef unsigned char	oslo_flag;


/*
------------------------------------------------------------
Programs:
1: Buffer - invokes MainBuffer code
2: Scanner - invokes MainScanner code
3: Parser - invokes MainParser code
------------------------------------------------------------
*/
enum PROGRAMS {
	PGM_BUFFER	= '1',
	PGM_SCANNER = '2',
	PGM_PARSER	= '3'
};

/*
------------------------------------------------------------
Main functions signatures
(Codes will be updated during next assignments)
------------------------------------------------------------
*/
oslo_int mainBuffer	(oslo_int argc, oslo_char** argv);
/*
sofia_int mainScanner	(sofia_int argc, sofia_chr** argv);
sofia_int mainParser	(sofia_int argc, sofia_chr** argv);
*/
#endif
