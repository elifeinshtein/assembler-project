typedef struct symbol * symbol_ptr;
typedef struct symbol{	
	char *name; 
	int address;/*in decimal*/
	short type;/*0,1,2 or 3*/ 
	symbol_ptr next; }
symbol;

int create_new_symbol(symbol_ptr* head, char *command, int current_index, int length, int address, short type);/*Action:Add new symbol node to table
		      												 *Input:Symbol table with all information to add
		   											        *Output:1 if succsuss, -2 otherwise*/

int check_symbol(symbol_ptr head, char *name);/*Action:Check if symbol is exist in table
		      			       *Input:Table and name to search for
		   			       *Output:1 if succsuss, 0 otherwise*/

symbol_ptr get_symbol(symbol_ptr head, char *name);/*Action:Get relevant symbol node from table
		      			       *Input:Symbol table and symbol name
		   			       *Output:pointer to node if succsuss, NULL otherwise*/

int is_symbol_empty(symbol_ptr head);/*Action:Check if symbol table is empty
		      			       *Input:Symbol table
		   			       *Output:1 if empty, 0 otherwise*/

symbol_ptr get_next_symbol(symbol_ptr* head);/*Action:Get next node of given node.
		      			       *Input:Symbol table
		   			       *Output:pointer to node if succsuss, NULL otherwise*/

int add_symbol(symbol_ptr* head, char *command, int current_index);/*Action:Extract all relevant inputs to create_new_symbol function
		      			      		  	    *Input:Symbol table pointer, command line, index to work from
		   			     			    *Output:positive number if succsuss, -2 if memory allocation failed,-3 if there's syntax error */

void update_symbol_table(symbol_ptr head, int IC);/*Action:Update symbol table with missing information in file second check.
						   *Input:Symbol table, IC*/

void free_symbol(symbol_ptr* head);/*Free symbol table.*/
