#include "text_analyze.h"

#define SYMBOL_MAX 30
#define NUM_OF_ACTIONS 16
#define NUM_OF_DIRECT 5
#define DIRECT_TABLE 7 
#define DECIMAL_MAX_VAL '9'
#define DECIMAL_MIN_VAL '0'
#define END_OF_LINE '\0'
#define OK 0
#define CHAR_TAB '\t'
#define CHAR_PLUSE '+'
#define CHAR_DOT '.'
#define CHAR_SPACE ' '
#define CHAR_COLON ':'
#define CHAR_HYPHEN '-'




char *actions[NUM_OF_ACTIONS] = {"mov\0","cmp\0","add\0","sub\0","not\0","clr\0","lea\0","inc\0","dec\0","jmp\0","bne\0","get\0","prn\0","jsr\0","rts\0","hlt\0"};
char *directives[NUM_OF_DIRECT] = {"data\0","string\0","struct\0","entry\0","extern\0"};

int is_symbol(char *command, int current_index){
    int pos = 1;
    if (isalpha(command[current_index])){
        while(isalpha(command[current_index + pos]) || isdigit(command[current_index + pos])){
            if(pos > SYMBOL_MAX){
                return -1;
            }
            pos++;
        }
        if (command[current_index + pos] == CHAR_COLON)
            return current_index + pos;
    }
    return -1;
}
int is_direct_order(char *command, int current_index){
    int pos = 1,i;
    char directive[DIRECT_TABLE];   
    if(command[current_index] == CHAR_DOT){        
        while((command[current_index + pos] != CHAR_SPACE) && (command[current_index + pos] != CHAR_TAB) && (command[current_index + pos] != END_OF_LINE)){
            if(pos >= DIRECT_TABLE)
                return -1;
            directive[pos - 1] = command[current_index + pos];
            pos++;
        }        
        directive[pos - 1] = END_OF_LINE;
        for (i = 0; i < NUM_OF_DIRECT ;i++){
            if(strcmp(directive,directives[i]) == OK){
                return i+1;
            }
        }
        return -1;

    }    
    return -1;
}
int is_order(char *command, int current_index){
    int pos = 0,i;
    char order[NUM_OF_DIRECT]; 
    while (pos < (NUM_OF_DIRECT - 1) && command[current_index + pos] != END_OF_LINE && !isspace(command[current_index + pos])){
        order[pos]=command[current_index + pos];
        pos++;
    }    
    order[pos] = END_OF_LINE;
    for (i = 0; i < NUM_OF_ACTIONS ;i++){
        if(strcmp(order,actions[i]) == OK){
            return i;
        }
    }    
    return -1;
}

int is_integer(char *command, int current_index)
{
    int start = current_index ,end = 0;
    if(start + end < CMD_MAX && (command[start+end] == CHAR_HYPHEN || command[start+end] == CHAR_PLUSE))
        start++;
    while (command[start + end] != END_OF_LINE && command[start + end] >= DECIMAL_MIN_VAL && command[start + end] <= DECIMAL_MAX_VAL){        
        end++;
    }
    return (end == 0)? -1: start+end;
}
