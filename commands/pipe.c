#include "helper.h"
#include "commands/commands.h"

void Pipe(int curr_indx)
{
    int input_file_descriptor=dup(STDIN_FILENO);
    int output_file_descriptor=dup(STDOUT_FILENO);

    int number_of_pipes=compile_piped_commands(Multiple_Commands[curr_indx]);
    int *store; 
    store=(int *)malloc(sizeof(int)*2);
    int file_ptr=STDIN_FILENO;
    for(int i=0;i<number_of_pipes;i++)
    {
        if(pipe(store)>=0)
        {
            pid_t child_process = fork();
            if(child_process>0)
            {
                wait(NULL);
                close(store[1]);
                file_ptr=store[0];    
            }
            else if(child_process==0)
            {
                dup2(file_ptr,STDIN_FILENO);
                if(number_of_pipes != i+1)
                dup2(store[1],STDOUT_FILENO);
                close(store[0]);

                strcpy(Multiple_Commands[curr_indx],for_piping_command[i]);
                compile_command(for_piping_command[i]);

                execute_command(curr_indx);
                exit(EXIT_SUCCESS);
            }
            else
            {
                last_command_status=-1;
                perror("Error in forking");
                return;
            }
        }
        else
        {   
            last_command_status = -1;
            perror("Error in Piping");
            return;
        }
    }
    dup2(input_file_descriptor,STDIN_FILENO);
    dup2(output_file_descriptor,STDOUT_FILENO);
    return;
}