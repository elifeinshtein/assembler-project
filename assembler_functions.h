/*General assembler functions*/

char *count_actions(int instruction_ind, int *length, char *cmd, int current_index); /*Input: instruction number, pointer to counter, command,index to start from.
									      * instruction number (0-15)
									      * Output: set length to number of needed words in code file, and NULL if no error found*/


char *add_to_EAE(char *cmd, int current_index, int end); /*Input: command,length of symbol name(starting index + end index). 
						    *Action:check if symbol exist in list, if exist add to EAE table if needed. 
						    *Output: add relevant content to EAE table.*/


int create_files(const char *file_name);/*Input: file name.
					*Action: create and write output files.
					*Output: positive number if file creation succsus, otherwise return -2.*/

char* add_extensions(const char *str1,const char *str2); /*input: str1 = file name, str2 = file extension to add.  
					                 *Output: file name with given extension */

int add_data_to_code(int IC); /*Input: IC represent the last address in code table.
			      *Action: add data to program code table.
			      *Output: positive number if data added succsusfuly,otherwise return -2. */

char *add_order_to_code(int instruction_ind, char *cmd, int current_index,int *IC); /*Input: command,instruction number. 
									 *Action: add the action method, to the code table.
									 *Output: add relevent content to code table. */

void free_assembler();/*Contain call to free functions to all tables*/							 
