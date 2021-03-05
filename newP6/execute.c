#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include "parse.h"
#define TOTAL_CHILD_MAX 10
#define ARG_MAX 3
#define TOTAL_CHILD_MIN 2
#define DEFAULT_TOTAL_CHILD 2
#define WRITE_END       1
#define READ_END        0
#define FIRST_PIPE      0
#define SECOND_PIPE     1
#define NO_PIPE         -1
#define TOTAL_PIPES     2


/*void newPipe(int i,int newFD[2], int child_max)
{
   int retVal;
   if(i!= (child_max-1)) {
      retVal = pipe(newFD);
      if(retVal == -1) {
         perror(NULL);
         exit(EXIT_FAILURE);
      }
   }   
}
void firstChild(int oldFD[2], int newFD[2], char read[3], char write[3]) {
   sprintf(read, "%d", 0);
   sprintf(write, "%d", newFD[WRITE_END]);
   close(newFD[READ_END]);
}
void lastChild(int oldFD[2], int newFD[2], char read[3], char write[3]) {
   sprintf(write, "%d", 1);
   sprintf(read, "%d", oldFD[READ_END]);
}
void middleChild(int oldFD[2], int newFD[2], char read[3], char write[3]) {
   sprintf(read, "%d", oldFD[READ_END]);
   sprintf(write, "%d", newFD[WRITE_END]);
   close(newFD[READ_END]);
}
void beforeFork(int oldFD[2], int newFD[2], int  numCommands, int i) {
  }
int forker(int pid) {
   if (-1 == (pid = fork()) ){
      perror(NULL);
      exit(EXIT_FAILURE);
   }
   return pid;
}
void childProcess(int oldFD[2], int newFD[2],  struct stage stages[20], int i, 
   int numCommands) {
   char write[3];
   char read[3];
   if(i==0) 
      firstChild(oldFD, newFD, read, write);
   
   else if(i==(numCommands-1)) 
      lastChild(oldFD, newFD, read, write);            
   
   else
      middleChild(oldFD, newFD, read, write); 
        
   execv(command.command, command.arg);
   perror(NULL);
   exit(EXIT_FAILURE);

}*/
void parentProcess(int newFD[2], int oldFD[2], int i, int numCommands) {
     if(i!=(numCommands-1)) 
      close(newFD[WRITE_END]);
   
   if(i!=0) 
      close(oldFD[READ_END]);
}
void firstChild(int oldFD[2], int newFD[2], struct stage stages[20], int i) {
   newFD[READ_END] = STDIN_FILENO;

}

void childProcess(struct stage stages[20], int numCommands, int i, int oldFD[2], int newFD[2]) {
   if(i==0 && (numCommands != 1)) { 
      dup2(pipefd[1], 1);
   }
   else if (i>0 && i < (numCommands - 1)) {
      dup2(
   }
      firstChild(oldFD, newFD, stages, i);
   
   else if(i==(numCommands-1)) 
      lastChild(oldFD, newFD, stages, i);            
   
   else
      middleChild(oldFD, newFD, stages, i); */

   if((strcmp(stages[i].input, "") != 0)) {
      newFD[READ_END] = openFile(stages[i].input, "r");
   }
   if((strcmp(stages[i].output, "") != 0)) {
      newFD[WRITE_END] = openFIle(stages[i].output, "w");
   }
}

void newPipe(int i,int newFD[2], int child_max)
{
   int retVal;
   if(i!= (child_max-1)) {
      retVal = pipe(newFD);
      if(retVal == -1) {
         perror(NULL);
         exit(EXIT_FAILURE);
      }
   }   
}
int forker(int pid) {
   if (-1 == (pid = fork()) ){
      perror(NULL);
      exit(EXIT_FAILURE);
   }
   return pid;
}

void runCommand(struct stage stages[20], int i, int numCommands, int *pid) {
   int pipefd[2], inputFd, outputFd;
   newPipe(i, pipefd, numCommands);
   *pid = forker(*pid);
   if(*pid == 0) {
      childProcess(stages,numCommands, i, pipefd);
   }
}
void run(struct stage stages[20],  int  numCommands) {
   int pid;
   int oldFD[2];
   int newFD[2];
   int waitstatus = 0;
   int i=0;
   while(i< (numCommands)) {
   
      
      if(pid==0) {
         oldFD[0] = newFD[0];
         oldFD[1] = newFD[1];
         newPipe(i,newFD, numCommands);
         childProcess(oldFD, newFD, stages[i], i, numCommands);
      }
      else 
         parentProcess(i, numCommands, oldFD, newFD);
   i++;
   }
   for(i=0;i<numCommands;i++) 
      wait(&waitstatus); 
   
}
