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

int sh( int argc, char **argv, char **envp )
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
//    char *report = which(*arguments[1]), pathelement);
//    printf("%s", report);
    which(*arguments[1]), pathelement);
   }

   else if (strcmp(*arguments, "where")==0) {
    //where(*arguments[1], pathelement);
   }

   else if (strcmp(*arguments, "prompt")==0) {
    if (**arguments[1]!=NULL)
     pref = prompt(*arguments[1]);
    else (**arguments[1]==NULL)
     pref = prompt();
   }
   else if (strcmp(*arguments, "pwd")) {
    pid = fork();
    if (pid) {
     waitpid(pid,NULL,0);
    }
    else {
     execve("/pwd", arguments);
     exit(-1);
    }
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

} /* which() */

char *where(char *command, struct pathelement *pathlist )
{
  /* similarly loop through finding all locations of command */
} /* where() */

void list ( char *dir )
{
  /* see man page for opendir() and readdir() and print out filenames for
  the directory passed */
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

void pwd(char* currwd) {
 printf("\n %s\n", currwd);
}
