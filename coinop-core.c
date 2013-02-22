/*
 * Core coin-op logic routines
 */

#include <stdlib.h>
#include <pthread.h>

#include "coinop-config.h"
#include "coinop.h"

/******************************************************************************
 * Main - starts up the other threads, then waits for them to exit.
 ******************************************************************************/
int main (int argc, char **argv, char **envp) {
  pthread_t server, acceptor;
  
  //TODO: deal with errors on thread creation
  pthread_create( &server, NULL, &server_thread_start, NULL);
  pthread_create( &acceptor, NULL, &acceptor_thread_start, NULL);

  pthread_join( server, NULL);
  pthread_join( acceptor, NULL);

}
