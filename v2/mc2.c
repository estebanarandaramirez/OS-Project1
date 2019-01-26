#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

void statistics();
void whoAmI();
void last();
void ls();
void a();
void c();
void e();
void p();
void r();
void makeCommand(int num, char *name, char *descrip);
void freeCommand();
void addedCommands(int commandAsInt);

//Constant
#define MAX 128 //Max characters

//Global Variables
long prevFaults = 0, prevReclaims = 0;
struct timeval start, stop;
struct command *head = NULL;
int commandNum = 3;

struct command {
  int comNum;
  char *comName;
  char *comDescrip;
  struct command *next;
};

// struct node {
//   struct command *runningC;
//   struct node *next;
// }

int main() {
  int counter = 0;
  makeCommand(0,"whoami", "Prints out the result of the whoamicommand");
  makeCommand(1,"last", "Prints out the result of the last command");
  makeCommand(2,"ls", "Prints out the result of a listing on a user-specified path");
  while(1){
    char input[MAX];

    if(counter == 0){printf("===== Mid-Day Commander, v1 ===== \n");}
    printf("G’day, Commander! What command would you like to run? \n");
    //pointers
    //new stuff
    struct command *commandTemp;
    commandTemp = (struct command*)malloc(sizeof(struct command));
    commandTemp = head;

    for (int i = 0; i < commandNum; i++){
      printf("   %d%s %-7s %s", commandTemp->comNum, ".", commandTemp->comName, ": ");
      printf("%s",commandTemp->comDescrip);
      printf("\n");
      commandTemp = commandTemp->next;
    }
    free(commandTemp);

    //old statements
    /*
    printf("   %-10s %s", "2. ls", ":");
    printf(" Prints out the result of a listing on a user-specified path\n");
    */
    printf("   a. add command : Adds a new command to the menu\n");
    printf("   c. change directory : Changes process working directory\n");
    printf("   e. exit : Leave Mid-Day Commander\n");
    printf("   p. pwd : Prints working directory\n");
    printf("   r. running processes : Print list of running processes\n");
    printf("Option?: ");

    int j = 1;
    while (j == 1) {
      int result = wait3(NULL, WNOHANG, NULL);
      if(result > 0){
        printf("A child has finished\n");
        statistics();
      } else {
        printf("Nothing\n");
        j++;
      }
    }

    fgets(input, MAX, stdin);
    counter++;
    char command = input[0];

    //error for invalid input
    // if (command != '0' /*|| command != '1' || command != '2' || command != 'a' || command != 'c' || command != 'e' || command != 'p'*/){
    //   printf("\nError! That is an invalid input. Please select one of the options. \n\n");
    // }
    int commandAsInt = command - '0';
    if((2 < commandAsInt) && (commandAsInt <= commandNum -1)){
      addedCommands(commandAsInt);
    }
    else{
      switch (command) {
        case '0':
          whoAmI();
          break;
        case '1':
          last();
          break;
        case '2':
          ls();
          break;
        case 'a':
          a();
          break;
        case 'c':
          c();
          break;
        case 'e':
          e();
          break;
        case 'p':
          p();
          break;
        case 'r':
          r();
          break;
        default:
          printf("\nError! That is an invalid input. Please select one of the options. \n\n");
      }
    }
  }
  //freeCommand();
}

void statistics(){
  struct rusage usage;
  long faults, reclaims, currentFaults, currentReclaims;

  //getrusage statistics
  getrusage(RUSAGE_CHILDREN, &usage);
  currentFaults = usage.ru_majflt;
  currentReclaims = usage.ru_minflt;
  //printf("%li %li\n", currentFaults, prevFaults);
  //printf("%li %li\n", currentReclaims, prevReclaims);
  if(prevFaults <= currentFaults){
    faults = currentFaults - prevFaults;
  } else {
    faults = currentFaults;
  }
  if(prevReclaims <= currentReclaims){
    reclaims = currentReclaims - prevReclaims;
  } else {
    reclaims = currentReclaims;
  }
  prevFaults += faults;
  prevReclaims += reclaims;

  //Calculate elapsed time in milliseconds
  float secs = (float)(stop.tv_sec - start.tv_sec) * 1000;
  float micros = (float)(stop.tv_usec - start.tv_usec) / 1000;
  float time = secs + micros; //time in milliseconds

  //Print statistics
  printf("\n-- Statistics ---\n");
  printf("Elapsed Time: %f %s\n", time, " milliseconds");
  printf("Page Faults: %li\n", faults);
  printf("Page Faults (reclaimed): %li\n", reclaims);
  printf("\n");
}

//run because option 0
void whoAmI(){
  gettimeofday(&start, NULL);
  printf("\n-- Who Am I? -- \n");
  int pid = fork();
  char* whoamiString = "whoami";
  char* list [2];
  list[0] = whoamiString;
  list[1] = NULL;

    if(pid != 0){
      wait(&pid);
    }
    else{
      execvp("whoami", list);
    }
  gettimeofday(&stop, NULL);
  statistics();
}

//run because option1
void last(){
  gettimeofday(&start, NULL);
  printf("\n-- Last Logins --\n");
  int pid = fork();
  char* lastString = "last";
  char* list [2];
  list[0] = lastString;
  list[1] = NULL;

  if(pid != 0){
    wait(&pid);
  }
  else{
    execvp("last", list);
  }
  gettimeofday(&stop, NULL);
  statistics();
}

