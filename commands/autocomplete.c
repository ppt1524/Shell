#include "commands.h"
#include "../helper.h"


int IsDirr(char *path_of_directory) {
   struct stat for_directory;
   if (stat(path_of_directory, &for_directory) != 0)
       return 0;
   return S_ISDIR(for_directory.st_mode);
}
int counter=0;

char * compute_lcp(char ** strings,int number_of_strings,char *actual_inp)
{
    if(strings == NULL)return NULL;

    char * ans;
    ans = (char *)malloc(MAX_BUFFER_SIZE);
    char ch;
    int minn_length=strlen(strings[0]);
    for(int i=1;i<number_of_strings;i++)
    {
        if(strlen(strings[i])<minn_length)
            minn_length=strlen(strings[i]);
    }
    for(int i=0;i<minn_length;i++)
    {
        ch = strings[0][i];
        for(int j=1;j<number_of_strings;j++)
        {
            if(strings[j][i]!=ch)
            {
                if(number_of_strings>1 && i>0)
                {
                    printf("\n");
                    for(int k=0;k<number_of_strings;k++)
                    {
                        if(IsDirr(strings[k]))
                        printf("%s/\n",strings[k]);
                        else
                        printf("%s\n",strings[k]);
                    }
                    printf("\n");
                    display_prompt();
                    printf("%s",actual_inp);
                }
                else
                {
                    if(IsDirr(ans))
                    strcat(ans,"/");
                    else
                    strcat(ans," ");
                }
                return ans;
            }
        }
        strncat(ans,&ch,1);
    }
    if(number_of_strings>1)
    {
        printf("\n");
        for(int k=0;k<number_of_strings;k++)
        {
            if(IsDirr(strings[k]))
            printf("%s/\n",strings[k]);
            else
            printf("%s\n",strings[k]);
        }
        printf("\n");
        display_prompt();
        printf("%s",actual_inp);    
    }
    else
    {
        if(IsDirr(ans))
        strcat(ans,"/");
        else
        strcat(ans," ");
    }
    return ans;

}   
char* autocomplete(char input[],char actual_inp[])
{
    char *add_str;
    add_str = (char *)malloc(MAX_BUFFER_SIZE);

    char *directory_name;
    directory_name = (char *)malloc(MAX_BUFFER_SIZE);
    getcwd(directory_name,MAX_BUFFER_SIZE);

    struct dirent *file_ptr;
    DIR *directory = opendir(directory_name);

    if(directory==NULL)
    {
        last_command_status = -1;
        perror("Error : can't open directory");
        return NULL;
    }
    else
    {
        if(actual_inp[0]=='\0')
        {
            while((file_ptr = readdir(directory))!=NULL)
            {
                printf("%s\n",file_ptr->d_name);
            }
            printf("\n");
            return NULL;
        }
        char *temp,*check;
        char **temp_store;
        temp=(char *)malloc(MAX_BUFFER_SIZE);
        check=(char *)malloc(MAX_BUFFER_SIZE);
        temp_store=(char **)malloc(sizeof(char *)*(1LL<<8));
        for(int i=0;i<(1LL<<8);i++)
        temp_store[i]=(char *)malloc(MAX_BUFFER_SIZE);
        int cnt=0;
        int inp_len = strlen(input);



        while ((file_ptr = readdir(directory)) != NULL)
        {
            strcpy(check,file_ptr->d_name);
            int len = strlen(check);
            int ok=1;
            for(int i=0;i<inp_len;i++)
            {
                if(len<inp_len)
                {
                    ok=0;
                    break;
                }
                else if(input[i]!=check[i])
                {
                    ok=0;
                    break;
                }
            }
            if(ok)
            strcpy(temp_store[cnt++],check);
        }
        check = compute_lcp(temp_store,cnt,actual_inp);
        int len_check=strlen(check);
        temp  = check + inp_len;
        return temp;
    }
    return add_str;
}