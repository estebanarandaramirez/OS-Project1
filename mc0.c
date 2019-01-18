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

//Global Variables
long prevFaults = 0, prevReclaims = 0;

int main(int argc, char *argv[]) {
int counter = 0;
  while(1){
    int input;

    if(counter == 0){printf("===== Mid-Day Commander, v0 ===== \n");}
    printf("G’day, Commander! What command would you like to run? \n");
    printf("   %-10s %s", "0. whoami", ":");
    printf(" Prints out the result of the whoamicommand\n");
    printf("   %-10s %s", "1. last", ":");
    printf(" Prints out the result of the last command \n");
    printf("   %-10s %s", "2. ls", ":");
    printf(" Prints out the result of a listing on a user-specified path\n");
    printf("Option?: ");

    counter++;
    scanf("%d", &input);

    //error for invalid input
    if (input > 2 || input < 0){
      printf("Error! That is an invalid input. Please select one of the options. \n");
    }

    //codes to run based on input
    if(input == 0){
      whoAmI();
    }

    if(input == 1){
      printf("Option 1 \n");
      last();
    }

    if(input == 2){
      printf("Option 2 \n");
      ls();
    }
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

  //Print statistics
  printf("\n-- Statistics ---\n");
  printf("Elapsed Time: \n");
  printf("Page Faults: %li\n", faults);
  printf("Page Faults (reclaimed): %li\n", reclaims);
  printf("\n");
}

//run because option 0
void whoAmI(){
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
    statistics();
}

//run because option1
void last(){
  printf("\n-- Last Logins --\n");
  statistics();
}

//run because option2
void ls(){
  printf("\n-- Directory Listing --\n");
  statistics();
}
