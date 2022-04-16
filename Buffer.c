/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2022
* Author: Sohrab Najafzadeh
* Professors: Paulo Sousa / George Kriger / Abdullah Kadri
************************************************************
*/

/*
***********************************************************
* File name: buffer.c
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013, 014]
* Assignment: A12.
* Date: Jan 01 2022
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer (A12)
************************************************************
*/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Buffer.h"
#endif

/*
***********************************************************
* Function name: bCreate
* Purpose: Creates the buffer according to capacity, increment
	factor and operational mode ('f', 'a', 'm')
* Author: Svillen Ranev / Paulo Sousa
* History/Versions: Ver 2.1
* Called functions: calloc(), malloc()
* Parameters:
*   size = initial capacity
*   increment = increment factor
*   mode = operational mode
* Return value: bPointer (pointer to Buffer)
* Algorithm: Allocation of memory according to inicial (default) values.
* TO_DO:
*	- Adjust datatypes for your LANGUAGE.
*   - Use defensive programming
*	- Check boundary conditions
*	- Check flags.
*************************************************************
*/

BufferPointer bCreate(oslo_int size, oslo_int increment, oslo_int mode) {
	BufferPointer b;
	//defensive programming to check size variable is not smaller than zero and it not bigger than maximum value allowed by Oslo
	if (size<0 || size>OSLO_MAX_SIZE)
		return NULL;
	//if mode or increamnt or size is not seet it will default to fixed mode and default values
	if (!size || !increment ||!mode) {
		size = BUFFER_DEFAULT_SIZE;
		increment = BUFFER_DEFAULT_INCREMENT;
		mode = MODE_FIXED;
	}
	//allocates space in memory and initilizes the BufferPointer variable.
	b = (BufferPointer)calloc(1, sizeof(Buffer));
	//defensive programming if BufferPointer is null it will stop the function.
	if (b == NULL)
		return NULL;
	//allocates  space in memory for pointer to the string.
	b->string = (oslo_char*)malloc(size);
	//defensive programming to check if pointer to string is not null. if it is null it will free the space in memory and stop the function.
	if (!b->string) {
		free(b);
		return NULL;
	}
	//setting the values received as parametes into the object.
	b->size = size;
	b->increment = increment;
	b->mode = mode;
	b->flags = OSLO_DEFAULT_FLAG;
	//returning buffer pointer.
	return b;
}


