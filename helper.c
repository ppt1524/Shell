#include "helper.h"
#include "commands/commands.h"

void execute_command(int curr_indx)
{
    // printf("%d\n",curr_indx);
    if(Multiple_Commands[curr_indx]!=NULL && strstr(Multiple_Commands[curr_indx],"|")!=NULL)
    {
        Pipe(curr_indx);
        return;
    }
    last_command_status=0;
    if(Multiple_Commands[curr_indx]!=NULL && (strstr(Multiple_Commands[curr_indx],">")!=NULL || strstr(Multiple_Commands[curr_indx],"<")!=NULL))
    Redirect(curr_indx);
    else if(strcmp(Command[0],"cd")==0)
    cd();
    else if(strcmp(Command[0],"pwd")==0)
    pwd();
    else if(strcmp(Command[0],"echo")==0)
    echo();
    else if(strcmp(Command[0],"quit")==0)
    quit();
    else if(strcmp(Command[0],"ls")==0)
    ls();
    else if(strcmp(Command[0],"discover")==0)
    discover();
    else if(strcmp(Command[0],"pinfo")==0)
    pinfo();
    else if(strcmp(Command[0],"jobs")==0)
    jobs();
    else if(strcmp(Command[0],"bg")==0)
    bg();
    else if(strcmp(Command[0],"fg")==0)
    fg();
    else if(strcmp(Command[0],"sig")==0)
    sig();
    else if(strcmp(Command[0],"history")==0)
    {
        int cnt;
        if(number_of_arguments==1)cnt=10;
        else if(number_of_arguments>2)cnt=-1;
        else cnt=atoi(Command[1]);
        display_history(cnt);
        commit_history();
    }
    else if(!Is_Background_Process())
    other_system_commands(0);
    else
    other_system_commands(1);
}
void run()
{
    last_command_status=1;
    char *list_of_commands;
    list_of_commands = (char *)malloc(MAX_BUFFER_SIZE);
    list_of_commands = input();
    // if(fgets(list_of_commands,MAX_BUFFER_SIZE,stdin)==NULL) // to take multi word input
    // strcpy(list_of_commands,input());
    // // list_of_commands=input();
    // // strcpy(list_of_commands,input());
    if(list_of_commands == NULL) // to take multi word input
    {
        exit(EXIT_SUCCESS);
    }
    if(strcmp(list_of_commands,"\n")==0)return;  // if user presses enter then return
    last_command_status=0;

    modify_history(list_of_commands);

    int number_of_commands=compile_commands(list_of_commands),curr_indx=0;
    for( ; Multiple_Commands[curr_indx] ; )
    {
        char *temp_store=(char*)malloc(MAX_BUFFER_SIZE);
        strcpy(temp_store,Multiple_Commands[curr_indx]);

        int last_process_is_background;
        if(temp_store[strlen(temp_store)-2]=='&')last_process_is_background=1;

        int t_cnt=0;
        char *store_for_and[MAX_BUFFER_SIZE];
        for(int kk=0;kk<MAX_BUFFER_SIZE;kk++)
        store_for_and[kk]=(char*)malloc(MAX_BUFFER_SIZE);
        strcpy(store_for_and[t_cnt],strtok(temp_store,"&"));
        for( ; store_for_and[t_cnt] ; ) 
        {
            char *temp_str=strtok(NULL,"&");
            if(temp_str==NULL)
            {
                t_cnt=t_cnt+1;
                store_for_and[t_cnt]=NULL;
                break;
            }
            t_cnt=t_cnt+1;
            strcpy(store_for_and[t_cnt],temp_str);
            if(store_for_and[t_cnt]==NULL)
            {
                if(last_process_is_background)
                strcat(store_for_and[t_cnt-1],"&");
            }
            else
            strcat(store_for_and[t_cnt-1],"&");
        }
        // printf("t_cnt=%d\n",t_cnt);
            // printf("%s\n",store_for_and[0]);/
        int new_cnt=0;
        while(new_cnt<t_cnt)
        {
            // printf("%s\n",store_for_and[new_cnt]);
            number_of_arguments=compile_command(store_for_and[new_cnt])-1;
            if(number_of_arguments<0)break;
            execute_command(new_cnt++);
        }
        // number_of_arguments=compile_command(temp_store)-1;
        // if(number_of_arguments<0)break;
        // execute_command(curr_indx);
        curr_indx++;
        free(temp_store);
    }
}
void background_child_signal()
{
    int curr_status;
    int pid = waitpid(-1,&curr_status, WNOHANG);
    int curr_indx=0;
    if(pid<=0)
    return;
    else
    {
        char *Process_Name;
        Process_Name=(char*)malloc(MAX_BUFFER_SIZE);

        for(int i=0;i<number_of_background_jobs;i++)
        {
            if(Store_BackgroundJobs[i].pid != pid)continue;
            strcpy(Process_Name,Store_BackgroundJobs[i].Job_name);
            curr_indx=i;
            for(int j=i+1;j<number_of_background_jobs;j++)
            {
                Store_BackgroundJobs[j-1].pid = Store_BackgroundJobs[j].pid;
                strcpy(Store_BackgroundJobs[j-1].Job_name,Store_BackgroundJobs[j].Job_name);
            }
            Store_BackgroundJobs[--number_of_background_jobs].pid = -1;
            break;
        }
        if(WEXITSTATUS(curr_status)==EXIT_FAILURE || !WIFEXITED(curr_status))
            fprintf(stderr,"\033[1;31m\n%s with pid %d failed to exit normally.\033[0m\n",Process_Name,pid);
        else
            fprintf(stderr,"\033[1;31m\n%s with pid %d exited normally.\n\033[0m",Process_Name,pid);
        display_prompt();
        fflush(stdout);
    }
    return;
}
void init_var()
{
    signal(SIGTSTP,SIG_IGN);
    signal(SIGINT,SIG_IGN);
    signal(SIGCHLD,background_child_signal); // to give the parent process the info that the child prcosess is over
    last_command_status=1;
    last_working_directory=(char *)malloc(MAX_BUFFER_SIZE);
    strcpy(last_working_directory,"not_set");
    Home=(char *)malloc(MAX_BUFFER_SIZE);
    getcwd(Home,MAX_BUFFER_SIZE);
    strcpy(path_for_history,Home);
    strcat(path_for_history,"/history.txt");
}
void display_prompt() 
{
    struct passwd *pwd;
    pwd=getpwuid(getuid());
    char *username,*c_pwd,*hostname;

    Prompt=(char *)malloc(MAX_PROMPT_SIZE);
    hostname=(char *)malloc(MAX_HOST_SIZE);
    c_pwd=(char *)malloc(MAX_PWD_SIZE);

    gethostname(hostname,MAX_HOST_SIZE);
    getcwd(c_pwd,MAX_PWD_SIZE);
    username=pwd->pw_name;

    char *home_sub_dir=strstr(c_pwd,Home);
    char actual_location[MAX_BUFFER_SIZE];
    if(home_sub_dir!=NULL)
    {
        strcpy(actual_location,"~");
        strcat(actual_location,c_pwd+strlen(Home));
    }
    else
    {
        strcpy(actual_location,c_pwd);
    }
    // printf("%ld %ld\n",endtime,begin);
    int time_taken=endtime-begin;
    if(time_taken>0 && IS_Foreground)
    sprintf(Prompt,"\033[1;32m<\033[0;33m%s@%s:\033[0;35m%s\033[0m\033[1;32m took %ds>\033[0m",username,hostname,actual_location,time_taken);
    else
    sprintf(Prompt,"\033[1;32m<\033[0;33m%s@%s:\033[0;35m%s\033[0m\033[1;32m>\033[0m",username,hostname,actual_location);
    // endtime=begin=0;
    if(last_command_status==0) // success
    {
        if(time_taken>0 && IS_Foreground)
        printf("\n\033[0;31m:) ==> %s\033[0m",Prompt);
        else
        printf("\033[0;31m:) ==> %s\033[0m",Prompt);
        fflush(stdout);
    }
    else if(last_command_status==-1) // faliure
    {
        printf("\033[0;31m:( ==> %s\033[0m",Prompt);
        fflush(stdout);
    }
    else
    {
        printf("\033[0;31m:) ==> %s\033[0m",Prompt);
        fflush(stdout);
    }
    time_taken=0;
    free(Prompt);
}
int compile_command(char *st)
{
    // printf(",%s, check",st);
    int curr_indx=0;
    
    if(st==NULL || strcmp(st,"\n")==0)return -1;

    char *temp_store=(char *)malloc(strlen(st)+10);
    strcpy(temp_store,st);
    if(strtok(temp_store," \r\n\t")==NULL)
    return -1;

    for(int i=0;i<MAX_BUFFER_SIZE;i++)
    Command[i]=(char *)malloc(MAX_BUFFER_SIZE);

    strcpy(Command[curr_indx],strtok(st," \t\r\n"));
    for( ; Command[curr_indx] ; ) 
    {
        char *temp_str=strtok(NULL," \t\r\n");
        if(temp_str==NULL)
        {
            curr_indx=curr_indx+1;
            Command[curr_indx]=NULL;
            break;
        }
        curr_indx=curr_indx+1;
        strcpy(Command[curr_indx],temp_str);
    }
    return curr_indx+1;
}
int compile_commands(char *st)
{
    for(int i=0;i<MAX_BUFFER_SIZE;i++)
    Multiple_Commands[i]=(char *)malloc(MAX_BUFFER_SIZE);

    int curr_indx=0;

    strcpy(Multiple_Commands[curr_indx],strtok(st,";"));
    for( ; Multiple_Commands[curr_indx] ; )
    {
        char *temp_cmnd=strtok(NULL,";");
        if(temp_cmnd!=NULL)
        {
            curr_indx=curr_indx+1;
            strcpy(Multiple_Commands[curr_indx],temp_cmnd);
        }
        if(temp_cmnd==NULL)
        {
            curr_indx=curr_indx+1;
            Multiple_Commands[curr_indx]=NULL;
            break;
        }
    }
    return curr_indx+1;
}

