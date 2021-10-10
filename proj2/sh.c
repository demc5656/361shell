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
  char *pref;

  *pref = "";

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
   getcwd(cwd);
    /* print your prompt */
	printf("\n%s [%s]> ", pref, cwd);	//Prints the cwd in the prompt?
    /* get command line and process */
	char *cmdln;
	char *temp;
//	char **saveptr;
	char **arguments; //????????? idk it says arguments are stored in a char**
	int len;
	char buffer[BUFFERSIZE];
	fgets(buffer, BUFFERSIZE, stdin);
	if (fgets(buffer, BUFFERSIZE , stdin) != NULL) {
		len =(int) strlen(buffer);
		buffer[len-1]='\0';
		strcopy(cmdln, buffer);
		temp = strtok(cmdln, " ");
		for (int i=0; temp!=NULL; i++) {
			temp = strtok(NULL, " ");
			*arguments[i] = temp;
		}
	}	//Got command line???
    /* check for each built in command and implement */
   if (strcmp(*arguments, "which")==0) {
    char *report = which(*arguments[1]), pathlist);
    printf("%s", report);
    //which(*arguments[1]), pathlist);
   }

   else if (strcmp(*arguments, "where")==0) {
    where(*arguments[1], pathlist);
   }

   else if (strcmp(*arguments, "prompt")==0) {
    if (**arguments[1]!=NULL)
     pref = prompt(*arguments[1]);
    else (**arguments[1]==NULL)
     pref = prompt();
   }
   else if (strcmp(*arguments, "pwd")==0) {
    pid = fork();
    if (pid) {
     waitpid(pid,NULL,0);
    }
    else {
     execve(which("pwd", pathlist), arguments);
     exit(-1);
    }
   }
    else if (strcmp(*arguments, "pid")==0) {
      int procid = getpid();
      if (procid==0) {
        printf("Process ID: %d", pid);
      }
      printf("Process ID: %d", procid);
    }
    else if (strcmp(*arguments, "cd")==0) {
      pid = fork();
      if (pid) {
        waitpid(pid,NULL,0);
      }
      else {
        execve(which("cd", pathlist), arguments);
        exit(-1);
      }
    }
    else if (strcmp(*arguments, "list")==0) {
      list(which(*arguments[1], pathlist));
    }

    else if (strcmp(*arguments, "exit")==0) {
      go = FALSE;
      exit();
    }

    else if (strcmp(*arguments, "kill")==0){
      int SigNum, toKill;
      printf("Do you have a SIGNUM? (y/n)")
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

    else if (strcmp(*arguments, "printenv")==0){ //prints whole environment
      int j;
      for(int j = 0; envp[j] != NULL; j++)
        printf("\n%s", envp[j])
      getchar();
    }
    else if (strcmp(*arguments, "printenv")==1){
       getenv(3);
    }
    else if (strcmp(*arguments, "printenv")>=2){
      //print error message
    }

    else if (strcmp(*arguments, "setenv")==0){ //prints whole environment
      int j;
      for(int j = 0; envp[j] != NULL; j++)
        printf("\n%s", envp[j])
      getchar();
    }
    else if (strcmp(*arguments, "setenv")==1){
      int holderenvir = setenv(3);
      return holderenvir;
    }
    else if (strcmp(*arguments, "setenv")==2){
      //the second one is the value of the first??
    }
    else if (strcmp(*arguments, "setenv")>2){
      //print stderr message
    }

     /*  else  program to exec */
    {
       /* find it */
	
       /* do fork(), execve() and waitpid() */
	
      /* else */
        /* fprintf(stderr, "%s: Command not found.\n", args[0]); */
    }
  }
  return 0;
} /* sh() */

char *which(char *command, struct pathelement *pathlist )
{
   /* loop through pathlist until finding command and return it.  Return
   NULL when not found. */
   pathlist = get_path();
  while (pathlist) {         // WHICH
    sprintf(cmd, "%s/gcc", pathlist->element);
    if (access(cmd, X_OK) == 0) {
      //printf("[%s]\n", cmd);
      return cmd;
      break;
    }
    pathlist = pathlist->next;
  }

} /* which() */

char *where(char *command, struct pathelement *pathlist )
{
  /* similarly loop through finding all locations of command */
  pathlist = get_path();
  while (pathlist) {         // WHERE
    sprintf(cmd, "%s/gcc", pathlist->element);
    if (access(cmd, F_OK) == 0)
      printf("[%s]\n", cmd);
    pathlist = pathlist->next;
  }
  return cmd;
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
  closedir(dir);
} /* list() */

char* prompt() {
 char *prefix;
 char bufferp[BUFFERSIZE];
 int len;
 printf("What would you like your new prefix to be?> ");
 fgets(bufferp, BUFFERSIZE, stdin);
 if (fgets(bufferp, BUFFERSIZE , stdin) != NULL) {
  len =(int) strlen(bufferp);
  bufferp[len-1]='\0';
  strcopy(prefix, bufferp);
  return prefix;
 }
}

char* prompt(char* prefix) {
	return prefix;
}

void kill(int* pidtokill){
  kill(2, pidtokill);
}

void kill(int* SIGNUM, int* pidtokill){
  kill(SIGNUM,pidtokill);
}

/*void pwd(char* currwd) {
 printf("\n %s\n", currwd);
}*/
