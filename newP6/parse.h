#ifndef PARSE_H
#define PARSE_H
#define MAXCOMMANDS 20
typedef struct stage {
   char input[1024];
   char output[1024];
   char command[1024];
   char arg[10][1024];

}stage;
int parseStages(char *cline, struct stage stages[20], int *numCommands);
int parseItems(char items[200][1024], struct stage stages[MAXCOMMANDS], int *numCommands); 
/*int parseArgs(char *cString, char baseCMD[1024], char *args[10]);*/
void printError();















#endif
