#ifndef commnads
#define commands

void cd();
void echo();
void pwd();
void quit();
void ls();
void history();
void pinfo();
void discover();
void Pipe(int curr_indx);
void other_system_commands(int Is_Background);
void Redirect(int curr_indx);
char *autocomplete(char *,char *);
void jobs();
void bg();
void fg();
void sig();

#endif