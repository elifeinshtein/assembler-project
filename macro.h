#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX 81

struct Macro {
    char name[MAX];
    char content[MAX];
    struct Macro* next;
};

int is_macro(char line[]);/*Action:Check if the current line contain "macro" or "endmacro" content
			   *Input:Current line from assembly file
			   *Output:if "macro" return 1, if "endmacro" return 2, otherwise return 0*/

void add_name(struct Macro *temp, char line[]);/*Action:Add macro name to macro table
			   			*Input:pointer to macro table, macro name.*/

void add_content(struct Macro *temp, FILE *fp);/*Action:Add macro content to macro table
			   			*Input:pointer to macro table,pointer to assembly file.*/

int macro_check(int i, char *argv[],struct Macro *head);/*Action:Check if current file contain any macros
							  *Input:file name (argv[i], pointer to macro table.
			 				  *Output:If can't open file return 0, otherwise return 1*/

int add_macro(char line[], FILE *fpw,struct Macro *tail); /*Action:Add macro content to relevant place in newly created .am file
							  *Input:string with macro content, output file .am , macro table.
			 				  *Output:If macro added succssusfuly return 0, otherwise return 1*/

void create_file_am(int i, char *argv[],struct Macro *tail);/*Action:Create new file with .am extension with macro included.
							     *Input:file name(argv[i], macro table.*/
