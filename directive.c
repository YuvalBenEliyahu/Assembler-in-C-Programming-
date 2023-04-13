#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>



#include "utils.h"


/* This function finds the first directive in a given line of code, starting from a specified index. 
 * It returns the type of the directive if it is found, NONE_DIRE if no directive is found, and ERROR_DIRE if an invalid directive is found.
 */
directive find_directive_from_index(line_info line, int *index)
{
	
	char temp[MAX_LINE];/* Define a character array for storing the temporary string */

	
	int j;
	directive result;/* Define a variable for storing the result of the function */

	
	MOVE_TO_NOT_WHITE(line.content, *index) /* get index to first not white place */
	
	/* Try to read a string that starts with a dot from the line, starting from the current index */
	j = sscanf(line.content+(*index),".%s",temp);
	
	/* If no such string is found, return NONE_DIRE */
	if (j != 1)
		return NONE_DIRE;
		
		
	(*index) += strlen(temp) +1;/*advance the index to be after the directive, +1 for after the dot*/

	/* Check if the directive name is valid, and return the type of the directive */
	if ((result = find_directive_by_name(temp)) != NONE_DIRE) 
		return result;
		
	/* If the directive name is invalid, print an error message and return ERROR_DIRE */
	printf_line_error(line, "Invalid directive name: %s", temp);
	return ERROR_DIRE; 
}


/* This function receives a line of code, a string, and a buffer to store the validated string.
 * It checks if the given string is a valid string in assembly language (enclosed in double quotation marks),
 * and returns 1 if it is valid or 0 if it is not valid.
 */
int checkValidString(line_info line, char *string,char *str)
{
	
	char *ptrString = string;/*pointer to the given string*/
	
	/* Check if the string is empty */
	if (string == NULL) 
	{
		printf_line_error(line,"string must recive input");
		return 0;
	}
	ptrString += skipSpacesAndTabs(ptrString);/*skip the first spaces*/
	
	
	
	if(ptrString[0] != '"')/*check for quotation marks at the beggining of the string*/
	{
		printf_line_error(line,"need quotation marks at the beggining of the string %s first char %c\n", ptrString,ptrString[0]);
		return 0;
	}
	
	ptrString += 1;/*advance the pointer after the first quotation mark*/
	strcpy(str,ptrString);/*copy the string for output*/

	/* Find the second double quotation mark in the string */
	if ((ptrString = strchr(str,'"')) == NULL)
	{
		printf_line_error(line,"need quotation marks at the ending of the string");
		return 0;
	}
	ptrString[0]= '\0';/* Replace the second double quotation mark with a null character */

	ptrString += 1;/*advance the pointer after the second quotation mark*/
  	ptrString += skipSpacesAndTabs(ptrString);/*skip spaces after quotation mark*/

	/* Check if there is any extraneous text after the second double quotation mark */
	if(ptrString[0]=='\0')
	{

		return 1;/* Return 1 if the string is valid */
	}
	else
	{

		printf_line_error(line,"extranious text after ending quotation marks of the string");
		return 0;
	}	

		
	
}


/* This function receives an integer of the line number and a string,
 * and returns an array of integers (or NULL if invalid data) */
int checkValidData(line_info line, char *data,short* numbers)
{

	
	char *token; /* Pointer to numbers in data */
	char *endptr; /* Pointer to what comes after the numbers */
	long number; /*the number*/
	int num_numbers = 0; /* Index of the numbers in the data */
	char *ptrData = data; /* Pointer to the data string */
	int dataLen;  /* The length of the data */
	int countNum = 0;  /* Number of integers in the array */
	
	
	/* Check if the data is empty */
	if (data == NULL) 
	{
		printf_line_error(line, ".data must recive input");
		return 0;
	}
	
	removeSpaces(ptrData);/*remove all spaces*/
	
	dataLen = strlen(ptrData);/*the length of the data string(without spaces)*/
	

	/* Check if the input string starts or ends with commas*/
	if (ptrData[0] == ',' || ptrData[dataLen-1] == ',') 
	{
		printf_line_error(line,"(%s) illegal comma.",ptrData);
		return 0;
	}
  
	/* Check if the data has consecutive commas*/
	if (strstr(data, ",,") != 0) 
	{
		printf_line_error(line,"(%s) consecutive commas.",ptrData);
		return 0;
	}

	/* Get the first number */
	token = strtok(ptrData, ",");
	while (token != 0)
	{
	
		
		/* Check if the token is an integer */
		number = strtol(token, &endptr, 10);
		if (*endptr != '\0') 
		{
			
			printf_line_error(line,"(%s) not an integer",endptr);
			return 0;
		}
		/* Check if the number is short  */
   		if (number < SHRT_MIN_14_BITS || number > SHRT_MAX_14_BITS ) 
   		{
			printf_line_error(line,"%d value out of range",number);
     			return 0;
    		}
		/* Add the number to the array */
		numbers[num_numbers++] = (short)number;
		countNum++;
		/* Get the next token */
		token = strtok(NULL, ",");
	}
	
	return countNum; /* Return the number of integers in the array */
}

/* This function receives an array of short integers and adds them to the output array.
 * It also increments the DC counter by the number of elements added to the output array.
 */
void insertDataToArray(short* input, short* output,int* dc, int count) 
{
	int i;
	for(i = 0; i < count; i++,(*dc)++)
		output[*dc] = input[i];

	
}

/* This function receives a string and adds it to the output array as an array of short integers,
 * where each character in the string is represented by its ASCII code.
 * It also increments the DC counter by the number of characters in the string plus one.*/
void insertStringToArray(char* input, short* output,int* dc) 
{
	int i;
	for(i=0;input[i]!='\0';i++,(*dc)++)
		output[*dc] = (short)input[i];

	output[*dc] = '\0';	/* add '\0' at the end */
	(*dc)++;
		 
}

