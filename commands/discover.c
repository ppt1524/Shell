#include "commands.h"
#include "../helper.h"
int IsDir(char *directory_name) {
    if(directory_name==NULL)return 0;
    struct stat for_directory;
    if (stat(directory_name, &for_directory) != 0)
    return 0;
    return S_ISDIR(for_directory.st_mode);
}
void to_display_file_recursively(char *input_path,char *file_name,char *initials,int flag_d,int flag_f)
{
    char *temp_path;
    struct dirent *for_dirent;
    DIR *dir = opendir(input_path);
    if (!dir)
    return;

    char *modify_init;
    while ((for_dirent = readdir(dir)) != NULL)
    {
        char *temp=(char*)malloc(MAX_BUFFER_SIZE);
        strcpy(temp,initials);
        strcat(temp,"/");
        strcat(temp,for_dirent->d_name);
        if (strcmp(for_dirent->d_name, ".") != 0 && strcmp(for_dirent->d_name, "..") != 0)
        {
            if(flag_d)
            {
                if((strcmp(for_dirent->d_name,file_name)==0 || strcmp(file_name,"#")==0) && IsDir(temp))
                {
                    printf("%s/",initials);
                    printf("%s\n",for_dirent->d_name);
                }
            }
            if(flag_f  && !IsDir(temp))
            {
                if(strcmp(for_dirent->d_name,file_name)==0 || strcmp(file_name,"#")==0)
                {
                    printf("%s/",initials);
                    printf("%s\n",for_dirent->d_name);
                }
            }
            else if(flag_d ==0 &&  flag_f == 0)
            {
                if(strcmp(for_dirent->d_name,file_name)==0 || strcmp(file_name,"#")==0)
                {
                    printf("%s/",initials);
                    printf("%s\n",for_dirent->d_name);
                }   
            }
            
            temp_path=(char*)malloc(strlen(input_path)+10+strlen(for_dirent->d_name));
            strcpy(temp_path, input_path);
            strcat(temp_path, "/");
            strcat(temp_path, for_dirent->d_name);

            modify_init=(char *)malloc((strlen(input_path)+10+strlen(for_dirent->d_name)));
            strcpy(modify_init, initials);
            strcat(modify_init,"/");
            strcat(modify_init,for_dirent->d_name);

            to_display_file_recursively(temp_path,file_name,modify_init,flag_d,flag_f);
        }
    }
    closedir(dir);
}

void discover()
{
    int dis_d=0,dis_f=0,dis_curr_indx=1;
    char *input_path;
    char *file_name;
    char *initials;
    input_path=(char *)malloc(MAX_BUFFER_SIZE);
    file_name=(char *)malloc(MAX_BUFFER_SIZE);
    initials=(char *)malloc(MAX_BUFFER_SIZE);

    if(number_of_arguments>=6)
    {
        last_command_status=-1;
        perror("More Arguments given than possible");
        return;
    }
    while(Command[dis_curr_indx]!=NULL)
    {
        if(Command[dis_curr_indx][0]=='-')
        {
            int length = strlen(Command[dis_curr_indx]);
            for (int i = 0; i < length; i++)
            {
                if (Command[dis_curr_indx][i] == 'd')
                    dis_d = 1;
                else if (Command[dis_curr_indx][i] == 'f')
                    dis_f = 1;
            }
        }
        if(Command[dis_curr_indx][0]=='"')
        {
            for(int i=1;i<strlen(Command[dis_curr_indx])-1;i++)
            file_name[i-1]=Command[dis_curr_indx][i];
            file_name[strlen(Command[dis_curr_indx])-2]='\0';
        }
        dis_curr_indx++;
    }
    if(IsDir(Command[1])==1)
    {
        strcpy(input_path,Command[1]);
        strcpy(initials,Command[1]);
    }
    else
    {
        if(strlen(file_name)==0)
        printf(".\n");
        getcwd(input_path,MAX_BUFFER_SIZE);
    }
    if(strlen(file_name)==0)
    strcpy(file_name,"#");
    
    if(strlen(initials)==0)
    {
        strcpy(initials,".");
    }
    to_display_file_recursively(input_path,file_name,initials,dis_d,dis_f);
}