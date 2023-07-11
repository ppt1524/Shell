#include "commands.h"
#include "../helper.h"

void other_system_commands(int Is_Background)
{
    int pid=fork();
    if(pid>0)
    {
        if(Is_Background==1)
        {
            Insert_Process(Command,pid);
            printf("[%d] %d\n",number_of_background_jobs,pid);
        }
        else
        {

            signal(SIGTTIN,SIG_IGN);
            signal(SIGTTOU,SIG_IGN);
            tcsetpgrp(STDIN_FILENO,pid); 

            int current_status;
            begin=endtime=0;
            begin=time(NULL);
            waitpid(pid, &current_status, WUNTRACED);
            endtime=time(NULL);
            if(current_status>=255 && current_status<2*255)
            last_command_status=-1;

            tcsetpgrp(STDIN_FILENO,getpgrp()); 
            signal(SIGTTOU,SIG_DFL); 
            signal(SIGTTIN,SIG_DFL);
            
            if(WIFSTOPPED(current_status))
            {   
                printf("Process with pid: %d is stopped\n",pid);
                last_command_status=-1;
                Insert_Process(Command,pid);
            }
        }
    }
    else if(pid==0)
    {
        signal(SIGINT,SIG_DFL);
        signal(SIGTSTP,SIG_DFL);
        setpgid(0,0);
        if(execvp(Command[0],Command)>=0)
        {
            printf("Process done successfully\n");
            exit(EXIT_SUCCESS);
        }
        else
        {
            perror("Invalid Command");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        last_command_status=-1;
        perror("Unable to Fork correctly");
        exit(EXIT_FAILURE);
    }
}