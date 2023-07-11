#include "commands.h"
#include "../helper.h"

void pwd()
{
    char *current_directory;
    current_directory=(char *)malloc(MAX_BUFFER_SIZE);
    if(getcwd(current_directory,MAX_BUFFER_SIZE)!=NULL)
    {
        printf("%s\n",current_directory);
    }   
    else
    {
        last_command_status=-1;
        perror("Error in displaying the directory");
        return;
    }
}