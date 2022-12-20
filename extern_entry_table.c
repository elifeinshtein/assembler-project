#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "extern_entry_table.h"
#include "base_convert.h"

int add_EAE(EAE_ptr * head ,char *name , int address, short is_extern)
{	
	EAE_ptr ptr, temp = (EAE_ptr)malloc(sizeof(EAE));
	if(temp && (temp->address = decimal_to_base32(address)))
	{
		temp->is_extern = is_extern;
		temp->name = name;
		temp->next = NULL;
		if(*head)
		{
			ptr = *head;
			while(ptr->next)
				ptr = ptr->next;
			ptr->next = temp;
		}
		else
		{
			*head = temp;
		}
	}
	else
	{
		return -2;
	}
	return 1;
}

int is_EAE_empty(EAE_ptr head){ 
	return (!head)? 1:0;
}


void free_EAE(EAE_ptr *head)
{
	EAE_ptr ptr,temp;
	temp = *head;
	while(temp)
	{
		ptr = temp->next;
		free(temp->address);
		free(temp);
		temp = ptr;
	}
	*head = NULL;
}
