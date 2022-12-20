#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "assembler_functions.h"
#include "symbol_table.h"
#include "extern_entry_table.h"
#include "code_table.h"
#include "data_table.h"


#define SYMBOL_MAX 30
#define NUM_OF_ORDERS 16
#define NUM_OF_DIRECT 5
#define DIRECT_TABLE 7 
#define DECIMAL_MAX_VAL '9'
#define DECIMAL_MIN_VAL '0'
#define END_OF_LINE '\0'
#define OK 0
#define CHAR_HASH '#'
#define CHAR_TAB '\t'
#define CHAR_PLUSE '+'
#define CHAR_DOT '.'
#define CHAR_COMMA ','
#define CHAR_SPACE ' '
#define CHAR_COLON ':'
#define CHAR_HYPHEN '-'
#define CHAR_PRECENT '%'

/*Pointer to all data tables*/
extern DP data_head;
extern symbol_ptr symbol_head;
extern CP code_head;
extern EAE_ptr EAE_head;

int get_address(char *cmd, int current_index){	
	int index = 0;
	if(cmd[current_index] == CHAR_HASH)
		return 0;
	if(cmd[current_index] == 'r' && cmd[current_index + 1] >= DECIMAL_MIN_VAL && cmd[current_index + 1] <= '7')
		return 3;
	while(cmd[current_index+index] != END_OF_LINE && !isspace(cmd[current_index + index]) && cmd[current_index + index] != CHAR_DOT && cmd[current_index + index] != CHAR_COMMA)
		index++;
	return (cmd[current_index+index] == CHAR_DOT)? 2:1;
}

char *add_to_code_table(unsigned int arr[],int num,int *IC){
	int index = 0;
	char *binary;

	while(index < num){
		binary = decimal_to_binary(arr[index]);
		if(binary[0] == CHAR_PRECENT || create_new_code(&code_head,*IC,binary) == -2)
			return "ERROR: memory allocation failed!\0";
		(*IC) += 1;
		index++;
		
	}
	return NULL;
}

char *count_actions(int instruction_ind, int *count, char *cmd, int current_index){
	int num_of_line = 1,first_address,second_address;
	current_index += 3;
	if(instruction_ind >= 14)	{
		if(instruction_ind == (NUM_OF_ORDERS - 1))
			current_index += 2;
		if(is_line_empty(cmd,current_index))
			return "EROR: unexpected text afer action name.\0";
	}
	else{
		if(!is_line_empty(cmd,current_index))
			return "ERROR: expected command after action name.\0";
		current_index = ignore_space(cmd,current_index);
		first_address = get_address(cmd,current_index);
		switch (first_address){
			case 0:
			if(instruction_ind <= 3 || instruction_ind == 12)
				num_of_line += 1;
			else
				return "ERROR: Address number: 0, wrong address number, cannot execute this action in first address.\0";
			break;
			case 1:
			num_of_line += 1;
			break;
			case 2:
			num_of_line += 2;
			break;
			case 3:
			if(instruction_ind != 6)
				num_of_line += 1;
			else
				return "ERROR: Address number: 3, wrong address number, cannot execute this action in first address.\0";
			break;
		}
		current_index = end_of_line(cmd,current_index);
		if(instruction_ind <= 3 || instruction_ind == 6){
			current_index = ignore_break(cmd,current_index);
			if(current_index == -1)
				return "ERROR: expected comma \",\" after first command, and missing second command.\0";
			current_index = ignore_space(cmd,current_index);
			second_address = get_address(cmd,current_index);
			switch (second_address){
				case 0:
				if(instruction_ind == 1)
					num_of_line += 1;
				else
					return "ERROR: Address number: 0, wrong address number, cannot execute this action in second address.\0";
				break;
				case 1:
				num_of_line += 1;
				break;
				case 2:
				num_of_line += 2;
				break;
				case 3:				
					num_of_line += (first_address == 3)? 0 : 1;				
				break;
			}
		}
		else{
			if(is_line_empty(cmd,current_index))
				return "ERROR: Unexpected extarnous text after end program.\0";
		}
	}
	*count = num_of_line;
	return NULL;
}

