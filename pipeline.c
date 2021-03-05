#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include "launcher.h"
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
extern int getArg(char* argv[], const char *, int *,int*);

void showUsageAndExit()
{
   fprintf(stderr, "Usage: pipeline path -p processes\nWhere: "
      "path is some \"random child\" and processes is 2 to 10"
      ", inclusive\n");
   exit(1);
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
void beforeFork(int oldFD[2], int newFD[2], int totalChild, int i) {
   oldFD[0] = newFD[0];
   oldFD[1] = newFD[1];
   newPipe(i,newFD,totalChild);
}
int forker(int pid) {
   if (-1 == (pid = fork()) ){
      perror(NULL);
      exit(EXIT_FAILURE);
   }
   return pid;
}
void childProcess(int oldFD[2], int newFD[2],  char *arg, int i, 
   int totalChild) {
   char write[3];
   char read[3];
   if(i==0) 
      firstChild(oldFD, newFD, read, write);
   
   else if(i==(totalChild-1)) 
      lastChild(oldFD, newFD, read, write);            
   
   else
      middleChild(oldFD, newFD, read, write); 
        
   execl(arg, arg,"-i",read,"-o",write,NULL);
   perror(NULL);
   exit(EXIT_FAILURE);

}
void parentProcess(int i, int totalChild, int oldFD[2], int newFD[2]) {
   if(i!=(totalChild-1)) 
      close(newFD[WRITE_END]);
   
   if(i!=0) 
      close(oldFD[READ_END]);
}

void mainHelper(int totalChild, char * arg) {
   int pid;
   int oldFD[2];
   int newFD[2];
   int waitstatus = 0;
   int i=0;
   for(i=0; i<(totalChild);i++){
      beforeFork(oldFD, newFD, totalChild, i);
      pid = forker(pid);
      if(pid==0)
         childProcess(oldFD, newFD, arg, i, totalChild);
      else 
         parentProcess(i, totalChild, oldFD, newFD);
   }
   for(i=0;i<totalChild;i++) 
      wait(&waitstatus); 
}

int main(int argc, char **argv) {
   int retVal;
   int total_child = DEFAULT_TOTAL_CHILD;
   /*int child_file=0; */
   int arg_position=-1;
   int argv_position=1; 
   retVal = getArg(argv, "-p", &total_child,&arg_position);
   if( arg_position == 1)
      argv_position=3; 
   
   if( (argc > ARG_MAX+1) || (retVal == -1) || (argc < 1) 
      || (total_child>TOTAL_CHILD_MAX) ||
      (total_child < TOTAL_CHILD_MIN) ) {
 
      showUsageAndExit();
   }
   
   mainHelper(total_child,argv[argv_position]);
   return 0;

}
