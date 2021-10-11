//Dara McNally and Donovan Messer
/*
I don't even know what's going wrong. It seg faults the instant
I try to type something. Never hits the print step 1 line. The only
thing that seems to work is ignoring ctrl+c when there's nothing to halt
and that it shows the directory you're in.
I tried emailing about this and got no response. The lines that keep seg
faulting are lines I pulled directly off my perfectly working proj_1a.
I do not know what is going wrong and I don't know how to fix it and I've
spent 6 hours trying to figure it out to no avail.
*/

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
  char *prompt = malloc(PROMPTMAX*sizeof(char));
  char *commandline = malloc(MAX_CANON*sizeof(char));
  char *command, *arg, *commandpath, *p, *pwd, *owd;
  char **args = malloc(MAXARGS*sizeof(char*));
  /*
  for (int i=0; i<MAXARGS; i++) {
    args[i] = NULL;
  }
  */
  //args[MAXARGS-1] = NULL;
  int uid, i, status, argsct, go = 1;
  struct passwd *password_entry;
  char *homedir;
  struct pathelement *pathlist;
  char *cwd = NULL;
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
   cwd = getcwd(cwd, PATH_MAX+1);
    /* print your prompt */
	printf("\n%s [%s]> ", prompt, cwd);	//Prints the cwd in the prompt?
    /* get command line and process */
	//char *cmdln;
	char *temp;
  //char hold[BUFFERSIZE];
//	char **saveptr;
	//char **arguments; //????????? idk it says arguments are stored in a char**
	int len;
	char buffer[BUFFERSIZE];
  //arg=fgets(commandline, MAX_CANON, stdin);
  //fgets(commandline, MAX_CANON, stdin);
  if (fgets(buffer, BUFFERSIZE, stdin)!=NULL) {
    len =(int) strlen(buffer);
    buffer[len-1]='\0';
    strcpy(commandline, buffer);
    strcpy(arg, buffer);
  }
  printf("step1");
	//fgets(buffer, BUFFERSIZE, stdin);
  //fgets(buffer, BUFFERSIZE, stdin);
  /*
  while (arg==NULL && strcmp(arg, "\0")!=0) {
    //free(arg);
    arg=fgets(commandline, BUFFERSIZE , stdin);
    len =(int) strlen(commandline);
		commandline[len-1]='\0';
		strcpy(arg, commandline);
  }
  int lent = strlen(arg);
  if (lent>1) {
    arg[lent-1] = '\0';
  }
  */
  
	if (arg!=NULL) {
		//len =(int) strlen(commandline);
		//commandline[len-1]='\0';
		//strcpy(arg, commandline);
		temp = strtok(commandline, " ");
		for (int i=0; temp!=NULL; i++) {
      //strcpy(hold, *arguments[i]);
			
			//arguments[i] = temp;
      args[i] = temp;
      args[i+1]=NULL;
      temp = strtok(NULL, " ");
      //strcpy(hold, temp);
		}
	}	//Got command line???
    /* check for each built in command and implement */
   if (args[1]!=NULL && args[1]!="") {
   if (strcmp(*args, "which")==0) {
     printf("which");
    char *report = which(args[1], pathlist);
    printf("%s", report);
    //which(*arguments[1]), pathlist);
   }

   else if (strcmp(*args, "where")==0) {
     printf("where");
    where(args[1], pathlist);
   }

   else if (strcmp(*args, "prompt")==0) {
     printf("prompt");
    if (args[1]!=NULL) {
     prompt = promptwith(args[1]);
    }
    else {
     printf("prompt2");
     prompt = promptnone();
    }
   }
   else if (strcmp(*args, "pwd")==0) {
     printf("pwd");
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
      printf("pid");
      int procid = getpid();
      if (procid==0) {
        printf("Process ID: %d", pid);
      }
      printf("Process ID: %d", procid);
    }
    else if (strcmp(*args, "cd")==0) {
      printf("cd");
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
      printf("list");
      list(which(args[1], pathlist));
    }

    else if (strcmp(*args, "exit")==0) {
      printf("exit");
      go = 0;
      char *temp;
      
      for (int i=0; i<MAXARGS; i++) {
        temp = args[i];
        free(temp);
      }
      //free(args);
      //free(path);
      free(commandline);
      free(prompt);
      free(owd);
      struct pathelement *tp = pathlist;
      for (; tp; tp = pathlist->next) {
        pathlist = pathlist->next;
        free(tp);
      }
      exit(0);
    }

    else if (strcmp(*args, "kill")==0){
      printf("kill");
      int SigNum, toKill;
      printf("Do you have a SIGNUM? (y/n)");
      arg = fgets(commandline, MAX_CANON , stdin);
      if(arg == "n"){
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
    printf("printenv");
      int j;
      for(int j = 0; envp[j] != NULL; j++)
        printf("\n%s", envp[j]);
      getchar();
    }
    else if (strcmp(*args, "printenv")==1){
      printf("penv1");
      printf("What do you want to search the value for?");
      char *tstring = fgets(commandline, MAX_CANON , stdin);
      getenv(tstring);
      free(tstring);
    }
    else if (strcmp(*args, "printenv")>=2){
      printf("penv2");
      fprintf(stderr, "%s", "Invalid input.\n");
    }

    else if (strcmp(*args, "setenv")==0){ //prints whole environment
    printf("senv");
      int j;
      for(int j = 0; envp[j] != NULL; j++)
        printf("\n%s", envp[j]);
      getchar();
    }
    else if (strcmp(*args, "setenv")==1){
      printf("senv1");
      char *nme;
      //const char *nme;
      printf("What do you want your environment to be called?");
      nme = fgets(commandline, MAX_CANON , stdin);
      setenvone(nme);
      free(nme);
    }
    else if (strcmp(*args, "setenv")==2){
      printf("senv2");
      char *nme, *vle;
      //const char *nme, *vle;
      printf("What do you want your environment to be called?");
      nme = fgets(commandline, MAX_CANON , stdin);
      printf("What do you want its value to be?");
      vle = fgets(commandline, MAX_CANON , stdin);
      setenvtwo(nme,vle);
      free(nme);
      free(vle);
    }
    else if (strcmp(*args, "setenv")>2){
      printf("senv3");
      fprintf(stderr, "%s", "Invalid input.\n");
    }
   }

     /*  else  program to exec */
    else {
      printf("why are oyu here");
      char *prog = which(*args, pathlist);
      if (*prog) {
        pid = fork();
        if (pid) {
          waitpid(pid,NULL,0);
        }
      }
      else if (which("cd", pathlist)) {
        printf("cd??");
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

int setenvone(char *name){
   char* tempEnv = getenv(name);
   if(!tempEnv){
     tempEnv = NULL;
   }

   return 0;
}

int setenvtwo(char *name, char *value){
  char* tempEnv = getenv(name);
  if(!tempEnv){
    tempEnv = value;
   }

   return 0;
}

/*void pwd(char* currwd) {
 printf("\n %s\n", currwd);
}*/
