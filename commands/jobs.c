#include "commands.h"
#include "../helper.h"

struct for_sorting
{
    int indx;
    char * store;
    char * job_name;
    int job_pid;
};

int Comp(const void *a, const void *b)
{
    return strcmp(((struct for_sorting *)a)->job_name,((struct for_sorting *)b)->job_name) >= 0;
}
struct for_sorting memo[MAX_BUFFER_SIZE];
char * helper_function_for_jobs(int len,char *file_name)
{
    char *string=strtok(file_name," \r\t");
    int Cnt=0;
    while(Cnt<len-1)
    {
        string=strtok(NULL, " \r\t");
        Cnt++;
    }
    return string;
}
void jobs()
{
    int c_cnt=0;
    for(int i=0;i<number_of_background_jobs;i++)
    {
        char *proc_file_path;
        proc_file_path=(char *)malloc(MAX_BUFFER_SIZE);
        sprintf(proc_file_path,"/proc/%d/stat",Store_BackgroundJobs[i].pid);
        FILE * file_ptr = fopen(proc_file_path,"r");
        if(file_ptr!=NULL)
        {
            char *temp_ptr, *store;
            store=(char *)malloc(MAX_BUFFER_SIZE);
            temp_ptr=(char *)malloc(MAX_BUFFER_SIZE);
            fgets(temp_ptr,MAX_BUFFER_SIZE,file_ptr);
            strcpy(store,helper_function_for_jobs(3,temp_ptr));
            if(strcmp(store,"T") && strcmp(store,"I"))
                strcpy(store,"Running");
            else if(strcmp(store,"T"))
                strcpy(store,"Interrupted");
            else
                strcpy(store,"Stopped");
            
            memo[i].indx=i+1;
            memo[i].store=(char *)malloc(MAX_BUFFER_SIZE);
            memo[i].job_name=(char *)malloc(MAX_BUFFER_SIZE);
            strcpy(memo[i].store,store);
            // memo[i].store=store;
            memo[i].job_name=Store_BackgroundJobs[i].Job_name;
            memo[i].job_pid=Store_BackgroundJobs[i].pid;
            c_cnt++;
            free(temp_ptr);
            free(store);
        }
        else
        {
            last_command_status=-1;
            fprintf(stderr,"Can't open process directory for process %d",Store_BackgroundJobs[i].pid);
        }
        free(proc_file_path);
    }
    qsort(memo,c_cnt,sizeof(struct for_sorting),Comp);
    for(int i=0;i<c_cnt;i++)
    {
        if(Command[1]==NULL)
            printf("[%d] %s %s [%d]\n",memo[i].indx,memo[i].store,memo[i].job_name,memo[i].job_pid);
            
        else if(strcmp(Command[1],"-r") && strcmp(memo[i].store,"Running"))
        {
            printf("[%d] %s %s [%d]\n",memo[i].indx,memo[i].store,memo[i].job_name,memo[i].job_pid);
        }
        else if(strcmp(Command[1],"-s") && strcmp(memo[i].store,"Stopped"))
            printf("[%d] %s %s [%d]\n",memo[i].indx,memo[i].store,memo[i].job_name,memo[i].job_pid);      
    }
}   