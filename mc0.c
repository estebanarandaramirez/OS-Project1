#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>


void statistics();
void whoAmI();
void last();
void ls();

int main(int argc, char *argv[]) {
  int input;

  //initial menu
  printf("===== Mid-Day Commander, v0 ===== \n");
  printf("G’day, Commander! What command would you like to run? \n");
  printf("0. whoami : Prints out the result of the whoamicommand\n");
  printf("1. last : Prints out the result of the last command \n");
  printf("2. ls : Prints out the result of a listing on a user-specified path)\n");
  printf("Option?: ");

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

void statistics(){
  printf("\n-- Statistics ---\n");
  printf("Elapsed Time: \n");
  printf("Page Faults: \n");
  printf("Page Faults (reclaimed): \n");
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
}

//run because option2
void ls(){
  printf("\n-- Directory Listing --\n");
}
