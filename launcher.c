#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "launcher.h"
extern void showUsageAndExit(void);

int simple(int* argread, int i) {
   *argread = i;
   return 0;
}
int getArg(char *argv[], const char *arg, int *value,int* argread)
{
   int i;
   for (i = 1; argv[i+1]!=NULL; i+=1) {
      if (0 == strcmp(argv[i], arg)) {
         if (1 != sscanf(argv[i+1], "%d", value))
            break;
         return simple(argread, i);
         
      }
   }
   return -1;
}
