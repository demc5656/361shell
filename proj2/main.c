//Dara McNally and Donovan Messer
#include "sh.h"
#include <signal.h>
#include <stdio.h>

void sig_handler(int signal); 

int main( int argc, char **argv, char **envp )
{
  /* put signal set up stuff here */
  
  signal(SIGINT, sig_handler);
  signal(SIGTSTP, sig_handler);
  signal(SIGTERM, sig_handler);
  return sh(argc, argv, envp);
}

void sig_handler(int signal)
{
  /* define your signal handler */
}