/*
***********************************************************
* Function name: bAddChar
* Purpose: Adds a char to buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   ch = char to be added
* Return value:
*	bPointer (pointer to Buffer)
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

BufferPointer bAddChar(BufferPointer const pBuffer, oslo_char ch) {
	oslo_int newSize = 0;
	oslo_char* tempbuffer;
	if (!pBuffer)
		return NULL;
	pBuffer->flags = pBuffer->flags & OSLO_RST_REL_FLAG;
	//defensive programming checking if current size plus another oslo_char is not going to reach the OSLO_MAX_SIZE
	if (pBuffer->position.writePos + sizeof(oslo_char) == OSLO_MAX_SIZE)
		return NULL;
	//if buffer is full we need to increament the buffer
	if (bIsFull(pBuffer) == OSLO_TRUE) {
		switch (pBuffer->mode) {
		case MODE_FIXED:
			return NULL;
		case MODE_ADDIT:
			newSize = pBuffer->size + pBuffer->increment;
			//defensive programming in case we pass "a" for mode and 0 or negative number as increment as argument
			if (newSize <= pBuffer->size || newSize < 0)
				return NULL;
			break;
		case MODE_MULTI:
			newSize = pBuffer->size * pBuffer->increment;
			//defensive programming in case we pass "m" for mode and 0 or 1 or negative number as increment as argument
			if (newSize <= pBuffer->size || newSize < 0)
				return NULL;
			break;
		}
		//reallocating space for tempbuffer to make sure incase if it wasnt successful we will not lose our data in pBuffer->string
		tempbuffer = (char*)realloc(pBuffer->string, newSize);
		//checking to see if tempbuffer is not null 
		if (!tempbuffer)
			return NULL;
		//sets the relocation flag
		pBuffer->flags = pBuffer->flags | OSLO_SET_REL_FLAG;
		//resets the buffer full flag
		pBuffer->flags = pBuffer->flags & OSLO_RST_FUL_FLAG;
		//passess the new space (new location in memory) to pBuffer->string
		pBuffer->string = tempbuffer;
		pBuffer->size = newSize;
	}
	//adding the new character to the string variable
	pBuffer->string[pBuffer->position.writePos++] = ch;
	//if buffer writePos and size are equal it means buffer is full and sets the buffer full flag.
	if (pBuffer->position.writePos == pBuffer->size) {
		pBuffer->flags = pBuffer->flags | OSLO_SET_FUL_FLAG;
	}
	//checks the character to see if it has reached end of file while it is reading the source code.
	if (ch == BUFFER_EOF)
		pBuffer->flags = pBuffer->flags | OSLO_SET_EOB_FLAG;
	//returns the buffer pointer.
	return pBuffer;
}

/*
***********************************************************
* Function name: bClear
* Purpose: Clears the buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
oslo_bool bClear(BufferPointer const pBuffer) {
	//checks to make sure buffer pointer is not null
	if (!pBuffer)
		return BUFFER_ERROR;
	//sets all the position data fields to 0 to reset.
	pBuffer->position.writePos = pBuffer->position.markPos = pBuffer->position.readPos = 0;
	//resets all the flags to default pBuffer->flags = OSLO_DEFAULT_FLAG also can be used.
	pBuffer->flags = pBuffer->flags & OSLO_RST_EOB_FLAG;
	pBuffer->flags = pBuffer->flags & OSLO_RST_REL_FLAG;
	pBuffer->flags = pBuffer->flags & OSLO_RST_FUL_FLAG;
	pBuffer->flags = pBuffer->flags & OSLO_RST_EMP_FLAG;
	//returns true if bClear function was successful.
	return OSLO_TRUE;
}

/*
***********************************************************
* Function name: bDestroy
* Purpose: Releases the buffer address
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
oslo_bool bDestroy(BufferPointer const pBuffer) {
	//checks to make sure buffer pointer is not null
	if (!pBuffer)
		return BUFFER_ERROR;
	//deleting memory allocated for char pointer inside pBuffer pointer
	free(pBuffer->string);
	//deleting memory allocated for  pBuffer pointer it self
	free(pBuffer);
	//returns true if bDestroy function was successful
	return OSLO_TRUE;
}

/*
***********************************************************
* Function name: bIsFull
* Purpose: Checks if buffer is full
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
oslo_bool bIsFull(BufferPointer const pBuffer) {
	//checks to make sure buffer pointer is not null
	if (!pBuffer)
		return BUFFER_ERROR;
	//checks if buffer full flag has been set
	if (pBuffer->flags & OSLO_CHK_FUL_FLAG) {
		//if buffer is full it will return true
		return OSLO_TRUE;
	}
	else {
		//if buffer is NOT full it will return false
		return OSLO_FALSE;
	}

}

/*
***********************************************************
* Function name: bGetWritePos
* Purpose: Returns the position of char to be added
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	addcPosition value
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
oslo_int bGetWritePos(BufferPointer const pBuffer) {
	//checks to make sure buffer pointer is not null
	if (!pBuffer)
		return BUFFER_ERROR;
	//getter to return pBuffer->position.writePos
	return pBuffer->position.writePos;
}

/*
***********************************************************
* Function name: bGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
oslo_int bGetSize(BufferPointer const pBuffer) {
	//checks to make sure buffer pointer is not null
	if (!pBuffer)
		return BUFFER_ERROR;
	//getter to return pBuffer->size
	return pBuffer->size;
}

/*
***********************************************************
* Function name: bGetMode
* Purpose: Returns the operational mode
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	operational mode.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
oslo_int bGetMode(BufferPointer const pBuffer) {
	//checks to make sure buffer pointer is not null
	if (!pBuffer)
		return BUFFER_ERROR;
	//getter to return pBuffer->mode
	return pBuffer->mode;
}


/*
***********************************************************
* Function name: bGetMarkPos
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	mark offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
oslo_int bGetMarkPos(BufferPointer const pBuffer) {
	//checks to make sure buffer pointer is not null
	if (!pBuffer)
		return BUFFER_ERROR;
	//getter to return pBuffer->position.markPos
	return pBuffer->position.markPos;
}


/*
***********************************************************
* Function name: bSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
oslo_bool bSetMark(BufferPointer const pBuffer, oslo_int mark) {
	//checks to make sure buffer pointer is not null
	if (!pBuffer)
		return BUFFER_ERROR;
	//sets the pBuffer->position.markPos with mark value passed to the function
	pBuffer->position.markPos = mark;
	return OSLO_TRUE;
}

/*
***********************************************************
* Function name: bPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
oslo_int bPrint(BufferPointer const pBuffer) {
	oslo_int size = 1;
	oslo_char c;
	//checks to make sure buffer pointer is not null and char pointer inside buffer pointer is not null either.
	if (!pBuffer || !(pBuffer->string))
		return OSLO_FALSE;
	//returns the next character on buffer using bGetChar function.
	c = bGetChar(pBuffer);
	//loop will continue while c value is bigger than zero and end of buffer is set
	while (c > 0 && !(pBuffer->flags & OSLO_CHK_EOB_FLAG)) {
		//prints the next character.
		printf("%c", c);
		//returns the next character on buffer using bGetChar function.
		c = bGetChar(pBuffer);
		size++;
	}
	//returns number of bytes that was printed from inside the buffer.
	return size;

}

/*
***********************************************************
* Function name: bLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   fi = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
oslo_int bLoad(BufferPointer const pBuffer, FILE* const fi) {
	oslo_int size = 1;
	oslo_char c;
	//checks to make sure buffer pointer is not null and file is not null either.
	if (!pBuffer || !fi)
		return BUFFER_ERROR;
	//read the next character on the file.
	c = (oslo_char)fgetc(fi);
	//while end of file has not been reached loop will continue
	while (!feof(fi)) {
		//if bAddChar returns false indicating there was issue with adding the next character
		if (!bAddChar(pBuffer, c)) {
			ungetc(c, fi);
			return BUFFER_ERROR;
		}
		//read the next character on the file.
		c = (char)fgetc(fi);
		//increamenting number bytes that has been loaded into the buffer
		size++;
	}
	//checks the file for error
	if (ferror(fi))
		return BUFFER_ERROR;
	return size;
}

/*
***********************************************************
* Function name: bIsEmpty
* Purpose: Checks if buffer is empty.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
oslo_bool bIsEmpty(BufferPointer const pBuffer) {
	//checks to make sure buffer pointer is not null
	if (!pBuffer)
		return BUFFER_ERROR;
	//checks to make sure buffer empty flag has been set
	if (pBuffer->flags & OSLO_CHK_EMP_FLAG)
		//if buffer empty flag has been set it will return true
		return OSLO_TRUE;
	else
		//if buffer empty flag has been set it will return false
		return OSLO_FALSE;
}

/*
***********************************************************
* Function name: bGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
oslo_char bGetChar(BufferPointer const pBuffer) {
	//checks to make sure buffer pointer is not null
	if (!pBuffer)
		return BUFFER_ERROR;
	// if position.readPos and position.writePos postions are equal it means end of buffer has been reached we need to set the flag for it
	if (pBuffer->position.readPos == pBuffer->position.writePos) {
		//sets the flag for end of buffer
		pBuffer->flags = pBuffer->flags | OSLO_SET_EOB_FLAG;
		//it will return \0 to indicate end of file has been reached
		return BUFFER_EOF;
	}
	//if position.readPos and position.writePos are not equal means buffer still has data in it that we need to read
	else 
	{
		//resets the flag for end of buffer incase it has been change previously
		pBuffer->flags = pBuffer->flags & OSLO_RST_EOB_FLAG;
		//returns the next string on buffer which will be used for print function
		return pBuffer->string[pBuffer->position.readPos++];
	}
	
}


/*
***********************************************************
* Function name: bRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
oslo_bool bRecover(BufferPointer const pBuffer) {
	//checks to make sure buffer pointer is not null
	if (!pBuffer)
		return BUFFER_ERROR;
	//sets readPos and markPos to zero so buffer would be able to reread again.
	pBuffer->position.readPos = 0;
	pBuffer->position.markPos = 0;
	return OSLO_TRUE;
}


/*
***********************************************************
* Function name: bRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
oslo_bool bRetract(BufferPointer const pBuffer) {
	//checks to make sure buffer pointer is not null and position.readPos value is positive
	if (!pBuffer || pBuffer->position.readPos <= 0)
		return BUFFER_ERROR;
	//decrements position.readPos to go back one postion
	pBuffer->position.readPos--;
	return OSLO_TRUE;
}


/*
***********************************************************
* Function name: bRestore
* Purpose: Resets the buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
oslo_bool bRestore(BufferPointer const pBuffer) {
	//checks to make sure buffer pointer is not null
	if (!pBuffer)
		return BUFFER_ERROR;
	//the function sets readPos offset to the value of the current markPos offset. It will be used when it is necessary to "undo" several reading operations
	pBuffer->position.readPos = pBuffer->position.markPos;
	return OSLO_TRUE;
}


/*
***********************************************************
* Function name: bGetReadPos
* Purpose: Returns the value of getCPosition.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	The readPos offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
oslo_int bGetReadPos(BufferPointer const pBuffer) {
	//checks to make sure buffer pointer is not null
	if (!pBuffer)
		return BUFFER_ERROR;
	//getter to get pBuffer->position.readPos value
	return pBuffer->position.readPos;
}


/*
***********************************************************
* Function name: bGetIncrement
* Purpose: Returns the buffer increment.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	The Buffer increment.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
oslo_int bGetIncrement(BufferPointer const pBuffer) {
	//checks to make sure buffer pointer is not null
	if (!pBuffer)
		return BUFFER_ERROR;
	//getter to get pBuffer->increment value
	return pBuffer->increment;
}


/*
***********************************************************
* Function name: bGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
oslo_char* bGetContent(BufferPointer const pBuffer, oslo_int pos) {
	//checks to make sure buffer pointer is not null
	if (!pBuffer)
		return NULL;
	//returns a pointer to the location of the character buffer indicated by pos that is the distance
	return pBuffer->string + pos;
}


/*
***********************************************************
* Function name: bGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value:
*	Flags from Buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
#define FLAGS_
#undef FLAGS_
#ifndef FLAGS_
oslo_flag bGetFlags(BufferPointer const pBuffer) {
	//checks to make sure buffer pointer is not null
	if (!pBuffer)
		return BUFFER_ERROR;
	//getter to get the flag of pBuffer pointer to object
	return pBuffer->flags;
}
#else
#define bGetFlags(pBuffer) ((pBuffer)?(pBuffer->flags):(RT_FAIL_1))
#endif
