
#include "get_path.h"

int pid;
int sh( int argc, char **argv, char **envp);
char *which(char *command, struct pathelement *pathlist);
char *where(char *command, struct pathelement *pathlist);
void list ( char *dir );
void printenv(char **envp);
void cd(char *destination);
void pwd(char* currwd);
char *promptnone(void);	// asks for new prompt prefix
char *prompt(char* prefix);	// accepts the new prefix as an arg and returns it? Had to overload.
void killit(int* pidtokill); // Kills a process at a certain process ID location
void killitnow(int* SIGNUM, int* pidtokill);// Same as above, but with a sigterm with it

#define PROMPTMAX 32
#define MAXARGS 10