int create_files(const char *file_name){
	FILE *file_ptr1,*file_ptr2;
	CP CP_ptr;
	EAE_ptr ptr_EAE;
	char *write_to_file = NULL,*object_file = NULL,*entry_file = NULL,*extern_file = NULL;
	int entry_flag=0,extern_flag=0;
		
	if(is_code_empty(code_head)){		
		object_file = add_extensions(file_name,".ob\0");		
		if(!object_file || !(file_ptr1 = fopen(object_file,"w")))
			return -1;
		fprintf(file_ptr1,"m\tf\n");
		CP_ptr = code_head;
		while(CP_ptr){
			write_to_file = get_code(&CP_ptr);			
			if(!write_to_file){
				fclose(file_ptr1);
				unlink(object_file);
				return -1;
			}
			fprintf(file_ptr1,"%s\n",write_to_file);
		}
		fclose(file_ptr1);
	}
	if(!is_EAE_empty(EAE_head)){
		ptr_EAE = EAE_head;
		while(ptr_EAE){
			entry_flag = (!(ptr_EAE->is_extern) || entry_flag);
			extern_flag = ((ptr_EAE->is_extern) || extern_flag);
			ptr_EAE = ptr_EAE->next;
		}
		if (extern_flag){
			extern_file = add_extensions(file_name,".ext\0");
			if(!extern_file || !(file_ptr1 = fopen(extern_file,"w"))){
				unlink(object_file);
				return -1;
			}
		}
		if(entry_flag){
			entry_file = add_extensions(file_name,".ent\0");
			if(!entry_file || !(file_ptr2 = fopen(entry_file,"w"))){
				unlink(object_file);
				if(extern_file) {
					unlink(extern_file);
			}
				return -1;
		}
	}

		ptr_EAE = EAE_head;
		while(ptr_EAE){
			if(ptr_EAE->is_extern){
				fprintf(file_ptr1,"%s\t%s\n",ptr_EAE->name,ptr_EAE->address);
			}else{
				fprintf(file_ptr2,"%s\t%s\n",ptr_EAE->name,ptr_EAE->address);
			}
			ptr_EAE = ptr_EAE->next;
		}
		if (extern_flag)
			fclose(file_ptr1);
		if(entry_flag)
			fclose(file_ptr2);
	}

	return 1;
}

char* add_extensions(const char *str1,const char *str2){
    char *full_name = malloc(strlen(str1)+strlen(str2)+1);
    if(full_name){
        strcpy(full_name, str1);
        strcat(full_name, str2);
        return full_name;
    }
    else{
        return NULL;
    }
}

int add_data_to_code(int IC){
	CP last = code_head,temp;
	DP head = data_head;
	char *binary;
	if(last)
		while(last->next)
			last = last->next;
	else{
		if(head){
			if((temp = (CP)malloc(sizeof(code))) && (binary = decimal_to_binary(head->data))){
				temp->address = IC + head->DC;
				temp->bin = binary;
				code_head = temp;
				last = temp;
			}
			else{
				return -2;
			}
		}
	}

	while(head){
		if((temp = (CP)malloc(sizeof(code))) && (binary = decimal_to_binary(head->data))){
			temp->address = IC+head->DC;
			temp->bin = binary;
			last->next = temp;
			last = last->next;
		}
		else{
			return -2;
		}
		head = head->next;
	}
	return last->address;
}