//run because option2
void ls(){
  gettimeofday(&start, NULL);
  char argInput[570];
  char pathInput[540];
  char* lsString = "ls";
  char* list [4];
  list[0] = lsString;
  list[1] = NULL;
  list[2] = NULL;
  list[3] = NULL;
  printf("\n-- Directory Listing --\n");

  printf("Arguments?:");
  //getchar();
  fgets(argInput, 550, stdin);
  if(strcmp("\n",argInput) != 0){
    list[1] = argInput;
  }
  if(argInput[strlen(argInput) - 1] == '\n'){
    argInput[strlen(argInput) - 1] = '\0';
  }

  printf("Path?:");
  //lol dont know why i cant have it but it causes me so many fucking headaches: getchar();
  fgets(pathInput, 520, stdin);
  if(strcmp("\n",pathInput) != 0){
    list[2] = pathInput;
  }
  if(pathInput[strlen(pathInput) - 1] == '\n'){
    pathInput[strlen(pathInput) - 1] = '\0';
  }
  int pid = fork();

  if(pid != 0){
    wait(&pid);
  }
  else{
    execvp("ls", list);
  }
  gettimeofday(&stop, NULL);
  statistics();
}

//helper make command and add to list
void makeCommand(int num, char *name, char *descrip){
  struct command *commandNew;
  commandNew= (struct command*)malloc(sizeof(struct command));
  commandNew->comName = (char*)malloc(500*sizeof(char));
  commandNew->comDescrip = (char*)malloc(500*sizeof(char));

  commandNew->comNum = num;
  commandNew->comName = name;
  //printf("%s\n", name );
  //printf("%s\n", commandNew-> comName );
  commandNew->comDescrip = descrip;
  commandNew->next = NULL;


  if (head == NULL){
    head = commandNew;
  }
  else {
    struct command *commandTemp;
    commandTemp= (struct command*)malloc(sizeof(struct command));
    commandTemp = head;
    while (commandTemp->next != NULL){
      commandTemp = commandTemp->next;
    }
    commandTemp->next = commandNew;
    //free(commandTemp);
  }
}

//remove all commands from list
void freeCommand(){
  while(head->next != NULL){
    struct command *commandTemp;
    commandTemp= (struct command*)malloc(sizeof(struct command));
    commandTemp = head;
    head = commandTemp->next;
    free(commandTemp);
  }

  free(head);
}

//add command
void a(){
  //char commandInput[540];
  char *commandInput;
  commandInput= (char*)malloc(500*sizeof(char));

  printf("\n-- Add a command --\n");
  printf("Command to add?: ");

  fgets(commandInput, 520, stdin);
  strtok(commandInput, "\n");
  //commandInput[539] = '\0';

  //add command and increment number of commands
  makeCommand(commandNum,commandInput, "User added command");
  commandNum++;

  printf("Okay, added with ID %d!\n\n", commandNum-1);
}

//to run added command
void addedCommands(int commandAsInt){
  gettimeofday(&start, NULL);
  int backgroundCommand = 0;
  struct command *tempCommand;
  //tempCommand = (struct command*)malloc(sizeof(struct command));
  tempCommand = head;

  int j = 0;

  while(j < commandNum - (commandNum - commandAsInt)){
    tempCommand = tempCommand->next;
    j++;
  }

  printf("\n-- Command: %s --\n", tempCommand->comName);

  char *toBeEaten = (char*)malloc(500*sizeof(char));
  toBeEaten = strdup(tempCommand->comName);
  char *list [80];
  char *tokenizedString = (char*)malloc(500*sizeof(char));

  tokenizedString = strtok(toBeEaten, " ");
  list[0] = tokenizedString;
  int i = 1;

  while(tokenizedString != NULL){
    tokenizedString = strtok(NULL, " ");
    list[i] = tokenizedString;
    i++;
  }
  list[79] = NULL;

  char *lastString = list[i-2];
  char lastChar = lastString[0];
  //printf("%c\n", lastChar);
  if(lastChar == '&'){
    list[i-2] = NULL;
    backgroundCommand = 1;
    printf("[%d] %d\n\n", 1, tempCommand->comNum);
  }

  int status;
  int pid = fork();

  if(pid < 0){
    printf("There's an error.\n");
  }
  else if(pid == 0){
    execvp(list[0], list);
  }
  else{
    if(backgroundCommand == 0){
      pid = wait(&status);
    } else if(backgroundCommand == 1){
      gettimeofday(&stop, NULL);
      return;
    }
  }
  gettimeofday(&stop, NULL);
  statistics();

  //free(tempCommand);
}

//change directory
void c(){
  int wasSuccess = -1;
  char pathInput[540];

  // char* list [2];
  // list[0] = pathInput;
  // list[1] = NULL;

  printf("\n-- Change Directory --\n");
  printf("New Directory?: ");
  fgets(pathInput, 520, stdin);
  strtok(pathInput, "\n");
  printf("\n");

  wasSuccess = chdir(pathInput);

  if (wasSuccess == -1){
    printf("Error, was unable to change directories successfully. \n");
  }
}

//exits
void e(){
  printf("Logging you out, Commander.\n");
  exit(0);
}

//print directory
void p(){
  printf("\n-- Current Directory --\n");
  printf("Directory: ");
  char cwd[MAX];
  if(getcwd(cwd,sizeof(cwd)) != NULL){
    printf("%s\n\n", cwd);
  }
}

//list of running processes
void r(){
  printf("\n-- Background Processes --\n");
}
