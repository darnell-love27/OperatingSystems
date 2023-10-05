#include  <stdio.h>
#include  <string.h>
#include  <sys/types.h>
#include  <unistd.h>
#include  <stdlib.h>
#include  <time.h>

#define   MAX_COUNT  30
#define   BUF_SIZE   100

void ChildProcess(void) {
  pid_t pid = getpid();
  int i;
  int loop_count;
  char buf[BUF_SIZE];

  pid = getpid();

  // Generate a random number of iterations
  loop_count = (rand() % MAX_COUNT) + 1;

  for (i=1; i <= loop_count; i++) {
    //Print a message that the child process is going to sleep
    sprintf(buf, "Child Pid: %d is going to sleep!", pid);
    write(1, buf, strlen(buf));

    // Sleep for a random amount of time, but no more than 10 seconds
    sleep((rand() % 10) + 1);

    // Print a message that the child process is awake and get the parent's Pid
    sprintf(buf, "Child Pid: %d is awake!\nWhere is my Parent: %d?\n", pid, getppid());
    write(1, buf, strlen(buf));
  }

  // Exit the child process
  exit(0);
}

void ParentProcess(void) {
  int status;
  pid_t child_pid;

  // Wait for both child processes to complete
  while ((child_pid = wait(&status)) != -1) {
    printf("Child Pid: %d has completed\n", child_pid);
  }
}



int  main(void)
{    
    pid_t pid1;
    pid_t pid2;
  //Create the first child process
    pid1 = fork();

    if (pid1 == 0) {
        // This code is executed by the first child
        ChildProcess();
    } else if (pid1 > 0) {
        // This code is executed by the parent
        // Create the second child process
        pid2 = fork();

        if (pid2 == 0) {
            // This code is executed by the second child
            ChildProcess();
        } else if (pid2 > 0) {
            // This code is executed by the parent
            printf("Parent is running with PID: %d\n", getpid());

            // Wait for both child processes to complete
            wait(NULL);
            wait(NULL);
        } else {
            perror("Error creating second child");
        }
    } else {
        perror("Error creating first child");
    }

    return 0;
}
