#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void  ClientProcess(int []);

int  main(int  argc, char *argv[])
{
     //Initialize Variables
     int ShmID; //Shared memory ID
     int *ShmPTR = NULL; //Pointer for shared memory ID
     pid_t pid; //Process ID
     int status;
     int turn = 0;
     int Bank_Account = 0;

     //Creating shared memory for bank account and turn Variables
     ShmID = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0666);

    //Adds the shared memory from Pointer
     ShmPTR = (int *) shmat(ShmID, NULL, 0);
     if (*ShmPTR == -1) {
      //Print Error
          printf("*** shmat error (server) ***\n");
          exit(1);
     }
     printf("Server has attached the shared memory...\n");

    // Initialize bank account and turn variables as '0'
     ShmPTR[0] = Bank_Account;
     ShmPTR[1] = turn;

    //Checks for error in accessing shared memory ID
     if (ShmID < 0) {
          printf("*** shmget error (server) ***\n");
          exit(1);
     }
     printf("Server received shared memory for bank account and turn...\n");

     printf("Server initialized the bank account and turn to 0...\n");

     if (argc != 5) {
          printf("Use: %s #1 #2 #3 #4\n", argv[0]);
          exit(1);
     }

     ShmID = shmget(IPC_PRIVATE, 4*sizeof(int), IPC_CREAT | 0666);
     if (ShmID < 0) {
          printf("*** shmget error (server) ***\n");
          exit(1);
     }
     printf("Server has received a shared memory of four integers...\n");
     
     //Stored shared memory with respective command-line arguments
     ShmPTR[0] = atoi(argv[1]);
     ShmPTR[1] = atoi(argv[2]);
     ShmPTR[2] = atoi(argv[3]);
     ShmPTR[3] = atoi(argv[4]);
     printf("Server has filled %d %d %d %d in shared memory...\n",
            ShmPTR[0], ShmPTR[1], ShmPTR[2], ShmPTR[3]);

     printf("Server is about to fork a child process...\n");

     // Stores shared memory with line arguments
     pid = fork();

     if (pid < 0) {
          printf("*** fork error (server) ***\n");
          exit(1);
     }
     else if (pid == 0) {
          srand(time(0));
          for (int x = 0; x < 25; x++){
               sleep(rand() % 6);
               while (ShmPTR[1] != 1) {

               }
               int account = ShmPTR[0];
               int balance_need = rand() % 51;
               printf("Poor Student needs $%d\n", balance_need);
               if (balance_need <= account) {
                ShmPTR[0] -= balance_need;
                printf("Poor Student: Withdraws $%d / Balance = $%d\n", balance_need, ShmPTR[0]);
               } else {
                printf("Poor Student: Not Enough Cash ($%d)\n", account);
               }
               ShmPTR[1] = 0;
               
          }
          exit(0);
     } else {
        srand(time(0));
        for (int i = 0; i < 25; i++){
          sleep(rand() % 6);
          while (ShmPTR[1] != 0) {
            //While Operation runs...
          }
          int account = ShmPTR[0];
          if (account <= 100){
                    int balance = rand() % 101;
                    if (balance % 2 == 0) {
                         ShmPTR[0] += balance;
                         printf("Dear Old Dad: Desposits $%d / Balance = $%d\n", balance, ShmPTR[0]);
                    } else {
                         printf("Dear Old Dad: Doesn't have any money to give\n");
                    }
               } else {
                    printf("Dear Old Dad: Thinks Student has enough Cash ($%d)\n", Bank_Account);
               }
               ShmPTR[1] = 1;
        }
     }
    // Checks for errors when forking child process 
     wait(&status);
     printf("Server has detected the completion of its child...\n");
     shmdt((void *) ShmPTR);
     printf("Server has detached its shared memory...\n");
     shmctl(ShmID, IPC_RMID, NULL);
     printf("Server has removed its shared memory...\n");
     printf("Server exits...\n");
     exit(0);
}

// Waits for child process to finish -> Server exits
void  ClientProcess(int  SharedMem[])
{
     printf("   Client process started\n");
     printf("   Client found %d %d %d %d in shared memory\n",
                SharedMem[0], SharedMem[1], SharedMem[2], SharedMem[3]);
     printf("   Client is about to exit\n");
}
