#include "commands.h"
#include "../helper.h"

void fg()
{
    if(number_of_arguments==2)
    {
        int val = atoi(Command[1]);
        if(val>0 && val<=number_of_background_jobs)
        {
            int pid = Store_BackgroundJobs[val-1].pid;
            char *process_name;
            process_name = (char *)malloc(MAX_BUFFER_SIZE);
            strcpy(process_name,Store_BackgroundJobs[val-1].Job_name);
            int i=0;
            while(i<number_of_background_jobs)
            {
                if(Store_BackgroundJobs[i].pid==pid)
                {
                    int j=i+1;
                    while(j<number_of_background_jobs)
                    {
                        strcpy(Store_BackgroundJobs[j-1].Job_name,Store_BackgroundJobs[j].Job_name);
                        Store_BackgroundJobs[j-1].pid = Store_BackgroundJobs[j].pid;
                        j++;
                    }
                    Store_BackgroundJobs[number_of_background_jobs-1].pid=-1;
                    number_of_background_jobs--;
                    break;
                }
                i++;
            }
            int current_status;
            signal(SIGTTIN,SIG_IGN);
            signal(SIGTTOU,SIG_IGN);
            tcsetpgrp(STDIN_FILENO,pid); 
            kill(pid,SIGCONT);
            waitpid(pid, &current_status, WUNTRACED);
            tcsetpgrp(STDIN_FILENO,getpgrp()); 
            signal(SIGTTIN,SIG_DFL);
            signal(SIGTTOU,SIG_DFL); 
            if(WIFSTOPPED(current_status))
            {   
                printf("Process with pid: %d is stopped\n",pid);
                last_command_status=-1;
                strcpy(Store_BackgroundJobs[number_of_background_jobs].Job_name,process_name);
                Store_BackgroundJobs[number_of_background_jobs].pid = pid;
                number_of_background_jobs++;
            }
        }
        else
        {
            last_command_status = -1;
            fprintf(stderr,"Invalid job number\n");
        }
    }
    else
    {
        last_command_status = -1;
        fprintf(stderr,"Invalid number of arguments\n");
    }
}   