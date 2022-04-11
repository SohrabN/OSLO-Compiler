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
* File name: Buffer.h
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013, 014]
* Assignment: A12.
* Date: Jan 01 2022
* Professor: Paulo Sousa
* Purpose: This file is the main header for Parser (.h)
************************************************************
*/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#define BUFFER_H_

/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Modes (used to create buffer) */
enum BUFFERMODES {
	MODE_FIXED = 'f',
	MODE_ADDIT = 'a',
	MODE_MULTI = 'm'
};


#define BUFFER_ERROR (-1)						/* General error message */
#define BUFFER_EOF '\0'							/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME (SOFIA) .................................. */

#define BUFFER_DEFAULT_SIZE			200			/* default initial buffer capacity */
#define BUFFER_DEFAULT_INCREMENT	100			/* default increment factor */

/* Add your bit-masks constant definitions here - Defined for SOFIA */
/* BITS                                (7654.3210) */
#define OSLO_DEFAULT_FLAG 0x0F 	/* (0000.1111)_2 = (015)_10 */
#define OSLO_SET_REL_FLAG 0x80
#define OSLO_RST_REL_FLAG 0x7F
#define OSLO_CHK_REL_FLAG 0x80
#define OSLO_SET_EOB_FLAG 0x40
#define OSLO_RST_EOB_FLAG 0xBF
#define OSLO_CHK_EOB_FLAG 0x40
#define OSLO_SET_FUL_FLAG 0x20
#define OSLO_RST_FUL_FLAG 0xDF
#define OSLO_CHK_FUL_FLAG 0x20
#define OSLO_SET_EMP_FLAG 0x10
#define OSLO_RST_EMP_FLAG 0xEF
#define OSLO_CHK_EMP_FLAG 0x10

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME (SOFIA) .................................. */


/* Offset declaration */
typedef struct position {
	oslo_int markPos;			/* the offset (in chars) to the mark location */
	oslo_int readPos;			/* the offset (in chars) to the get-character location */
	oslo_int writePos;			/* the offset (in chars) to the add-character location */
} Position;

/* Buffer structure */
typedef struct buffer {
	oslo_char*	string;		/* pointer to the beginning of character array (character buffer) */
	oslo_int	size;		    /* current dynamic memory size (in bytes) allocated to character buffer */
	oslo_int	increment;		/* character array increment factor */
	oslo_int	mode;			/* operational mode indicator */
	oslo_flag	flags;			/* contains character array reallocation flag and end-of-buffer flag */
	Position	position;		/* Offset / position field */
} Buffer, * BufferPointer;


/* FUNCTIONS DECLARATION:  .................................. */
/* General Operations */
BufferPointer	bCreate(oslo_int, oslo_int, oslo_int);
BufferPointer	bAddChar(BufferPointer const, oslo_char);
oslo_bool		bRetract(BufferPointer const);
oslo_bool		bRestore(BufferPointer const);
oslo_bool		bRecover(BufferPointer const);
oslo_bool		bClear(BufferPointer const);
oslo_bool		bDestroy(BufferPointer const);
oslo_bool		bSetMark(BufferPointer const, oslo_int);
oslo_bool		bIsEmpty(BufferPointer const);
oslo_bool		bIsFull(BufferPointer const);
oslo_bool		bIsRealocated(BufferPointer const);
oslo_int		bPrint(BufferPointer const);
oslo_int		bLoad(BufferPointer const, FILE* const);
/* Getters */
oslo_int		bGetSize(BufferPointer const);
oslo_int		bGetWritePos(BufferPointer const);
oslo_int		bGetMarkPos(BufferPointer const);
oslo_int		bGetReadPos(BufferPointer const);
oslo_int		bGetIncrement(BufferPointer const);
oslo_int		bGetMode(BufferPointer const);
oslo_char		bGetChar(BufferPointer const);
oslo_char*		bGetContent(BufferPointer const, oslo_int);
oslo_flag		bGetFlags(BufferPointer const);

#endif
