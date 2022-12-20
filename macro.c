#include "macro.h"
int is_macro(char line[]) 
{
    int index = 0 , macro_index = 0 ;
    char macro [MAX];
    memset(macro , '\0' , MAX);
    while(isspace(line[index]))
        index ++;
    while (!isspace(line[index]) && line[index] != '\n')
    {
        macro[macro_index] = line[index];
        macro_index++;
        index++;
    }
    if (!strcmp(macro, "macro"))
        return 1;
    if (!strcmp(macro, "endmacro"))
        return 2;
    return 0;
}
void add_name(struct Macro *temp, char line[]) 
{
    int index = 0 , name_index = 0 ;
    char name[MAX];
    memset(name , '\0' , MAX);
    while(isspace(line[index]))
        index ++;
    while (!isspace(line[index]) && line[index] != '\n')
        index++;
    while(isspace(line[index]))
        index ++;
    while (!isspace(line[index]) && line[index] != '\n')
    {
        name[name_index] = line[index];
        name_index++;
        index++;
    }
    strcpy(temp->name,name);
}
void add_content(struct Macro *temp, FILE *fp)
{
    char line [MAX];
    char content [MAX];
    memset(line , '\0' , MAX);
    memset(content , '\0' , MAX);
    fgets(line, MAX, fp);
    while(is_macro(line) != 2)
    {
        strncat(content, line , MAX);
        fgets(line, MAX, fp);
    }
    strcpy(temp->content,content);
}
int macro_check(int i, char *argv[],struct Macro *head)
{
    char line [MAX];
    char file_name[MAX];
    FILE *fp;
    memset(line , '\0' , MAX);
    strcpy(file_name,argv[i]);
    strncat(file_name, ".as", 3);
    fp = fopen(file_name,"r");
    if(fp == NULL)
    {
        printf("Cannot find file %s, continue to next file if exist!\n\n" , argv[i]);
        return 0;
    }
    while(fgets(line, MAX, fp))
    {
        struct  Macro* temp = NULL;
        temp = (struct Macro*)malloc(sizeof(struct Macro));
        if(is_macro(line) == 1)
        {
            add_name(temp , line);
            add_content(temp , fp);
            strcpy(head->name,temp->name);
            strcpy(head->content,temp->content);
            free(temp);
            temp = NULL;
            head->next = temp;

        }
    }
    return 1;
}
int add_macro(char line[], FILE *fpw, struct Macro *tail)
{
    int index = 0, macro_index = 0;
    char name [MAX];
    struct  Macro *temp = NULL;
    temp = tail;
    memset(name, '\0' , MAX);
    while(isspace(line[index]))
        index ++;
    while (!isspace(line[index]) && line[index] != '\n')
    {
        name[macro_index] = line[index];
        macro_index++;
        index++;
    }
    while (temp != NULL)
    {
        if (!strcmp(temp->name , name))
        {
            fprintf(fpw, "%s", temp->content);
            return 0;
        }
        temp = temp ->next;
    }
    return 1;
}
void create_file_am(int i, char *argv[],struct Macro *tail)
{
    FILE *fpw;
    FILE *fpr;
    int flag = 1;
    char line[MAX];
    char file_name_as[MAX];
    char file_name_am[MAX];

    memset(line , '\0' , MAX);
    strcpy(file_name_as,argv[i]);
    strcpy(file_name_am,argv[i]);
    strncat(file_name_as, ".as", 3);
    strncat(file_name_am, ".am", 3);
    fpr = fopen(file_name_as,"r");
    fpw = fopen(file_name_am,  "w");

    if(fpr == NULL){
        printf("Cannot find file %s, continue with next file!\n\n" , argv[i]);
	fclose(fpr);
	return;
	}

    while(fgets(line, MAX, fpr))
    {
        if(flag)
        {
            if(add_macro(line, fpw, tail))
            {
                if(is_macro(line) == 0)
                {
                    fprintf(fpw,"%s",line);
                }
                else
                {
                    flag = 0;
                }
            }
        }
        else
        {
            if(is_macro(line) == 2)
                flag = 1;
        }
    }
    free(tail);
    fclose(fpw);
    fclose(fpr);
}


