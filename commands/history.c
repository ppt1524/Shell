#include "commands.h"
#include "../helper.h"

void display_history(int cnt) // to print the history
{
    if(cnt<0)
    {
        last_command_status=-1;
        perror("Invalid commands");
        return;
    }
    int start;
    if(history_indx>=cnt)
    start=history_indx-cnt;
    else
    start=0;
    for(int i=start;i<history_indx;i++)
    printf("%s",History[i]);
}

void commit_history() // to save the commands to history when the user closes 
{
    FILE* file_ptr = fopen(path_for_history,"r+");
    if(file_ptr==NULL)
    {
        perror("Error in opening history.txt");
        return;
    }
    for(int i=0;i<history_indx;i++)
    fputs(History[i],file_ptr);
    fclose(file_ptr);
}

void init_history()
{
    FILE *file_ptr = fopen(path_for_history,"r+");
    if(file_ptr!=NULL)
    {
        history_indx=0;
        while(history_indx<20 && fgets(History[history_indx],MAX_BUFFER_SIZE,file_ptr)!=NULL)
        history_indx++;
    }
    else 
    {
        perror("Error in opening history.txt");return;
    }
    fclose(file_ptr);
}
void modify_history(char* command)
{
    if(history_indx!=SIZE_OF_HISTORY)
    {
        strcpy(History[history_indx],command);
        history_indx++;
    }
    else
    {
        for(int i=0;i<SIZE_OF_HISTORY-1;i++)
        strcpy(History[i],History[i+1]);
        strcpy(History[history_indx-1],command);
    }
}