#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "read_file.h"
#include "symbol_table.h"
#include "text_analyze.h"

int create_new_symbol(symbol_ptr* head, char *command, int current_index, int length, int address, short type)
{
	symbol_ptr pt;	
	if(*head)
	{		
		pt = *head;		
		while(pt->next)
		{			
			pt = pt->next;
		}		
		pt->next = (symbol_ptr)malloc(sizeof(symbol));		
		pt = pt->next;
	}
	else
	{		
		*head = (symbol_ptr)malloc(sizeof(symbol));
		pt = *head;
	}	
	if(pt && (pt->name = (char *)malloc((length+1)*sizeof(char))))
	{			
		pt->next = NULL;
		pt->name = strncpy(pt->name, command + current_index,length);
		pt->name[length] = '\0';
		pt->address = address;
		pt->type = type;
	}
	else
	{		
		return -2;
	}
	return 1;
}

int check_symbol(symbol_ptr head, char *name)
{	
	while(head)
	{
		if(strcmp(head->name,name) == 0)
			return 1;
		head = head->next;		
	}
	return 0;
}

symbol_ptr get_symbol(symbol_ptr head, char *name)
{
	while(head)
	{
		if(strcmp(head->name,name) == 0)
			return head;
		head = head->next;		
	}
	return NULL;
}

int is_symbol_empty(symbol_ptr head)
{
	return (head)? 0 : 1;
}

symbol_ptr get_next_symbol(symbol_ptr* head)
{
	symbol_ptr p;
	if(!is_symbol_empty(*head))
	{
		p = *head;
		*head = (*head)->next;
		return p;
	}
	return NULL;
}

int add_symbol(symbol_ptr* head, char *command, int current_index)
{
	int start,end,count=0;	
	while(is_line_empty(command, current_index) && current_index >= 0)
	{
		start = ignore_space(command, current_index);
		end = end_of_line(command, start);
		if(end>-1)
		{						
			if(create_new_symbol(head, command, start, end - start , 0, 2) <0 )
				return -2;
			count++;
			current_index = ignore_break(command,end);				
			if((current_index == -1 && is_line_empty(command, end)) || (current_index != -1 && !is_line_empty(command, current_index)))
				return -3;			
		}
		else
		{
			return -1;
		}
	}
	return count;
}

void update_symbol_table(symbol_ptr head, int IC)
{
	while(head)
	{
		if(head->type <= 1)
			head->address += IC;
		head = head->next;
	}
}

void free_symbol(symbol_ptr* head)
{
	symbol_ptr pt,next;
	if(!is_symbol_empty(*head))
	{
		pt = *head;
		while(pt)
		{
			next = pt->next;
			free(pt->name);
			free(pt);
			pt = next;
		}
		*head = NULL;
	}
}
