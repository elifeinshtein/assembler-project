#include <stdio.h>
#include <stdlib.h>
#include "base_convert.h"

typedef struct code * CP;
typedef struct code{
    int address;             /*decimal value*/
    char *bin;          /*string represent binary value*/
    CP next;	
} code;


int is_code_empty(CP head);                              /*Action:Check if code table is empty
								*Input: a pointer code table head
                                                                 *Output: return 1 if empty, -1 otherwise*/

int create_new_code(CP * head ,int address ,char *bin);        /*Action:Add new content to code table.
								*Input:Table's head, new code address, binary value of code
                                                                *Output: 1 if added succsusfully, -2 if memory allocation failed, -1 otherwise*/

CP get_next_code(CP current);                       /*Action:return pointer to next node
								 *Input: pointer to current node
                                                                 *Output:pointer to the next node*/

char *get_code(CP * head);                             /*Action:Convert binary code from table to base32
								 *Input:pointer to code table's node
                                                                 *Output: convered code table node content to base 32*/

void free_code(CP * head);          /*free code table*/
