#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "parse.h"
#include "execute.h"

#define PROMPTER ":-) "
#define BUFSIZE 1024

int pid, status;
char buffer[1024];
struct stage stages[20];
int numCommands = 0;

void printPrompt(char *prompt) {
   printf("%s", prompt);

}

void loop() {
   int len;
   int status;
   setbuf(stdout, NULL);
   while(1) {
      printPrompt(PROMPTER);
      if (!fgets(buffer, BUFSIZE, stdin))
         break;
      if(strcmp(buffer, "\n")==0)
         continue; 
      len = strlen(buffer);
      buffer[len-1] = '\0';
      if(strcmp(buffer, "exit") == 0)
         break;
      if((parseStages(buffer, stages, &numCommands)) < 0) {
         continue;
      }
      run(stages, numCommands);
      printf("here\n");
      memset(stages, 0, sizeof(stage)*20);
      numCommands = 0;
      waitpid(pid, &status, 0);
   }
}




int main(int argc, char * argv[]) {
   setbuf(stdout, NULL);
   loop();
   return 0;


}


