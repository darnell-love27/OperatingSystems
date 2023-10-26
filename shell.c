#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#define MAX_COMMAND_LINE_LEN 1024
#define MAX_COMMAND_LINE_ARGS 128

char prompt[80];
char delimiters[] = " \t\r\n";
extern char **environ;
volatile sig_atomic_t finish = 0;

// Signal Handling Function
void sigintHandler(int sig_number) {
    signal(SIGINT, sigintHandler);
    printf("\n");
    fflush(stdout); //Flushes the output
}

// Background Processing Function
void background_process(int sig) {
    int status;
    pid_t pid = 0; //Assigns process ID
    while ((pid == waitpid(-1, &status, WNOHANG)) > 0) {
        printf("Background process with ID %d exited. \n", pid);
    }
}

// Alarm/Timer Handling Function
void timerHandler(int sig_number){
    finish = 1; //flag
    fflush(stdout); //Flushes output
    exit(EXIT_FAILURE); //Exits program for failure
}

// 
void echo_command(char *arguments[]) {
    int word = 1; // Variable for indexing
    while (arguments[word] != NULL) {
        //If word is environment variable -> Extracts variable name without the '$' and accesses it
        if (arguments[word][0] == '$') { 
            char *variable = arguments[word] + 1;
            char *env_var = getenv(variable);
            // Prints environment variable, otherwise print error then message
            if (env_var != NULL) {
                printf("%s ", env_var);
            } else {
                printf("Environment variable %s not found. ", variable);
            }
        } else {
            printf("%s ", arguments[word]);
        } // Continues to next word in argument
        word++;
    }
    printf("\n");
}


int main() {
    // Stores the string typed into the command line.
    char command_line[MAX_COMMAND_LINE_LEN];
    char cmd_bak[MAX_COMMAND_LINE_LEN];
  
    // Stores the tokenized command line input.
    char *arguments[MAX_COMMAND_LINE_ARGS];

    // Store string within the command line
    signal(SIGINT, sigintHandler);
    signal(SIGCHLD, background_process);
    signal(SIGALRM, timerHandler);

    int i;

    while (true) {

        finish = 0; // Flag variable
      
        do{ 

            // Print the shell prompt.
             // 0. Modify the prompt to print the current working directory
            if (getcwd(prompt, sizeof(prompt)) == NULL){
                perror("getcwd() error");
                exit(1);
            }
            else {
                printf("%s> ", prompt);
                fflush(stdout);
            }


            // Read input from stdin and store it in command_line. If there's an
            // error, exit immediately. (If you want to learn more about this line,
            // you can Google "man fgets")
        
            if ((fgets(command_line, MAX_COMMAND_LINE_LEN, stdin) == NULL) && ferror(stdin)) {
                fprintf(stderr, "fgets error");
                exit(0);
            }
 
        } while(command_line[0] == 0x0A);  // while just ENTER pressed

        // If the user input was EOF (ctrl+d), exit the shell.
        if (feof(stdin)) {
            printf("\n");
            fflush(stdout);
            fflush(stderr);
            return 0;
        }

        // TODO:

        // 1. Tokenize the command line inputs (split it on whitespace)
        arguments[0] = strtok(command_line, delimiters);
        i = 0;

        // While the variable does not equal NULL -> move to each index and tokenize it
        while (arguments[i] != NULL){
            i++;
            arguments[i] = strtok(NULL, delimiters);
        }
      
        // 2. Implement Built-In Commands
        if (strcmp(arguments[0], "cd") == 0) {
            chdir(arguments[1]);
        } else if (strcmp(arguments[0], "pwd") == 0) {
            printf("%s\n", prompt);
        } else if (strcmp(arguments[0], "echo") == 0) {
            echo_command(arguments);
        } else if (strcmp(arguments[0], "exit") == 0){
            exit(0);
        } else if (strcmp(arguments[0], "env") == 0){
            if (arguments[1] == NULL) {
                char **env = environ;
                while (*env) {
                    printf("%s\n", *env);
                    env++;
                }
            } else {
                char *env_var = getenv(arguments[1]);
                if (env_var != NULL) {
                    printf("%s\n", env_var);
                } else {
                    printf("Environment variable %s not found\n", arguments[1]);
                }
            }
        } else if (strcmp(arguments[0], "setenv") == 0){
            printf("setenv");
        }
    
    
        // 3. Create a child process which will execute the command line input
        // 4. The parent process should wait for the child to complete unless its a background process
        else {
            pid_t pid;
            int status;
            int flag = 0;
            i = 0;

            if (i>0 && strcmp(arguments[i - 1], "&") == 0) {
                flag = 1;
                arguments[i - 1] = NULL; //Removes the "&" from arguments
            }

            pid = fork();

            //Sets the alarm
            alarm(10);

            // If the process is classified as a child process -> Execute command & error message
            if (pid == 0) {
                execvp(arguments[0], arguments);
                perror("execvp() failed");
                exit(1);
            } else if (pid > 0) {
                if (!flag) {
                    waitpid(pid, &status, 0); //Waits for child process to finish
                } else {
                    printf("Background process & ID: %d\n", pid);
                }
            } else {
                // If fork fails
                perror("fork");
                exit(1);
            }
        }

        // Rests the timer again
        alarm(0);

      
      
        // Hints (put these into Google):
        // man fork
        // man execvp
        // man wait
        // man strtok
        // man environ
        // man signals
        
        // Extra Credit
        // man dup2
        // man open
        // man pipes
    }
    // This should never be reached.
    return -1;
}