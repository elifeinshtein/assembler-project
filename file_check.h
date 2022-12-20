#define ERROR \
errorFlag = ON;\
free(command);\
continue;

enum Status {OFF,ON};

int first_read(FILE *);/*Action:First pass on given file,analayze text and create each table of content and code table, fill code table with relevant information.
			*Input:Pointer to assembly file
			*Output:return 0 if no error found, 1 otherwise.*/

int second_read(FILE *);/*Action: Second pass on given file,complete and fill the missing information in the code table.
			*Input:Pointer to assembly file
			*Output:return 0 if no error found, 1 otherwise.*/



