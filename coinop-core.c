/*
 * Core coin-op logic routines
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>

#include "coinop-config.h"
#include "coinop.h"

/******************************************************************************
 * Main - starts up the other threads, then waits for them to exit.
 ******************************************************************************/
int main (int argc, char **argv, char **envp) {
  pthread_t server, acceptor;
  int s;

  //Create the esrver and acceptor threads  
  s = pthread_create( &server, NULL, &server_thread_start, NULL);
  if (s!=0) { errno = s; perror("Server creation failed."); exit(EXIT_FAILURE); }
  s = pthread_create( &acceptor, NULL, &acceptor_thread_start, NULL);
  if (s!=0) { errno = s; perror("Accepter creation failed."); exit(EXIT_FAILURE); }

  
  //if either of the threads terminate, then exit
  while (1) {
    s = pthread_tryjoin_np( server, NULL);
    if (s != EBUSY) { 
      printf("Server thread exited.\n");
      exit(0);
    }
    s = pthread_tryjoin_np( acceptor, NULL);
    if (s != EBUSY) {
      printf("Acceptor thread exited.\n");
      exit (0);
    }
  }
}
