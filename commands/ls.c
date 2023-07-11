#include "commands.h"
#include "../helper.h"
mode_t permissions_macros[10] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};
int sum=0;

int cmp(const void *a, const void *b)
{
    // printf("%s %s\n",*(char**)a,*(char**)b);
    return strcmp(*(char **)a, *(char **)b) >= 0;
}

int IsDirectory(char *path_of_directory) {
   struct stat for_directory;
   if (stat(path_of_directory, &for_directory) != 0)
       return 0;
   return S_ISDIR(for_directory.st_mode);
}

void return_permissions(char *filename)
{
    struct stat file;
    if (stat(filename, &file) == -1)
    {
        perror("Error occured");
        return;
    }
    else
    {
        mode_t file_perm_mode = file.st_mode;
        char temp[3] = {'r', 'w', 'x'};
        for (int i = 0; i < 9; i++)
        mode_permission[i] = permissions_macros[i] & file_perm_mode ? temp[i % 3] : '-';
        mode_permission[9] = '\0';
    }
}

void show_complete_ls(char *adress_of_directory, char *file_name)
{
    char *adress_of_file;
    adress_of_file = (char *)malloc(MAX_BUFFER_SIZE);
    strcpy(adress_of_file, adress_of_directory);

    strcat(adress_of_file, "/");
    strcat(adress_of_file, file_name);
    struct stat info;
    if (stat(adress_of_file, &info) == -1)
    {
        last_command_status = -1;
        perror("Error in execution of stat");
        return;
    }
    else
    {
        // sum+=info.st_blocks;
        return_permissions(adress_of_file);

        char *time = (char *)malloc(50);
        struct group *grp = getgrgid(info.st_gid);
        struct passwd *psswd = getpwuid(info.st_uid);
        strftime(time, 50, "%b  %d %H:%M", localtime(&info.st_mtime));
        if (!S_ISDIR(info.st_mode) && mode_permission[2] == 'x') // executable -> green
        {
            printf("-%s   %ld\t%s\t %s\t%ld\t%s\t\033[0;32m%s\033[0m\n", mode_permission, info.st_nlink, psswd->pw_name, grp->gr_name, info.st_size, time, file_name);
        }
        else if(!S_ISDIR(info.st_mode) && mode_permission[2]!='x')// file -> white
        {
            printf("-%s   %ld\t%s\t %s\t%ld\t%s\t%s\n", mode_permission, info.st_nlink, psswd->pw_name, grp->gr_name, info.st_size, time, file_name);
        }
        else // directory -> blue
            printf("d%s   %ld\t%s\t %s\t%ld\t%s\t\033[1;34m%s\033[0m\n", mode_permission, info.st_nlink, psswd->pw_name, grp->gr_name, info.st_size, time, file_name);
        free(adress_of_file);
    }
}
void compute_total(char * adress_of_directory,char *file_name)
{
    char *adress_of_file;
    adress_of_file = (char *)malloc(MAX_BUFFER_SIZE);
    strcpy(adress_of_file, adress_of_directory);

    strcat(adress_of_file, "/");
    strcat(adress_of_file, file_name);
    struct stat info;
    if (stat(adress_of_file, &info) == -1)
    {
        last_command_status = -1;
        perror("Error in execution of stat");
        return;
    }
    else
    {
        sum+=info.st_blocks;
        // free(adress_of_file);
    }   
}
void display_ls(char *adress_of_directory, int ls_a, int ls_l)
{
    // printf("%s check %d %d\n",adress_of_directory,!IsDirectory(adress_of_directory),ls_l);
    // sum=0;
    if (!IsDirectory(adress_of_directory) && !ls_l)
    {
        // printf("hello");
        char *adress_of_file;
        adress_of_file = (char *)malloc(MAX_BUFFER_SIZE);
        strcpy(adress_of_file, adress_of_directory);
        return_permissions(adress_of_file);
        struct stat info;
        if (stat(adress_of_file, &info) == 0 && S_ISDIR(info.st_mode)) // directory blue
        {
            printf("\033[1;34m%s\033[0m    ", adress_of_file);
        }
        else if (mode_permission[2] == 'x')
        {
            printf("\033[1;32m%s\033[0m    ", adress_of_file);
        }
        else
        {
            printf("%s    ", adress_of_file);
        }
        // cnt++;
        free(adress_of_file);
    }
    else if(!IsDirectory(adress_of_directory))
    {
        char *adress_of_file;
        adress_of_file = (char *)malloc(MAX_BUFFER_SIZE);
        strcpy(adress_of_file, adress_of_directory);
        
        struct stat info;
        if (stat(adress_of_file, &info) == -1)
        {
            last_command_status = -1;
            perror("Error in execution of stat");
            return;
        }
        else
        {
            return_permissions(adress_of_file);
            char *time = (char *)malloc(50);
            struct group *grp = getgrgid(info.st_gid);
            struct passwd *psswd = getpwuid(info.st_uid);
            strftime(time, 50, "%b  %d %H:%M", localtime(&info.st_mtime));

            if (!S_ISDIR(info.st_mode) && mode_permission[2] == 'x') // executable -> green
            {
                printf("-%s   %ld\t%s\t %s\t%ld\t%s\t\033[0;32m%s\033[0m\n", mode_permission, info.st_nlink, psswd->pw_name, grp->gr_name, info.st_size, time, adress_of_file);
            }
            else if(!S_ISDIR(info.st_mode) && mode_permission[2]!='x')// file -> white
            {
                printf("-%s   %ld\t%s\t %s\t%ld\t%s\t%s\n", mode_permission, info.st_nlink, psswd->pw_name, grp->gr_name, info.st_size, time, adress_of_file);
            }
            else // directory -> blue
                printf("d%s   %ld\t%s\t %s\t%ld\t%s\t\033[1;34m%s\033[0m\n", mode_permission, info.st_nlink, psswd->pw_name, grp->gr_name, info.st_size, time, adress_of_file);
            free(adress_of_file);
        }
    }
    else 
    {
        struct dirent *file_ptr;
        DIR *directory = opendir(adress_of_directory);
        if (directory == NULL)
        {
            last_command_status = -1;
            perror("Error : can't open directory");
            return;
        }
        else
        {
            // printf("%s\n",adress_of_directory);
            char **for_sorting;
            int *for_colour;
            int cnt = 0;
            while ((file_ptr = readdir(directory)) != NULL)
                cnt++;
            directory = opendir(adress_of_directory);
            for_sorting = (char **)malloc(cnt * sizeof(char *));
            for_colour = (int *)malloc(cnt * sizeof(int));
            cnt = 0;
            while ((file_ptr = readdir(directory)) != NULL)
            {
                if (ls_a == 0 && file_ptr->d_name[0] == '.')
                    continue;
                if(ls_l==1)
                compute_total(adress_of_directory, file_ptr->d_name);
            }
            directory = opendir(adress_of_directory);
            if(sum!=0)
            printf("total : %d\n",sum/2);
            while ((file_ptr = readdir(directory)) != NULL)
            {
                if (ls_a == 0 && file_ptr->d_name[0] == '.')
                    continue;
                if (ls_l == 0)
                {
                    char *adress_of_file;
                    adress_of_file = (char *)malloc(MAX_BUFFER_SIZE);
                    struct stat info;
                    strcpy(adress_of_file, adress_of_directory);
                    strcat(adress_of_file, "/");
                    strcat(adress_of_file, file_ptr->d_name);
                    return_permissions(adress_of_file);
                    if (stat(adress_of_file, &info) == 0 && S_ISDIR(info.st_mode)) // directory blue
                    {
                        for_sorting[cnt] = (char *)malloc(strlen(file_ptr->d_name));
                        strcpy(for_sorting[cnt], file_ptr->d_name);
                        for_colour[cnt] = 1;
                    }
                    else if (mode_permission[2] == 'x')
                    {
                        for_sorting[cnt] = (char *)malloc(strlen(file_ptr->d_name));
                        strcpy(for_sorting[cnt], file_ptr->d_name);
                        for_colour[cnt] = 2;
                        // printf("\033[1;32m%s\033[0m    ",file_ptr->d_name);
                    }
                    else
                    {
                        for_sorting[cnt] = (char *)malloc(strlen(file_ptr->d_name));
                        strcpy(for_sorting[cnt], file_ptr->d_name);
                        for_colour[cnt] = 3;
                        // printf("%s    ",file_ptr->d_name);
                    }
                    cnt++;
                    free(adress_of_file);
                }
                else
                {
                    show_complete_ls(adress_of_directory, file_ptr->d_name);
                }
            }
            if (ls_l == 0)
            {
                qsort(for_sorting, cnt, sizeof(char *), cmp);
                for (int i = 0; i < cnt; i++)
                {
                    if (for_colour[i] == 1)
                        printf("\033[1;34m%s\033[0m    ", for_sorting[i]);
                    else if (for_colour[i] == 2)
                        printf("\033[1;32m%s\033[0m    ", for_sorting[i]);
                    else
                        printf("%s    ", for_sorting[i]);
                }
                printf("\n");
            }
            closedir(directory);
        }
    }
}
void ls()
{
    int curr_indx = 1, ls_a = 0, ls_l = 0, is_printed = 0;
    int sum=0;
    while(Command[curr_indx]!=NULL)
    {
        if(Command[curr_indx][0]=='-')
        {
            int length = strlen(Command[curr_indx]);
            for (int i = 0; i < length; i++)
            {
                if (Command[curr_indx][i] == 'a')
                    ls_a = 1;
                else if (Command[curr_indx][i] == 'l')
                    ls_l = 1;
            }
        }
        else
        {
            struct stat info;
            stat(Command[curr_indx], &info);
            sum+=info.st_blocks;
        }    
        curr_indx++;
    }
    // printf("%d ",sum);
    curr_indx=1;
    while (Command[curr_indx] != NULL)
    {
        // is_printed = 0;
        if (Command[curr_indx][0] != '-') // it's a directory
        {
            char *adress_of_directory;
            adress_of_directory = (char *)malloc(MAX_BUFFER_SIZE);
            if (Command[curr_indx][0] != '~')
            strcpy(adress_of_directory, Command[curr_indx]);
            else
            {
                strcpy(adress_of_directory, Home);
                strcat(adress_of_directory, Command[curr_indx] + 1);
                strcat(adress_of_directory, "\0");
            }
            display_ls(adress_of_directory, ls_a, ls_l);
            is_printed = 1;
            printf("\n");
            free(adress_of_directory);
        }
        else // there is a flag
        {
            int length = strlen(Command[curr_indx]);
            for (int i = 0; i < length; i++)
            {
                if (Command[curr_indx][i] == 'l')
                    ls_l = 1;
                else if (Command[curr_indx][i] == 'a')
                    ls_a = 1;
            }
        }
        curr_indx++;
    }
    if (is_printed == 0)
        display_ls(".", ls_a, ls_l);
}
