#include "commands.h"
#include "../helper.h"

void bg()
{
    if(number_of_arguments==2)
    {
        int val = atoi(Command[1]);
        if(val>0 && val<=number_of_background_jobs)
            kill(Store_BackgroundJobs[val-1].pid,SIGCONT);
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