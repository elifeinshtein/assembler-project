#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_check.h"
#include "read_file.h"
#include "symbol_table.h"
#include "extern_entry_table.h"
#include "code_table.h"
#include "text_analyze.h"
#include "base_convert.h"
#include "data_table.h"
#include "assembler_functions.h"

#define SYMBOL_MAX 30
#define EAE_SIZE 7
#define PROG_FIRST_ADDER 100



/****************************************************
It is recommended to avoid global variables. (-3)
*****************************************************/
/*Pointer to assembler tables*/
DP data_head = NULL;
symbol_ptr symbol_head = NULL;
CP code_head = NULL;
EAE_ptr EAE_head = NULL;

short errorFlag;
char symbol_name[CMD_MAX];

int first_read(FILE *file)
{
	short symbolFlag = OFF;
	int index, end, flag1, flag2, symbol_sindex, symbol_length, IC = PROG_FIRST_ADDER, DC = 0, count = 0, row = 0;
	char *check_error = NULL, *command = NULL;
	errorFlag = OFF;	
	while((command = get_cmd(file)))
	{
		index = 0;
		count = 0;
		symbolFlag = OFF;
		row++;
		if(!is_line_empty(command,index) || command[ignore_space(command,0)] == ';')
			continue;		
		if(command[index] == '`')
		{
			printf("ERROR! row: %d %s\n",row,(command+index+1) );
			errorFlag = ON;			
			continue;
		}		
		index = ignore_space(command,index);
		end = is_symbol(command,index);
		if(end>0)
		{
			symbolFlag = ON;
			symbol_sindex = index;
			symbol_length = end - index;
			if(symbol_length >= SYMBOL_MAX)
			{
				printf("ERROR! row: %d, col: %d Symbol name can't be longer then %d characters.\n",row,index,SYMBOL_MAX);
				ERROR	
			}
			index = ignore_space(command,end+1);			
			if(!is_line_empty(command,index))
			{
				printf("ERROR! row: %d, col: %d, expected symbol or method name after symbol.\n",row,index);
				ERROR
			}
		}
		if(command[index] == '.')
		{
			flag1 = is_direct_order(command,index);
			if(flag1 <= 0)
			{
				printf("ERROR! row: %d, col: %d, unknow istruction name.\n",row,index);
				ERROR
			}
			if(flag1 <= 3)
			{
				if(symbolFlag)
				{
					symbolFlag = OFF;
					strncpy(symbol_name,(command+symbol_sindex),symbol_length);			
					symbol_name[symbol_length] = '\0';
					if(check_symbol(symbol_head,symbol_name))
					{
						printf("ERROR! row: %d, col: %d, symbol is already defined\n",row,symbol_sindex);						
						ERROR
					}
					if(flag1 == 3)
						flag2 = create_new_symbol(&symbol_head, command, symbol_sindex, symbol_length, DC, 1);/*1 - represent struct.*/
					else
						flag2 = create_new_symbol(&symbol_head, command, symbol_sindex, symbol_length, DC, 0);
					if(flag2 == -2)
					{
						printf("ERROR! row: %d, col: %d Dynamic memory allocation failed! cannot add new symbols.\n",row,symbol_sindex);
						ERROR
					}
				}
				switch(flag1)
				{
					case 1:
					flag2 = add_new_data(command, index, &DC, &data_head);
					if(flag2 < 0 && flag2 != -2)
					{
						printf("ERROR! row: %d, col: %d expected integer values after instruction name.\n",row,index);
						ERROR
					}					
					break;
					case 2:
					flag2 = add_new_string(command, index, &DC, &data_head);
					if(flag2 < 0 && flag2 != -2)
					{
						printf("ERROR! row: %d, col: %d expected \" \" between string after instruction name.\n",row,index);	
						ERROR						
					}					
					break;
					case 3:					
					flag2 = add_new_struct(command, index, &DC, &data_head);
					if(flag2 < 0 && flag2 != -2)
					{
						printf("ERROR! row: %d, col: %d expected integer value,string after instruction name.\n",row,index);
						ERROR
					}					
					break;
				}
				if(flag2 == -2)
				{
					printf("ERROR! row: %d, col: %d Dynamic memory allocation failed! could not add data.\n",row,index);					
					ERROR
				}
				if(is_line_empty(command,flag2))
				{
					printf("ERROR! row: %d, col: %d extranous text after end of command.\n",row,flag2);
					ERROR
				}				
			}
			else
			{
				if(symbolFlag)
				{
					symbolFlag = OFF;
					printf("ERROR! row: %d, col: %d unexpected symbol.\n",row,symbol_sindex);				
				}
				if(flag1 == 5)
				{					
					index += EAE_SIZE;
					if(is_line_empty(command,index))
					{
						index = ignore_space(command,index);
						flag2 = add_symbol(&symbol_head, command, index);
						if(flag2 < 0)
						{
							switch(flag2)
							{
								case -2:
								printf("ERROR! row: %d, col: %d Dynamic memory allocation failed! could not add symbol.\n",row,symbol_sindex);
								ERROR
								break;
								case -3:								
								printf("ERROR! row: %d, col: %d extranous text after symbol name.\n",row,index);
								ERROR
								break;
								default :
								printf("ERROR! row: %d, col: %d expected symbol name.\n",row,index);
								ERROR
							}							
						}
					}					
					else
					{
						printf("ERROR! row: %d, col: %d missing extern symbol name. \n",row,symbol_sindex);
						ERROR
					}					
				}
			}
			free(command);
			continue;
		}
		if(symbolFlag)
		{
			symbolFlag = OFF;
			strncpy(symbol_name,(command+symbol_sindex),symbol_length);					
			symbol_name[symbol_length] = '\0';
			if(check_symbol(symbol_head,symbol_name))
			{
				printf("ERROR! row: %d, col: %d, symbol is already defined.\n",row,symbol_sindex);						
				ERROR
			}			
			flag2 = create_new_symbol(&symbol_head, command, symbol_sindex, symbol_length, IC, 3);/* 3 - represent code Symbol.*/			
			if(flag2 == -2)
			{
				printf("ERROR! row: %d, col: %d Dynamic memory allocation failed! could not add symbol.\n",row,symbol_sindex);
				ERROR
			}
		}
		flag1 = is_order(command,index);
		if(flag1 < 0)
		{
			printf("ERROR! row: %d, col: %d unknown instruction name.\n",row,index);
			ERROR
		}
		check_error = count_actions(flag1, &count, command, index);
		if(check_error != NULL && check_error[0] == '`')
		{
			printf("ERROR! row: %d, col: %d : %s.\n",row,index,(check_error+1));
			ERROR
		}
		IC+=count;
		free(command);
	}
	update_symbol_table(symbol_head,IC);
	return errorFlag;	
}


