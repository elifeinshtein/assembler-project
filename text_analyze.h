#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "read_file.h"


int is_symbol(char *command, int current_index);/*Action:Check if given command contain symbol(label)
                                       		 *Input:Command line, index to work from 
                                                 *Output:If symbol, return his length, otherwise return -1 */

int is_direct_order(char *command, int current_index);/*Action:Check if given command contain direct order
                                       		 	*Input:Command line, index to work from 
                                                 	*Output:If direct order, return his number, otherwise return -1 */

int is_order(char *command, int current_index);/*Action:Check if given command contain an order
                                       		 	*Input:Command line, index to work from 
                                                 	*Output:If an order, return his number, otherwise return -1 */

int is_integer(char *command, int current_index);/*Action:Check if given command contain an integer value
                                       		 	*Input:Command line, index to work from 
                                                 	*Output:If an integer, return the last digit index + 1 (char after the number), otherwise return -1 */



