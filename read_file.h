#define CMD_MAX 81

char *get_cmd(FILE *);/*Action: Get command from given assembly file.
		       *Input: pointer to assembly file.
		       *Output:Whole line from file, put '\0' at the end.*/

char get_char(FILE *);/*Action:Get one char from given assembly file.
		       *Input:pointer to assembly file. 
		       *Output:character from assembly file*/

void set_char(char c);/*Action:Set given character in program buffer.
		       *input:relevant character to program buffer*/

int ignore_space(char *command, int current_index);/*Action:Ignore all white character from given command
						    *Input:Command from assembly, and index to work from
						    *Output:Relevant index in given command after ignoring white chars*/

int ignore_break(char *command, int current_index);/*Action:Ignore assembly break ',' from given command
						    *Input:Command from assembly, and index to work from
						    *Output:Relevant index in given command after ignoring break char, -1 if couldn't find break char.*/

int is_line_empty(char *command, int current_index);/*Action:Check if given command is empty.
						    *Input:Command from assembly, and index to work from
						    *Output:1 if empty, 0 otherwise.*/

int end_of_line(char *command, int current_index);/*Action:Return the index of the last character in given command.
						    *Input:Command from assembly, and index to work from
						    *Output:index of the last char*/
