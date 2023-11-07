#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
  // Declare the pipe file descriptors
  // Both variables store ends of the first and second pipe
  int pipefd1[2];
  int pipefd2[2];

  //Child processes IDs
  pid_t pid1;
  pid_t pid2;
  
  //Command arguments for cat, grep, and sort
  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", argv[1], NULL};
  char *sort_args[] = {"sort", NULL};

  // Check if the needed arguments is given and indicates error
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <pattern>\n", argv[0]);
    return 1;
  }

  // Creates the pipes and checks for potential errors
  if (pipe(pipefd1) == -1 || pipe(pipefd2) == -1) {
    perror("Pipe creation failed");
    return 1;
  }

  // Creates the first child process for 'cat' command
  pid1 = fork();


  if (pid1 == 0) {
    //Child Process that handles grep
    close(pipefd1[0]); // Closes read end of first pipe
    dup2(pipefd1[1], 1); // Redirects output to the write end of first pipe
    close(pipefd1[1]); // Closes write end of first pipe

    // Executes 'cat' command
    execvp("cat", cat_args);
  } else {
    // Create second child process for 'grep' and 'sort'
    pid2 = fork();

    if (pid2 == 0) {
      close(pipefd1[1]); // Close write end of first pipe
      dup2(pipefd1[0], 0); // Duplicates output to the read end of first pipe
      close(pipefd1[0]); // Close the read end of first pipe

      close(pipefd2[0]); // Close read end of second pipe
      dup2(pipefd2[1], 1); // Duplicate write end of file descriptor of second pipe
      close(pipefd2[1]); // Close write end of the second pipe

      execvp("grep", grep_args); // Execute grep command
    } else {
      // Parent Process
      // Close first pipe read and write end
      close(pipefd1[0]);
      close(pipefd1[1]); 

      // Close the write end, redirect standard output, then close read end of second pipe
      close(pipefd2[1]);
      dup2(pipefd2[0], 0);
      close(pipefd2[0]);

      // Execute 'sort' command
      execvp("sort", sort_args);
    }
  }
  // Wait for child processes to finish
  wait(NULL);
  wait(NULL);

  // Exit main function
  return 0;

}
