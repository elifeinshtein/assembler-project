#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "read_file.h"

char buffer[CMD_MAX];
static int index = -1;

char *get_cmd(FILE *fp1){
	char c;
	char *arr;
	int i;
	if((c = get_char(fp1))!= EOF)
		set_char(c);
	else
		return NULL;
	arr = (char *)malloc(CMD_MAX*sizeof(char));
	if(arr)
	{
		i = 0;
		while((c = get_char(fp1)) != EOF && c != '\n' && i<(CMD_MAX-1))
		{
			arr[i] = c;
			i++;
		}
		arr[i] = '\0';
		if(i <= (CMD_MAX-1) && c!= EOF && c != '\n')
		{
			set_char(c);
			while((c = get_char(fp1)) != EOF && c != '\n')
				;
			if(c == EOF)			
				set_char(c);			
			return "ERROR:Command length overflow, max characters 81.\0";
		}
		else
			return arr;

	}
	else
	{
		return "ERROR: Dynamic allocation failed! canno't read more lines.\0";
	}	
}

char get_char(FILE *fp1){
	if(index>-1)
	{
		return buffer[index--];
	}
	return getc(fp1);
}

void set_char(char c){
	buffer[++index] = c;
}

int ignore_space(char *command, int current_index){
	char c;
	while((c = command[(current_index)]) != '\0' && isspace(c))
		current_index++;
	return current_index;
}

int ignore_break(char *command, int current_index){
	current_index = ignore_space(command,current_index);
	if(command[current_index] == ',')
	{
		current_index++;
		return current_index;
	}
	else
	{
		return -1;
	}
}

int is_line_empty(char *command, int current_index){
	current_index = ignore_space(command,current_index);
	return (command[current_index] != '\0')? 1 : 0;	
}

int end_of_line(char *command, int current_index){
	char c;
	while((c = command[(current_index)]) != '\0' && !isspace(c) && (c != ','))
		current_index++;
	return current_index;
}
