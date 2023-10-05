/* timer.c */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>


int flag = 0;
int alarm_count = 0;
time_t start_time;
int valid = 0;

void handler(int signum)
{
  //signal handler
  if (valid == 0) {
    printf("Hello World!\n");
    valid = 1;
  } else {
    //Print "Turing was right!"
    printf("Turing was right!\n");
    valid = 0;
  }
  alarm_count++;
  flag = 1;
}

void sigint_handler(int signum){
  time_t end_time;
  double execution_time;

  end_time = time(NULL);
  execution_time = difftime(end_time, start_time);

  printf("\nProgram executed for %.0lf seconds.\n", execution_time);
  printf("Alarms occurred: %d\n", alarm_count);
  exit(0);
}

int main(int argc, char * argv[])
{ //Register handler to handle SIGALRM
  signal(SIGALRM, handler);
  
  //Register handler to handle SIGINT (CTRL-C)
  signal(SIGINT, sigint_handler);

  start_time = time(NULL);

  while(1) {
    // Schedule a SIGALRM for 1 second
    alarm(1);
    // While waiting for signal to be delivered
    while (1) {
      if (flag) {
        break;
      }
    }
    
    // Reset flag
    flag = 0;
  }

  return 0;
  }
