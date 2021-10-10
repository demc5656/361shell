//Dara McNally and Donovan Messer

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "sh.h"

#define BUFFERSIZE 128

int sh( int argc, char *argv[], char * envp[] )
{
  char *prompt = calloc(PROMPTMAX, sizeof(char));
  char *commandline = calloc(MAX_CANON, sizeof(char));
  char *command, *arg, *commandpath, *p, *pwd, *owd;
  char **args = calloc(MAXARGS, sizeof(char*));
  int uid, i, status, argsct, go = 1;
  struct passwd *password_entry;
  char *homedir;
  struct pathelement *pathlist;
  char *cwd;
  //char *pref = calloc(PROMPTMAX, sizeof(char));
  //char * agmt;
  //char **arguments = calloc(MAXARGS, sizeof(char*));
  //strcpy(pref, "");

  uid = getuid();
  password_entry = getpwuid(uid);               /* get passwd info */
  homedir = password_entry->pw_dir;		/* Home directory to start
						  out with*/

  if ( (pwd = getcwd(NULL, PATH_MAX+1)) == NULL )
  {
    perror("getcwd");
    exit(2);
  }
  owd = calloc(strlen(pwd) + 1, sizeof(char));
  memcpy(owd, pwd, strlen(pwd));
  prompt[0] = ' '; prompt[1] = '\0';

  /* Put PATH into a linked list */
  pathlist = get_path();

//  char *delim = (char*) malloc(sizeof(char));	//Delim???
//  delim = " ";
  while ( go )
  {
   getcwd(cwd, PATH_MAX+1);
    /* print your prompt */
	printf("\n%s [%s]> ", prompt, cwd);	//Prints the cwd in the prompt?
    /* get command line and process */
	char *cmdln;
	char *temp;
  char hold[BUFFERSIZE];
//	char **saveptr;
	//char **arguments; //????????? idk it says arguments are stored in a char**
	int len;
	char buffer[BUFFERSIZE];
	fgets(buffer, BUFFERSIZE, stdin);
	if (fgets(buffer, BUFFERSIZE , stdin) != NULL) {
		len =(int) strlen(buffer);
		buffer[len-1]='\0';
		strcpy(cmdln, buffer);
		temp = strtok(cmdln, " ");
		for (int i=0; temp!=NULL; i++) {
      //strcpy(hold, *arguments[i]);
			temp = strtok(NULL, " ");
			//arguments[i] = temp;
      args[i] = temp;
      args[i+1]=NULL;
      //strcpy(hold, temp);
		}
	}	//Got command line???
    /* check for each built in command and implement */
   if (strcmp(*args, "which")==0) {
    char *report = which(args[1], pathlist);
    printf("%s", report);
    //which(*arguments[1]), pathlist);
   }

   else if (strcmp(*args, "where")==0) {
    where(args[1], pathlist);
   }

   else if (strcmp(*args, "prompt")==0) {
    if (args[1]!=NULL) {
     prompt = promptwith(args[1]);
    }
    else {
     prompt = promptnone();
    }
   }
   else if (strcmp(*args, "pwd")==0) {
    pid = fork();
    if (pid) {
     waitpid(pid,NULL,0);
    }
    else {
     execve(which("pwd", pathlist), args, envp);
     exit(-1);
    }
   }
    else if (strcmp(*args, "pid")==0) {
      int procid = getpid();
      if (procid==0) {
        printf("Process ID: %d", pid);
      }
      printf("Process ID: %d", procid);
    }
    else if (strcmp(*args, "cd")==0) {
      pid = fork();
      if (pid) {
        waitpid(pid,NULL,0);
      }
      else {
        execve(which("cd", pathlist), args, envp);
        exit(-1);
      }
    }
    else if (strcmp(*args, "list")==0) {
      list(which(args[1], pathlist));
    }

    else if (strcmp(*args, "exit")==0) {
      go = 0;
      char *temp;
      for (int i=MAXARGS; i>=0; i--) {
        temp = args[i];
        free(temp);
      }
      //free(args);
      free(prompt);
      exit(0);
    }

    else if (strcmp(*args, "kill")==0){
      int SigNum, toKill;
      printf("Do you have a SIGNUM? (y/n)");
      if(fgets(buffer, BUFFERSIZE , stdin) == "n"){
        printf("Enter your PID");
        scanf("%d", &toKill);
        kill(2, toKill);
      }
      else{
        printf("Enter your SIGNUM");
        scanf("%d", &SigNum);
        SigNum= SigNum * -1;
        printf("Enter your PID");
        scanf("%d", &toKill);
        kill(SigNum, toKill);
      }
    }

    else if (strcmp(*args, "printenv")==0){ //prints whole environment
      int j;
      for(int j = 0; envp[j] != NULL; j++)
        printf("\n%s", envp[j]);
      getchar();
    }
    else if (strcmp(*args, "printenv")==1){
      printf("What do you want to search the value for?")
      char *tstring = fgets(buffer, BUFFERSIZE , stdin);
      getenv(tstring);
      free(tstring);
    }
    else if (strcmp(*args, "printenv")>=2){
      fprintf(stderr, "%s", "Invalid input.\n")
    }

    else if (strcmp(*args, "setenv")==0){ //prints whole environment
      int j;
      for(int j = 0; envp[j] != NULL; j++)
        printf("\n%s", envp[j]);
      getchar();
    }
    else if (strcmp(*args, "setenv")==1){
      const char *nme;
      printf("What do you want your environment to be called?")
      nme = fgets(buffer, BUFFERSIZE , stdin);
      setenvone(nme);
      free(nme);
    }
    else if (strcmp(*args, "setenv")==2){
      const char *nme, *vle;
      printf("What do you want your environment to be called?")
      nme = fgets(buffer, BUFFERSIZE , stdin);
      printf("What do you want its value to be?")
      vle = fgets(buffer, BUFFERSIZE , stdin);
      setenvtwo(nme,vle);
      free(nme);
      free(vle);
    }
    else if (strcmp(*args, "setenv")>2){
      fprintf(stderr, "%s", "Invalid input.\n")
    }

     /*  else  program to exec */
    else {
      char *prog = which(*args, pathlist);
      if (*prog) {
        pid = fork();
        if (pid) {
          waitpid(pid,NULL,0);
        }
      }
      else if (which("cd", pathlist)) {
        execve(which("cd", pathlist), args, envp);
        exit(-1);
      }
       /* find it */
	
       /* do fork(), execve() and waitpid() */
	
      else
        fprintf(stderr, "%s: Command not found.\n", args[0]);
    }
  }
  return 0;
} /* sh() */