char *add_order_to_code(int action_num, char *cmd, int current_index, int *IC){
	unsigned int next_line[5] = {0,0,0,0,0};/*5 lines that we will need to add.*/
	int line_counter = 0;/*Represent the number of line being converted to code.*/
	int result;
	int first_address,second_address,end,i;
	static char symbol_name[SYMBOL_MAX];
	symbol_ptr temp;
	/*frist line*/
	line_counter++;
	next_line[0]|= (action_num<<6);
	current_index += 3;
	if(action_num >= 14){
		if(action_num == 15)
			current_index+=2;			
		if(is_line_empty(cmd,current_index))
			return "ERROR: unexpected extarnous text afer action name.\0";
		else
			return add_to_code_table(next_line,line_counter,IC);
	}
	/*second line*/
	if(!is_line_empty(cmd,current_index))
		return "ERROR: expected command after action name.\0";
	current_index = ignore_space(cmd,current_index);
	first_address = get_address(cmd,current_index);	
	switch (first_address){
		case 0:/*represent integer value "#....."*/
		if(action_num<=3 || action_num == 12){
			current_index++;
			end = 0;
			end = is_integer(cmd,current_index);
			if(end > 0)/*Check for line max char overflow*/{
				result = string_to_int(cmd,current_index,end-1);
				if(result>128 || result<-128)/*Each command use 10 bits(8 bits + 2(A,R,E))*/
					return "ERROR: integer value can be between 128 to -128.\0";	
				next_line[line_counter]|= ((result<<2)&(-4));
				line_counter++;
				current_index = end;
			}
			else
				return "ERROR: expected integer value after '#' in first command.\0";
		}
		else
			return "ERROR: Address number: 0, can't be in first command, at this action.\0";
		break;
		case 1:
		end = 0;
		end = end_of_line(cmd, current_index);
		if(end - current_index >= SYMBOL_MAX)
			return "ERROR: Symbol name overflow, symbol name length is 30 characters only.\0";
		for(i=0;i < (end-current_index) ;i++)
			symbol_name[i] = cmd[current_index+i];
		symbol_name[i] = END_OF_LINE;
		if((temp = get_symbol(symbol_head,symbol_name))){
			next_line[line_counter]|= (temp->address)<<2;
			if(temp->type != 2)/*relocatable label*/
				next_line[line_counter]|= 2;/*10*/
			else{	/* external label*/
				next_line[line_counter]|= 1;/*01*/
				if( add_EAE(&EAE_head, temp->name , (*IC) + line_counter, 1) == -2)
					return "ERROR: Memory allocation failed!\0";
			}
			if(action_num<=3 || action_num == 6)/*check if target or source variable*/
				next_line[0]|= 1<<4;
			else
				next_line[0]|= 1<<2;
			line_counter++;
			current_index = end;
		}
		else
			return "ERROR: Unknown symbol name.\0";
		break;
		case 2:/*dot char*/
		end = 0;
		while(cmd[current_index+end] != CHAR_DOT)
			end++;
		if(end >= SYMBOL_MAX)
			return "ERROR: Symbol name overflow, symbol name length is 30 characters only.\0";
		for(i=0;i<end;i++)
			symbol_name[i] = cmd[current_index+i];
		symbol_name[i] = END_OF_LINE;		
		if((temp = get_symbol(symbol_head,symbol_name))){
			if(temp->type == 1 || temp->type == 2){
				current_index += end+1;
				next_line[line_counter]|= (temp->address)<<2;
				line_counter++;
				if(cmd[current_index] == '1' || cmd[current_index] == '2')
					next_line[line_counter]|= (cmd[current_index] - '0') <<2;
				else
					return "ERROR: Expected struct field type 1 or 2 after: '.' in struct name in first command.\0";
				if(temp->type != 2){/*means ENTERNAL label*/				
					next_line[line_counter-1]|= 2;/*10*/
					next_line[line_counter]|= 0;/*00*/
				}
				else{				/*means EXTERNAL label*/
					next_line[line_counter-1]|= 1;/*01*/
					next_line[line_counter]|= 0;/*00*/
					if(add_EAE(&EAE_head, temp->name , (*IC) + line_counter, 1) == -2)
						return "ERROR: Memory allocation failed!\0";
				}

				if(action_num<=3 || action_num == 6)/*check if target or source*/
					next_line[0]|= 2<<4;
				else
					next_line[0]|= 2<<2;				
				line_counter++;
				current_index++;
			}
			else
				return "ERROR: Unexpected symbol,expected struct symbol.\0";
		}
		else
			return "ERROR: Unknown Symbol in fisrt action.\0";
		break;
		case 3:/*represent register "r*..." */
		if(action_num != 6){
			current_index++;
			if(action_num<=3)/*check if target or source*/{
				next_line[0]|= 3<<4;
				next_line[line_counter]|= (cmd[current_index] - '0')<<6;
			}
			else{
				next_line[0]|= 3<<2;
				next_line[line_counter]|= (cmd[current_index] - '0')<<2;
			}
			line_counter++;
			current_index++;
		}
		else
			return "ERROR: Address number: 0, wrong address number, cannot execute this action in first address\0";
		break;
	}
	/*third line*/
	if(action_num<=3 || action_num == 6){
		current_index = ignore_break(cmd,current_index);
		if(current_index<0)
			return "ERROR: expected comma \",\" after first command, and missing second command.\0";
		current_index = ignore_space(cmd,current_index);
		second_address = get_address(cmd,current_index);
		switch(second_address){
			case 0:
			if(action_num == 1){
				current_index++;
				end = 0;
				end = is_integer(cmd,current_index);
				if(end>0){ 
					result = string_to_int(cmd,current_index,end-1);
					if(result>128 || result<-128)
						return "ERROR: integer value can be between 128 to -128.\0";	
					next_line[line_counter]|= ((result<<2)&(-4));
					line_counter++;
					current_index = end;
				}
				else
					return "ERROR: expected integer value after '#' in second variable.\0";
			}
			else
				return "ERROR: Address number: 0, wrong address number, cannot execute this action in second address.\0";
			break;
			case 1:
			end = 0;
			end = end_of_line(cmd, current_index);
			if(end - current_index >= SYMBOL_MAX)
				return "ERROR: Symbol name overflow, symbol name length is 30 characters only.\0";
			for(i=0;i<end - current_index;i++)
				symbol_name[i] = cmd[current_index+i];
			symbol_name[i] = END_OF_LINE;
			if((temp = get_symbol(symbol_head,symbol_name))){
				next_line[line_counter]|= (temp->address)<<2;
				if(temp->type != 2)/*relocatable label*/
					next_line[line_counter]|= 2;/*10*/
				else{ /*external label*/
					next_line[line_counter]|= 1;/*01*/
					if(add_EAE(&EAE_head, temp->name , (*IC) + line_counter, 1) == -2)
						return "ERROR: Memory allocation failed!\0";
				}				
				next_line[0]|= 1<<2;				
				line_counter++;
				current_index = end;
			}
			else
				return "ERROR: Unknown Symbol in second action.\0";
			break;
			case 2:/*dot char*/
			end = 0;
			while(cmd[current_index+end]!= '.')
				end++;		
			if(end >= SYMBOL_MAX)
				return "ERROR: Symbol name overflow, symbol name length is 30 characters only.\0";
			for(i=0;i<end;i++)
				symbol_name[i] = cmd[current_index+i];
			symbol_name[i] = END_OF_LINE;		
			if((temp = get_symbol(symbol_head,symbol_name))){
				if(temp->type == 1 || temp->type == 2){
					current_index += end+1;
					next_line[line_counter]|= (temp->address)<<2;
					line_counter++;
					if(cmd[current_index] == '1' || cmd[current_index] == '2')
						next_line[line_counter]|= (cmd[current_index] - '0')<<2;
					else
						return "ERROR: expected 1 or 2 after: dot '.' in second variable.\0";
					if(temp->type != 2){/*relocatable label*/				
						next_line[line_counter-1]|= 2;/*10*/
						next_line[line_counter]|= 0;/*00*/
					}
					else{				/*external label*/
						next_line[line_counter-1]|= 1;/*01*/
						next_line[line_counter]|= 0;/*00*/
						if(add_EAE(&EAE_head, temp->name , (*IC) + line_counter, 1) == -2)
							return "ERROR: Memory allocation failed!\0";
					}					
					next_line[0]|= 2<<2;				
					line_counter++;
					current_index++;
				}
				else
					return "ERROR: Missing struct symbol.\0";
			}
			else
				return "ERROR: Unknown Symbol in first action.\0";			
			break;
			case 3:/*represent register "r*..." */
			current_index++;
			next_line[0]|= 3<<2;
			if(first_address == 3)/*check if there place for another word to be added*/{
				next_line[line_counter-1]|= (cmd[current_index] - '0')<<2;
			}
			else{
				next_line[line_counter]|= (cmd[current_index] - '0')<<2;
				line_counter++;
			}
			current_index++;
			break;
		}
	}
	else{
		if(is_line_empty(cmd,current_index))
			return "ERROR: unexpected extarnous text after variable name.\0";
	}
	if(is_line_empty(cmd,current_index))
		return "ERROR: unexpected extarnous text after end of command.\0";
	return add_to_code_table(next_line,line_counter,IC);
}

char *add_to_EAE(char *cmd, int current_index, int end){
	static char name[SYMBOL_MAX];
	symbol_ptr temp = symbol_head;
	int length = end - current_index + 1,i,result;
	for(i=0;i<length;i++){
		name[i] = cmd[current_index+i];
	}
	name[i] = END_OF_LINE;	
	while(temp){		
		if(strcmp(temp->name,name) == 0){			
			if(temp->type == 2)
				return "ERROR: Same variable name for extern and entry symbol.\0";
			result = add_EAE(&EAE_head, temp->name, temp->address, 0);
			if(result == -2)
				return "ERROR: Memory allocation failed!\0";
			else
				return NULL;
		}
		temp = temp->next;
	}
	return "ERROR: Unknown symbol name\0";
}

void free_assembler(){
	free_data(&data_head);
	free_symbol(&symbol_head);
	free_code(&code_head);
	free_EAE(&EAE_head);
}
