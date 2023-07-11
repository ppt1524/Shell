## C - Shell
A C-based unix shell with numerous features, such as :
 - Executing multiple shell commands in one line
 - Finds any specified directory or file using customly made `discovery` command
 - Running multiple background and foreground processes simultaneously
 - **`Autocomplete feature`**
 - **Piping**, **Redirection** and **Signal handing**
 - Also has user defined functions such as **jobs, fg, bg, sig**
 - Monitoring processes with customly made `pinfo` command

- **I have made this shell and did all the testing on `WSL (windows subsystem for linux)`**

# Usage
1. run `make`

2. execute `./main`

## Assumptions
1. The name of the system is of at max 1024 characters.
2. The path to the actual home of the user is of at max 1024 characters.
3. The length of each command is of at max 1024 .characters
4. The history command is implemented with the assumption that it's the only program changing the history file during execution.
5. Autocomplete gives completion for all the files in current directory but not in subdirectories

# Features
## Display Prompt
The prompt displays the user's username along with the system name, and the directory they are currently working in,
assuming the location from which the user executed the shell as the home directory.
## Parsing arguments
The shell can correctly interpret the command sent, handling multiple tabs and spaces in between as well without any issues
## Semi-colon
You can execute multiple shell commands in one line by separating them with `;`
## Autocomplete
Using **Tab** you can get the complete input if it matches with any file name / directory name.
## Builtin commands
There are 8 main builtin commands
### 1. cd
- Done as specified in the assignement

- **Usage:**

    ``` <abc@def:~> cd <directory> ```

### 2. echo
- Done as specified in the assignement

- **Usage:**

    ``` <abc@def:~> echo <message> ```

- This will display `<message>` in the terminal.
### 3. pwd
- Done as specified in the assignement

- **Usage:**

    ``` <abc@def:~> pwd ```

- Executing this will display the current absolute directory.

### 4. ls
- Done as specified in the assignement

- **Usage:**

    ``` <abc@def:~> ls <flags optional> <directory optional> ```

- Based on the flags and the directory specified, `ls` will display the directory information. 

### 5. pinfo
- pinfo prints information about a certain process. 
- If no pid is mentioned, the command runs pinfo on the current shell
- Else it runs on the pid mentioned, giving details such as:
    1. pid number
    2. process status
    3. memory
    4. executable path

- **Usage:**

    ``` <abc@def:~> pinfo <pid>  ```

### 6. discover
- Done as specified in assignment, in accordance to the example given in assignment.
- **Usage:**

    ``` <abc@def:~> discover <tagert_dir> <flags-optional> <file_name - optional> ```

### 7. history
- Done as specified in assignment
- **Usage:**

    ``` <abc@def:~> history ```

### 8. quit
- Enter quit to exit out of the terminal
- **Usage:**

    ``` <abc@def:~> quit ```

### 9. Handled Redirection and Piping and signal handing
- As in original linux terminal

### 10. User defined Commands - jobs, fg, bg
- As in original linux terminal
- In jobs also handled flags -r, -s to display only running and sleeping process

## Running system commands

### 1. Foreground processes:
- You can execute other system commands such as `code` with or without arguments as
a foreground process. You'll have to wait for the process to terminate before you can use your shell, 
or terminate it prematurely by using `Ctrl+C`. The time displayed on the prompt is the time taken by the last foreground process. 

- **Usage:** 

    ``` <abc@def:~> external-command arg1 arg2 ...  ```

    This will execute the external command with the arguments specified. 

### 2. Background processes:
 - By appending '&,' you can have a process run in the background while still using the shell. You can run multiple processes at the same time, and executing a background process prints the PID of the newly created process.

- **Usage:** 

    ``` <abc@def:~> command1 &  ```

- This will execute the command in the background

### 3. Finished background processes:
- When a background process is finished, the shell will print whether the process exited
normally or abnormally

## File Structure

```
<2021101077>_Assignment2/
    makefile - Contains makefile
    README.md - Contains description of commands and file structure
    ./main - The executable
    main.c - Has the main function of the shell 
    helper.c - Has the helper functions
    helper.h - Has the definations for helper functions
    history.txt - contains history
    commands/
        commands.h - Contains the defination for commands
        cd.c    - has functions used to implement cd
        discover.c - has functions used to implement discover
        echo.c  - has functions used to implement echo
        history.c - has functions used to impelement history
        ls.c    - has functions used to implement ls
        other_system_commands_for_fg.c - has functions used to implement foreground process
        pinfo.c - has functions used to implement pinfo
        pwd.c - has functions used to implement pwd
        quit.c - has functions used to implement quit
        autocomplete.c - has functions used to implement autocomplete
        bg.c - has function used to implement bg
        fg.c - has function used to implement fg
        input.c - has function to take low level input
        jobs.c - has function used to implement jobs
        pipe.c - has functions used to handle piping
        redirection.c - has functions used to handle redirection
        sig.c - has function used to implement sig
```
