#include "helper.h"
#include "commands/commands.h"
int main()
{
    init_var();
    init_history();
    while(1)
    {
        display_prompt(); // starts and continues the execution of the prompt
        run(); // take input of the command and then execute it
    }
    return 0;
}