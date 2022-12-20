#include "code_table.h"

int is_code_empty(CP head){
    if(head==NULL)
        return 1;
    return -1;
}

int create_new_code(CP * head ,int address ,char *bin) {
    CP new_data = (CP) malloc(sizeof(code));
    CP temp;
    if (new_data == NULL) {
        return -2;
    }
    new_data->address = address;
    new_data->bin = bin;    
    new_data->next = NULL;
    if (is_code_empty(*head)>0) {

        *head = new_data;
        return 1;
    }

    temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = new_data;
    return 1;


}

CP get_next_code(CP current){
    return current->next;
}

char *get_code(CP * head){
    char *converted=(char *)malloc(6*sizeof(char));
    CP temp=*head;
    char *tmp_str;
    if(!converted)
        return NULL;
    (*head)=(*head)->next;
    memcpy(converted,decimal_to_base32((temp)->address),2);
    converted[2]='\t';
    tmp_str = converted+3;
    memcpy(tmp_str,binary_to_base32((temp)->bin),2);
    converted[5]='\0';
    return converted;
}

void free_code(CP * head){
    CP pt,temp;
	temp = *head;
	while(temp)
	{
		pt = temp->next;		
        free(temp->bin);
		free(temp);
		temp = pt;
	}
	*head = NULL;
}
