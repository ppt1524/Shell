#include "commands.h"
#include "../helper.h"

void die(const char *s) {
    // perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

/**
 * Enable row mode for the terminal
 * The ECHO feature causes each key you type to be printed to the terminal, so you can see what you’re typing.
 * Terminal attributes can be read into a termios struct by tcgetattr().
 * After modifying them, you can then apply them to the terminal using tcsetattr().
 * The TCSAFLUSH argument specifies when to apply the change: in this case, it waits for all pending output to be written to the terminal, and also discards any input that hasn’t been read.
 * The c_lflag field is for “local flags”
 */
void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

/**
 * stdout and stdin are buffered we disable buffering on that
 * After entering in raw mode we read characters one by one
 * Up arrow keys and down arrow keys are represented by 3 byte escape codes
 * starting with ascii number 27 i.e. ESC key
 * This way we interpret arrow keys
 * Tabs are usually handled by the term, but here we are simulating tabs for the sake of simplicity
 * Backspace move the cursor one control character to the left
 * @return
 */
char * input() {
    char *inp = malloc(MAX_BUFFER_SIZE);
    char *inp1 = malloc(MAX_BUFFER_SIZE);
    char c;
    // while (1) {
        setbuf(stdout, NULL);
        enableRawMode();
        // printf("Prompt>");
        memset(inp, '\0', MAX_BUFFER_SIZE);
        memset(inp1, '\0', MAX_BUFFER_SIZE);
        int pt = 0;
        int pt1=0;
        while (read(STDIN_FILENO, &c, 1) == 1) {
            if(c==32)
            {
                pt1=0;
                memset(inp1, '\0', MAX_BUFFER_SIZE);
            }
            if (iscntrl(c)) {
                if (c == 10) break;
                else if (c == 27) {
                    char buf[3];
                    buf[2] = 0;
                    if (read(STDIN_FILENO, buf, 2) == 2) { // length of escape code
                        printf("\rarrow key: %s", buf);
                    }
                } else if (c == 127) { // backspace
                    if (pt > 0) {
                        if (inp[pt-1] == 9) {
                            for (int i = 0; i < 7; i++) {
                                printf("\b");
                            }
                        }
                        inp[--pt] = '\0';
                        inp1[--pt1] = '\0';
                        printf("\b \b");
                    }
                } else if (c == 9) { // TAB character
                    // inp[pt++] = c;
                    char *next;
                    next = (char *)malloc(MAX_BUFFER_SIZE);
                    // printf("check->%s\n",inp1);
                    // printf("here %ld\n",strlen(inp1));
                    next = autocomplete(inp1,inp);
                    if(next==NULL)
                    {
                        display_prompt();
                        continue;
                    }
                    int cnt=0;
                    while(next[cnt++])
                    {
                        printf("%c",next[cnt-1]);
                        inp[pt++]=next[cnt-1];
                        inp1[pt1++]=next[cnt-1];
                    }
                    // for (int i = 0; i < 8; i++) { // TABS should be 8 spaces
                    //     printf(" ");
                    // }
                } else if (c == 4) {
                    exit(0);
                } else {
                    printf("%d\n", c);
                }
            } else {
                inp[pt++] = c;
                if(c!=32)
                inp1[pt1++] = c;
                printf("%c", c);
            }
        }
        printf("\n");
        strcat(inp,"\n");
        disableRawMode();
        // printf("%s<-check",inp);
        return inp;
        // printf("\nInput Read: [%s]\n", inp);
    // }
}