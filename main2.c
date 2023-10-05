#include  <stdio.h>
#include  <sys/types.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define   MAX_COUNT  30

void  ChildProcess(void);                /* child process prototype  */
void  ParentProcess(void);               /* parent process prototype */

void  main(void)
{
     pid_t  pid;

     pid = fork();
     if (pid == 0) 
          ChildProcess();
     else 
          ParentProcess();

    return 0;
}

void  ChildProcess(void)
{
     int   i;
     srand(time(NULL));

     for (i = 1; i <= MAX_COUNT; i++){
      printf("Child Pid: %d is going to sleep!\n", getpid());
        int sleep_time = rand() % 10 + 1;
        sleep(sleep_time);
        printf("Child Pid: %d is awake!\nWhere is my Parent: %d?\n", getpid(), getppid());
     }
     printf("   *** Child process is done ***\n");
     exit(0); //Terminate the child process
}

void  ParentProcess(void)
{
     int   i;
     itn status;
     pid_t child_pid;

     for (i = 1; i <= 2 * MAX_COUNT; i++){
        printf("This line is from parent, va;ue = %d\n", i);
     }
     printf("*** Parent is done ***\n");

     //Waiting for Child processes to finish
     while ((child_pid = wait(&status)) > 0) {
      printf("Child Pid: %d has completed\n", child_pid);
     }
     
}
