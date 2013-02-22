/*
 * Core coin-op logic routines
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include "coinop-config.h"
#include "coinop.h"


/******************************************************************************
 * Static global vars.
 ******************************************************************************/
pthread_mutex_t seconds_mutex = PTHREAD_MUTEX_INITIALIZER;
unsigned int seconds_remaining = 0;

/******************************************************************************
 * Forward Declarations
 ******************************************************************************/
//routine that starts the private thread that manages the clock
void *clock_thread_start(void *arg);


/******************************************************************************
 * Main - starts up the other threads, then waits for them to exit.
 ******************************************************************************/
int main (int argc, char **argv, char **envp) {
  pthread_t server, acceptor, clock;
   int s;

  //Create the esrver and acceptor threads  
  s = pthread_create( &server, NULL, &server_thread_start, NULL);
  if (s!=0) { errno = s; perror("Server creation failed."); exit(EXIT_FAILURE); }
  s = pthread_create( &acceptor, NULL, &acceptor_thread_start, NULL);
  if (s!=0) { errno = s; perror("Accepter creation failed."); exit(EXIT_FAILURE); }
  s = pthread_create( &clock, NULL, &clock_thread_start, NULL);
  if (s!=0) { errno = s; perror("Clock creation failed."); exit(EXIT_FAILURE); }
  
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
    s = pthread_tryjoin_np( clock, NULL);
    if (s != EBUSY) {
      printf("Clock thread exited.\n");
      exit(0);
    }
  }
}

void set_time(unsigned int seconds) {
  printf("set_time %d..", seconds);
  pthread_mutex_lock( &seconds_mutex );
  seconds_remaining = seconds;
  pthread_mutex_unlock( &seconds_mutex );
  printf("done\n");
}

void add_time(unsigned int seconds) {
  printf("add_time %d..", seconds);
  pthread_mutex_lock( &seconds_mutex );
  seconds_remaining+=seconds; //BUGBUG: need to check for overflow.
  pthread_mutex_unlock( &seconds_mutex );
  printf("done\n");
}

int get_time_remaining() {
  return seconds_remaining;
}

void tick() {
  pthread_mutex_lock( &seconds_mutex );
  if (seconds_remaining > 0) {
    seconds_remaining--;
  }
  pthread_mutex_unlock( &seconds_mutex );
  printf("tick... %d\n", seconds_remaining);
}

void *clock_thread_start(void *arg) {
  printf("Clock Started.\n");
  while(1) {
    usleep(1000000); //sleep 1 sec.
    tick();
  }
}
