#include "commands.h"
#include "../helper.h"

void cd()
{
    char *present_working_directory=(char *)malloc(MAX_BUFFER_SIZE);
    getcwd(present_working_directory,MAX_BUFFER_SIZE);  // store the current working directory in present_working_directory
    if(Command[2]!=NULL && strlen(Command[2]))
    {  
        perror("Unsucessfull execution of cd-> Too many arguments");
        return;
    }
    else if(Command[1]==NULL || Command[1][0]=='~')
    {
        if(chdir(Home)!=0)
        {
            last_command_status=-1;
            perror("Unsucessfull execution of cd");
            return;
        }
    }
    else if(Command[1][0]=='-' && strlen(Command[1])==1)
    {
        if(strcmp(last_working_directory,"not_set")==0)
        {
            last_command_status=-1;
            perror("OLD PWD not set");
            return;
        }
        else
        {
            printf("%s\n",last_working_directory);
            if(chdir(last_working_directory)!=0)
            {
                last_command_status=-1;
                perror("Unsucessfull execution of cd");
                return;
            }
        }
    }
    else
    {
        if(chdir(Command[1])!=0)
        {
            last_command_status=-1;
            perror("Unsucessfull execution of cd");
            return;
        }
    }
    strcpy(last_working_directory,present_working_directory);
    free(present_working_directory);
}