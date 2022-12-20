typedef struct EAE * EAE_ptr; /*Pointer to node*/
typedef struct EAE
{
	short is_extern;/*0 or 1*/
	char *name;/*name of symbol*/
	char *address;/*address of symbol in base32*/
	EAE_ptr next;
}EAE;

int add_EAE(EAE_ptr *head ,char *name , int adress, short is_extern); /*action:Add new node to EAE table.
								       *Input:pointer to EAE table, relevant content to be added.
								       *Output:return 1 if added succssusfuly, negative number otherwise*/

int is_EAE_empty(EAE_ptr head);/*Action:Check if table is empty.
				*Input: pointer to head of EAE table.
				*Output:return 1 if empty, negative number otherwise*/

void free_EAE(EAE_ptr *head);/*free EAE table*/

