#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>



char *decimal_to_base32(int dec_val); /*input: decimal value
					output: project's base32 value */

char *decimal_to_binary(int dec_val); /*input: decimal value
					output: binary value */

char *binary_to_base32(char *binary); /*input: binary value
					output: project's base32 value */

int binary_to_decimal(char *binary); /*input: binary value
					output: decimal value */

int string_to_int(char *str_val, int current_index, int last_index); /* Input: string with decimal val,string length (starting + last index). 
	 					  			Output: decimal integer value*/
