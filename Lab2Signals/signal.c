/* hello_signal.c */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int flag = 0;

void handler(int signum)
{
  //signal handler
  printf("Hello World!\n");
  flag = 1;
}

int main(int argc, char * argv[])
{ // Register handler to handle SIGALRM
  signal(SIGALRM, handler);
  while (1) {
    alarm(1); //Schedule a SIGALRM for 1 second

    //Busy wait for signal to be delivered
    while (1) {
      if (flag) {
        break;
      }
    }
    //Print "Turing was right!"
    printf("Turing was right!\n");

    //Reset flag
    flag = 0;
  }
  return 0;
}