int second_read(FILE *file)
{	
	int index, end, flag1, flag2, IC = PROG_FIRST_ADDER, row = 0;
	char *check_error = NULL, *command = NULL;
	errorFlag = OFF;
	while((command = get_cmd(file)))
	{
		index = 0;		
		row++;
		if(!is_line_empty(command,index) || command[ignore_space(command,0)] == ';')
			continue;		
		if(command[index] == '`')
		{
			printf("ERROR! row: %d %s\n",row,(command+index+1) );
			errorFlag = ON;			
			continue;
		}
		index = ignore_space(command,index);
		end = is_symbol(command,index);
		if(end > 0)
			index = ignore_space(command,end+1);
		if(command[index] == '.')
		{
			flag1 = is_direct_order(command,index);			
			if(flag1 == 4)
			{				
				index += EAE_SIZE;
				if(is_line_empty(command,index))
				{					
					index = ignore_space(command,index);
					end = end_of_line(command,index);
					if((end-index) > SYMBOL_MAX)
					{
						printf("ERROR! row: %d, col: %d Symbol name can't be longer then %d characters.\n",row,index,SYMBOL_MAX);
						ERROR
					}
					check_error = add_to_EAE(command,index,end-1);
					if(check_error)
					{
						printf("ERROR! row: %d, col: %d %s.\n",row,index,check_error);
						ERROR
					}
					if(is_line_empty(command,end))
					{
						printf("ERROR! row: %d, col: %d extrnous text after symbol's name.\n",row,ignore_space(command,end));
						ERROR
					}
				}
				else
				{
					printf("ERROR! row: %d, col: %d missing entry symbol name.\n",row,index);
					ERROR
				}
			}
			free(command);
			continue;
		}
		flag1 = is_order(command,index);
		if(flag1 < 0)
		{			
			ERROR
		}
		check_error = add_order_to_code(flag1, command, index, &IC);
		if(check_error != NULL && check_error[0] == '`')
		{
			printf("ERROR! row: %d, col: %d %s.\n",row,index,(check_error + 1));
			ERROR
		}		
		free(command);
	}
	flag2 =  add_data_to_code(IC);
	if(flag2 == -2)
	{
		printf("ERROR: insufficient memory, cann't add data to.\n");
		errorFlag = ON;		
	}
	return errorFlag;	
}
