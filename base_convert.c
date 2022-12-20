#include "base_convert.h"

#define BASE32 32
#define BITS 10
#define HALF_BITS 5
#define BINARY_BASE 2
#define DECIMAL_BASE 9
#define DECIMAL_MIN_VAL 0
#define DECIMAL_MIN_CHAR '0'
#define END_OF_LINE '\0'
#define CHAR_TAB '\t'
#define CHAR_PLUSE '+'
#define CHAR_DOT '.'
#define CHAR_SPACE ' '
#define CHAR_COLON ':'
#define CHAR_HYPHEN '-'




char base32_symbols[BASE32]={'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'};

void swap(char *binary){
	int index;
	char tmp;

	for (index = 0;index < HALF_BITS;index++){
		tmp = binary[DECIMAL_BASE - index];
		binary[DECIMAL_BASE - index] = binary[index];
		binary[index] = tmp;
	} 
}

void complete2(char *binary,int dec_val){
	int index;
	unsigned int val = dec_val;
	for(index = 0;index < BITS;index++){
		binary[index] = val % BINARY_BASE + DECIMAL_MIN_CHAR;
		val = val/BINARY_BASE;
	}
}

void convert_to_base32(char *binary,char *base32){
	int base_index = 0, counter = 0, index = 0, pos = 1;

	for(index = DECIMAL_BASE;index >= DECIMAL_MIN_VAL;index--){
		base_index += pow(BINARY_BASE,counter) * (binary[index] - DECIMAL_MIN_CHAR);
		counter++;

		if(counter == HALF_BITS){
			base32[pos--] = base32_symbols[base_index];	
			base_index = 0;
 			counter = 0;
		}
	}	
}

char *decimal_to_base32(int dec_val){   
	char binary[BITS];
	char *base32 = (char *)malloc((BINARY_BASE + 1)*sizeof(char));

	if(base32 == NULL)
		return "ERROR: Memory allocation failed!\0";

	complete2(binary,dec_val);
	swap(binary);
	convert_to_base32(binary, base32);
	base32[BINARY_BASE] = END_OF_LINE;

	return base32;    
}

char *decimal_to_binary(int dec_val){
	char *binary = (char *)malloc(11*sizeof(char));

	if (binary == NULL)
		return "ERROR: Memory allocation failed!\0";

	complete2(binary,dec_val);
	swap(binary);    
	binary[BITS] = END_OF_LINE;

	return binary;
}

char *binary_to_base32(char *binary){
	char * base32 = malloc(3*sizeof(char));

	if(!base32)
		return "ERROR: Dynamic allocation failed!\0";

	convert_to_base32(binary, base32);
	base32[BINARY_BASE] = END_OF_LINE;
	return base32;
}

int binary_to_decimal(char *binary){ 
	unsigned int index;
	int dec_val = 0;

	for (index = DECIMAL_BASE ; index >= DECIMAL_MIN_VAL ; index--){
		dec_val += pow(BINARY_BASE,DECIMAL_BASE - index) * (binary[index] - DECIMAL_MIN_CHAR);
	}

	if(binary[0] == '1')
		return DECIMAL_MIN_VAL - (pow(BINARY_BASE,BITS) - dec_val);
	return dec_val;
}

int string_to_int(char *str_val, int current_index, int last_index){
	int index = 0;
	int int_val = 0;
	int first_index = current_index;

	if(str_val[first_index] == CHAR_HYPHEN || str_val[first_index] == CHAR_PLUSE )
		first_index++;

	while((last_index - index) >= first_index){
		int_val += pow(BITS,index) * ((int)(str_val[last_index - index] - DECIMAL_MIN_CHAR));
		index++;
	}
	return (str_val[current_index] == CHAR_HYPHEN)? -int_val: int_val;
}