int compile_piped_commands(char *st)
{
    int curr_indx=0;
    
    if(st==NULL || strcmp(st,"\n")==0)return -1;

    char *temp_store=(char *)malloc(strlen(st)+10);
    strcpy(temp_store,st);
    if(strtok(temp_store,"|")==NULL)
    return -1;

    for(int i=0;i<MAX_BUFFER_SIZE;i++)
    for_piping_command[i]=(char *)malloc(MAX_BUFFER_SIZE);

    strcpy(for_piping_command[curr_indx],temp_store);
    for( ; for_piping_command[curr_indx] ; ) 
    {
        char *temp_str=strtok(NULL,"|");
        if(temp_str==NULL)
        {
            curr_indx=curr_indx+1;
            for_piping_command[curr_indx]=NULL;
            break;
        }
        curr_indx=curr_indx+1;
        strcpy(for_piping_command[curr_indx],temp_str);
    }
    return curr_indx;
}
void Insert_Process(char ** Commnd,int pid)
{
    strcpy(Store_BackgroundJobs[number_of_background_jobs].Job_name,Commnd[0]);
    // printf("check %s %d\n",Commnd[0],number_of_background_jobs);
    int curr_indx=1;
    Store_BackgroundJobs[number_of_background_jobs].pid=pid;
    for( ; Command[curr_indx] ; ) 
    {
        strcat(Store_BackgroundJobs[number_of_background_jobs].Job_name," ");
        strcat(Store_BackgroundJobs[number_of_background_jobs].Job_name,Commnd[curr_indx++]);
    }
    number_of_background_jobs++;
}
int Is_Background_Process()
{
    IS_Foreground=1;
    if(number_of_arguments<1)exit(EXIT_FAILURE);
    if(Command[number_of_arguments-1][strlen(Command[number_of_arguments-1])-1]=='&' && strcmp(Command[number_of_arguments-1],"&")!=0)
    {
        Command[number_of_arguments-1][strlen(Command[number_of_arguments-1])-1]='\0';
        IS_Foreground=0;
    }
    else if(strcmp(Command[number_of_arguments-1],"&")==0)
    {
        Command[number_of_arguments-1]=NULL;
        IS_Foreground=0;
    }
    return !IS_Foreground;
}