char *which(char *command, struct pathelement *pathlist )
{
   /* loop through pathlist until finding command and return it.  Return
   NULL when not found. */
   char *comm = "";
   pathlist = get_path();
  while (pathlist) {         // WHICH
    sprintf(comm, "%s/gcc", pathlist->element);
    if (access(comm, X_OK) == 0) {
      //printf("[%s]\n", cmd);
      return comm;
      break;
    }
    pathlist = pathlist->next;
  }
  return NULL;

} /* which() */

char *where(char *command, struct pathelement *pathlist )
{
  /* similarly loop through finding all locations of command */
  char *comm = "";
  pathlist = get_path();
  while (pathlist) {         // WHERE
    sprintf(comm, "%s/gcc", pathlist->element);
    if (access(comm, F_OK) == 0)
      printf("[%s]\n", comm);
    pathlist = pathlist->next;
  }
  return comm;
} /* where() */

void list ( char *dir )
{
  /* see man page for opendir() and readdir() and print out filenames for
  the directory passed */
  DIR *currDir = opendir(dir);
  struct dirent *directent;
  for (directent=readdir(currDir); directent!=NULL; directent=readdir(currDir)) {
    printf("%s\n", directent->d_name);
  }
  closedir(currDir);
} /* list() */

char* promptnone(void) {
 char *prefix;
 char bufferp[BUFFERSIZE];
 int len;
 printf("What would you like your new prefix to be?> ");
 fgets(bufferp, BUFFERSIZE, stdin);
 if (fgets(bufferp, BUFFERSIZE , stdin) != NULL) {
  len =(int) strlen(bufferp);
  bufferp[len-1]='\0';
  strcpy(prefix, bufferp);
  return prefix;
 }
}

char* promptwith(char* prefix) {
	return prefix;
}

void killit(int* pidtokill){
  kill(2, *pidtokill);
}

void killitnow(int* SIGNUM, int* pidtokill){
  kill(*SIGNUM,*pidtokill);
}

int setenvone(const char *name){
   char* tempEnv = getenv(name);
   if(!tempEnv){
     tempEnv = NULL;
   }

   return 0;
}

int setenvtwo(const char *name, const char *value){
  char* tempEnv = getenv(name);
  if(!tempEnv){
    tempEnv = value;
   }

   return 0;
}

/*void pwd(char* currwd) {
 printf("\n %s\n", currwd);
}*/
