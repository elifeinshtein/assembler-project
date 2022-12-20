#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include "text_analyze.h"
#include "read_file.h"
#include "base_convert.h"


typedef struct data_node * DP;
typedef struct data_node{/*decimal values table*/
    int data; 
    int DC; 
    DP next;

}data_node;





int create_new_data(DP *head , int number, int * DC);  /*Action:Create new data node, if table is empty add to head of table, if not empty add as next node, always updating DC.
							*Input: Pointer to data table, data number, pointer to current DC
                                                         *Output: New node to data table.*/

int add_new_string(char *command, int current_index, int *DC, DP *head);     /* Action:Add string content to table
									*Input:command contain string, current index to start from, pointer to DC, pointer to data table.
                                                                         *Output: If string added to table, return the relevant index to keep analayze text, -2 if memory allocation failed, -1 otherwise. */

int add_new_struct(char *command, int current_index, int *DC, DP *head);     /*Action:Add struct content to table
									 *Input:command contain struct content, index to work from, pointer to DC, pointer to table.
                                                                         *Output:if struct added to table, return the relevant index to keep analyze text, -2 if memory allocation failed, -1 otherwise. */

int add_new_data(char *command, int current_index, int *DC, DP *head);        /*Action:Add data content to table
									 *Input:command contain data content, index to work from, pointer to DC, pointer to table.
                                                                         *Output:If data added to table, return the relevant index to keep analayze text, -2 if memory allocation failed, -1 otherwise.*/

void free_data(DP * head);                                  /*free data table*/


