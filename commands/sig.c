#include "commands.h"
#include "../helper.h"

void sig()
{
    if(number_of_arguments==3)
    {
        int job_number = atoi(Command[1]);
        int  signal_number = atoi(Command[2]);
        if(job_number<1 && job_number>number_of_background_jobs)
        {
            last_command_status = -1;
            fprintf(stderr,"Invalid job number\n");
        }
        else if(signal_number<1)
        {
            last_command_status = -1;
            fprintf(stderr,"Invalid Signal number\n");
        }
        else
            kill(Store_BackgroundJobs[job_number-1].pid,signal_number);
    }
    else
    {
        last_command_status = -1;
        fprintf(stderr,"Invalid number of arguments\n");
    }
}   