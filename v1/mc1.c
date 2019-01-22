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

//Constant
#define MAX 100

//Global Variables
long prevFaults = 0, prevReclaims = 0;
struct timeval start, stop;

int main() {
  int counter = 0;
  while(1){
    char input[MAX];

    if(counter == 0){printf("===== Mid-Day Commander, v1 ===== \n");}
    printf("G’day, Commander! What command would you like to run? \n");
    printf("   %-10s %s", "0. whoami", ":");
    printf(" Prints out the result of the whoamicommand\n");
    printf("   %-10s %s", "1. last", ":");
    printf(" Prints out the result of the last command \n");
    printf("   %-10s %s", "2. ls", ":");
    printf(" Prints out the result of a listing on a user-specified path\n");
    printf("   a. add command : Adds a new command to the menu\n");
    printf("   c. change directory : Changes process working directory\n");
    printf("   e. exit : Leave Mid-Day Commander\n");
    printf("   p. pwd : Prints working directory\n");
    printf("Option?: ");

    fgets(input, MAX, stdin);
    counter++;

    char command = input[0];
    printf("%c\n", command);

    //error for invalid input
    if (command != '0' || command != '1' || command != '2' || command != 'a' || command != 'c' || command != 'e' || command != 'p'){
      printf("\nError! That is an invalid input. Please select one of the options. \n\n");
    }

    //codes to run based on input
    if(command == '0'){
      whoAmI();
    }

    if(command == '1'){
      last();
    }

    if(command == '2'){
      ls();
    }

    if(command == 'a'){
      a();
    }

    if(command == 'c'){
      c();
    }

    if(command == 'e'){
      e();
    }

    if(command == 'p'){
      p();
    }

    //return(0);
  }
}

void statistics(){
  struct rusage usage;
  long faults, reclaims;

  //getrusage statistics
  getrusage(RUSAGE_CHILDREN, &usage);
  faults = usage.ru_majflt - prevFaults;
  reclaims = usage.ru_minflt - prevReclaims;
  prevFaults = usage.ru_majflt;
  prevReclaims = usage.ru_minflt;

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
  getchar();
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

//add command
void a(){
  gettimeofday(&start, NULL);
  printf("\n-- Add a command --\n");
  printf("\nCommand to add?:");

  printf("\nOkay, added with ID __!\n");

  gettimeofday(&stop, NULL);
  statistics();
}

//change directory
void c(){
  gettimeofday(&start, NULL);

  int wasSuccess = -1;
  char pathInput[540];

  char* list [2];
  list[0] = pathInput;
  list[1] = NULL;

  printf("\n-- Change Directory --\n");
  printf("\nNew Directory?: ");
  fgets(pathInput, 520, stdin);

  wasSuccess = chdir(pathInput);

  if (wasSuccess == -1){
    printf("Error, was unable to change directories successfully. \n");
  }

  gettimeofday(&stop, NULL);
  statistics();
}

//exits
void e(){
  gettimeofday(&start, NULL);
  printf("\n  Logging you out, Commander.");

  gettimeofday(&stop, NULL);

  statistics();
}

//print directory
void p(){
  gettimeofday(&start, NULL);
  printf("\n-- Current Directory --\n");
  printf("\nDirectory: __");

  gettimeofday(&stop, NULL);
  statistics();
}
