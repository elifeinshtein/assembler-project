#include "data_table.h"

#define STR_LENGTH 7
#define STRUCT_SIZE 7
#define DATA_SIZE 5
#define INT_MAX 512
#define INT_MIN -512

int add_num(char * command, int pos, int last, int *DC, DP * head);/*adds the number represented as string in current command*/

int add_not_order_string(char * command,int current_index ,DP * head,int * DC); /*Add string content "example" to table*/

int add_num(char * command, int pos, int last, int *DC ,DP * head){
    int int_val = string_to_int(command,pos,last), add_to_table;        
    if(int_val > INT_MAX || int_val < INT_MIN)
        return -1;
    if ((add_to_table = create_new_data(head, int_val, DC) < 0))
        return add_to_table;
    return 1;
}

int create_new_data(DP *head , int number, int *DC){
    DP new_data = (DP)malloc(sizeof(data_node));
    DP temp;
    if(new_data == NULL){
        return -2;
    }
    new_data->data=number;
    new_data->DC=*DC;
    new_data->next=NULL;
    *DC+=1;
    if(*head==NULL){
        *head=new_data;
        return 1;
    }

    temp=*head;
    while(temp->next!=NULL){
        temp=temp->next;
    }
    temp->next=new_data;
    return 1;

}
int add_not_order_string(char * command ,int current_index ,DP * head, int * DC){
    int pos = current_index,add_to_table;
    if(command[pos]!='\"')
        return -1;
    pos++;
    while(command[pos]!='\"' && is_line_empty(command,pos)) {
        if ((add_to_table = create_new_data(head, command[pos], DC)) < 0)
            return add_to_table;
        pos++;
    }
    if(command[pos] != '\"')
        return -1;
    if((add_to_table = create_new_data(head,'\0',DC))<0)
        return add_to_table;
    return pos+1;
}
int add_new_string(char *command, int current_index, int *DC, DP *head){
    int pos = current_index + STR_LENGTH;    
    pos = ignore_space(command,pos);
    return add_not_order_string(command,pos,head,DC);
}

int add_new_struct(char *command, int current_index, int *DC, DP *head) {
    int pos = current_index + STRUCT_SIZE,last,num,endStruct;  
    pos = ignore_space(command, pos);
    if ((last = is_integer(command, pos)) < 0) {
        return -1;
    }    
    num = add_num(command,pos,last-1,DC,head);
    if(num<0)
        return num;
    pos = last;
    pos = ignore_space(command,pos);
    if(command[pos]!=',')
        return -1;
    pos = ignore_space(command,pos+1);
    if(command[pos]!='\"')
        return -1;
    if((endStruct = add_not_order_string(command,pos,head,DC))<0)
        return endStruct;    
    return endStruct;
}

int add_new_data(char *command, int current_index, int *DC, DP *head){
    int pos = current_index + DATA_SIZE, last, number;   
    pos = ignore_space(command,pos);
    if(!is_line_empty(command,pos))
        return -1;
    if((last = is_integer(command,pos))<0)
        return last;    

    number = add_num(command,pos,last-1,DC,head);
    if(number<0)
        return number;
    pos=last;    
    while(is_line_empty(command,pos)) {
        pos = ignore_space(command,pos);
        if(command[pos]!=',')
            return pos;
        pos = ignore_space(command,pos+1);
        if((last = is_integer(command,pos))<0)
            return last;        
        number = add_num(command,pos,last-1,DC,head);
        if(number<0)
            return number;
        pos=last;        
    }
    return pos;
}

void free_data(DP * head){    
    DP pt,temp;
    temp = *head;
    while(temp)
    {
        pt = temp->next;        
        free(temp);
        temp = pt;
    }
    *head = NULL;
}


