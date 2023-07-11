#include "helper.h"
#include "commands/commands.h"

int check_is_file(char *file)
{
    struct stat info;
    if(stat(file,&info)) return 0;
    if(S_ISDIR(info.st_mode)) return 0;
    return 1;
}

int redirect_input(int curr_indx)
{
    int temp=0,is_redirected=0;
    while(Multiple_Commands[curr_indx][temp]!='\0')
    {
        if(Multiple_Commands[curr_indx][temp]=='<')
        is_redirected=1;
        temp++;
    }
    if(!is_redirected)return 0;
    char * copy = (char *)malloc(MAX_BUFFER_SIZE);
    strcpy(copy,Multiple_Commands[curr_indx]);

    char * input_file_name=(char *)malloc(MAX_BUFFER_SIZE);

    char * middle_str = (char *)malloc(MAX_BUFFER_SIZE);
    middle_str=strtok(copy,"<");
    middle_str=strtok(NULL,"<");
    input_file_name=strtok(middle_str," \n\r\t");
    if(input_file_name != NULL && check_is_file(input_file_name))
    {
        int file_ptr = open(input_file_name,O_RDONLY);
        if(file_ptr>=0)
        {
            dup2(file_ptr,STDIN_FILENO);
            close(file_ptr);
            return 1;
        }
        else
        {
            last_command_status=-1;
            perror("Can't open given input file\n");
            return -1;
        }
    }
    else
    {
        last_command_status=-1;
        fprintf(stderr,"Input file does not exist");
        return -1;
    }
}
int redirect_output(int curr_indx)
{
    int temp=0,is_redirected=0;
    while(Multiple_Commands[curr_indx][temp]!='\0')
    {
        if(Multiple_Commands[curr_indx][temp]=='>')
        is_redirected=1;
        temp++;
    }
    if(!is_redirected)return 0;
    
    temp = 0;
    int is_double_redirected=1;
    if(strstr(Multiple_Commands[curr_indx],">>")==NULL)is_double_redirected=0;


    char * output_file_name=(char *)malloc(MAX_BUFFER_SIZE);
    char * copy = (char *)malloc(MAX_BUFFER_SIZE);
    strcpy(copy,Multiple_Commands[curr_indx]);
    char * middle_str = (char *)malloc(MAX_BUFFER_SIZE);
    if(is_double_redirected)
    {
        middle_str=strtok(copy,">>");
        middle_str=strtok(NULL,">>");
    }
    else
    {
        middle_str=strtok(copy,">");
        middle_str=strtok(NULL,">");
    }

    output_file_name=strtok(middle_str," \n\r\t");
    if(output_file_name != NULL)
    {
        int file_ptr=1;
        if(is_double_redirected)
        file_ptr = open(output_file_name,O_WRONLY | O_CREAT | O_APPEND, 0644);
        else
        file_ptr = open(output_file_name,O_WRONLY | O_CREAT | O_TRUNC, 0644);
        
        if(file_ptr>=0)
        {
            dup2(file_ptr,STDOUT_FILENO);
            close(file_ptr);
            return 1;
        }
        else
        {
            last_command_status=-1;
            perror("Can't open given output file\n");
            return -1;
        }
    }
    else
    {
        fprintf(stderr,"Output file not given: \n");
        return -1;
    }
}
void modify_command(int curr_indx)
{
    int ptr=0,should_continue=0,cur_ptr=0;
    char ** temp_store;
    temp_store = (char **)malloc(sizeof(char *) * MAX_BUFFER_SIZE);
    while(Command[ptr]!=NULL)
    {
        if(should_continue)
        should_continue = ! should_continue ;
        else if(Command[ptr][0]=='>')
        {
            if(strlen(Command[ptr])==1)
            should_continue=1;
            else
            {
                if(Command[ptr][1]=='>')should_continue=1;
                else continue;
            }
        }
        else if(Command[ptr][0] == '<')
        {
            if(strlen(Command[ptr])==1)should_continue=1;
        }
        else
        {
            temp_store[cur_ptr]=(char *)malloc(MAX_BUFFER_SIZE);
            strcpy(temp_store[cur_ptr++],Command[ptr]);
        }
        ptr++;
    }

    for(int i=0;i<MAX_BUFFER_SIZE;i++)
    {
        free(Command[i]);
        Command[i]=(char *)malloc(MAX_BUFFER_SIZE);
    }

    strcpy(Multiple_Commands[curr_indx],temp_store[0]);
    number_of_arguments = cur_ptr;

    for(int i=1;i<number_of_arguments;i++)
    {
        strcat(Multiple_Commands[curr_indx]," ");
        strcat(Multiple_Commands[curr_indx],temp_store[i]);
    }
    for(int i=0;i<number_of_arguments;i++)
    strcpy(Command[i],temp_store[i]);

    Command[number_of_arguments]=NULL;
}
void Redirect(int curr_indx)
{
    int input_file_descriptor=dup(STDIN_FILENO);
    int output_file_descriptor=dup(STDOUT_FILENO);

    if(redirect_input(curr_indx) == -1)return;
    if(redirect_output(curr_indx) == -1)return;

    modify_command(curr_indx);
    execute_command(curr_indx);

    dup2(input_file_descriptor,STDIN_FILENO);
    dup2(output_file_descriptor,STDOUT_FILENO);
    close(input_file_descriptor);
    close(output_file_descriptor);
}