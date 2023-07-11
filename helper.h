#ifndef helper
#define helper

#include<stdio.h>  
#include<signal.h>
#include<string.h>
#include<stdlib.h>
#include<pwd.h>
#include <sys/time.h>
#include<dirent.h>
#include<time.h>
#include<grp.h>
#include<sys/types.h>
#include<sys/time.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<unistd.h>
#include<assert.h>
#include <fcntl.h>
#include <termios.h>
#include <ctype.h>

#define MAX_BUFFER_SIZE 1<<10
#define MAX_PROMPT_SIZE 1<<10
#define MAX_HOST_SIZE 1<<10
#define MAX_USERNAME_SIZE 1<<10
#define MAX_PWD_SIZE 1<<10
#define SIZE_OF_HISTORY 20

 
int last_command_status,number_of_arguments,history_indx,number_of_background_jobs,IS_Foreground;
pid_t shell_id;
time_t begin,endtime;

char *Prompt;
char *Home;
char *Command[MAX_BUFFER_SIZE]; 
char *Multiple_Commands[MAX_BUFFER_SIZE]; 
char * for_piping_command[MAX_BUFFER_SIZE];
char *last_working_directory;
char mode_permission[11];
char History[SIZE_OF_HISTORY][MAX_BUFFER_SIZE];
char path_for_history[MAX_BUFFER_SIZE];

void display_prompt();
int compile_command(char* st);
void execute_command(int indx);
int Is_Background_Process();
int compile_commands(char* st);
int compile_piped_commands(char *st);
void run();
void init_var();
void init_history();
void commit_history();
void modify_history(char * commands);
void display_history(int cnt);
void to_display_file_recursively(char *input_path,char *file_name,char *initials,int flag_d,int flag_f);
void Insert_Process(char ** Commands, int pid);

char *input();
typedef struct BackgroundJobs
{
    int pid;
    char Job_name[MAX_BUFFER_SIZE];
}BackgroundJobs;
BackgroundJobs Store_BackgroundJobs[MAX_BUFFER_SIZE];

#endif