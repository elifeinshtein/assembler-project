#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro.h"
#include "file_check.h"
#include "assembler_functions.h"
#include "base_convert.h"


int main(int argc,char  **argv){	
	FILE *fp;
	int flag,i;
	char *file_name;
	
	if(argc == 1){
		printf("ERROR: missing file name!\n");
		exit(0);
	}
	
	for(i=1; i<argc ;i++){	
		struct  Macro* tail= NULL;
   		struct  Macro* head = NULL;
		head = (struct Macro*)malloc(sizeof(struct Macro));
   		tail = head;

		flag = macro_check(i,argv,head);
		if(flag)
			create_file_am(i,argv,tail);
		else
			continue;

		file_name = NULL;
		file_name = add_extensions(argv[i],".am\0");

		if((fp = fopen(file_name, "r")) == NULL)
			printf("Cannot open file %s, continue with next file!\n\n", file_name);
		else{
			printf("------- Open %s -------\n",file_name);
			if (fp){
				flag = first_read(fp);
				if(flag == ON){					
					fclose(fp);
					free_assembler();
					printf("------ %s compile failed! ------\n",file_name);
					free(file_name);
					continue;
				}
				rewind(fp);
				flag = second_read(fp);
				if(flag == ON){					
					fclose(fp);
					free_assembler();	
					printf("------ %s compile failed! ------ \n",file_name);
					free(file_name);
					continue;
				}
				if(create_files(argv[i]) != 1){
					printf("Error: %s files creation failed!\n",file_name );
				}
				else{
					printf("------ %s compile completed ------\n",file_name);
				}
			   	fclose(fp);
			}
			free(file_name);
			free_assembler();			
		}
	}
	return 0;
}	
