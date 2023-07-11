#include "commands.h"
#include "../helper.h"

void echo()
{
    int curr_indx=1;
    for( ; Command[curr_indx] ; )
    {
        printf("%s ",Command[curr_indx++]);
        if(Command[curr_indx]==NULL)
        printf("\n");
    }
    return;
}