/*
 * Core coin-op logic routines
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <syslog.h>

#include "coinop-config.h"
#include "coinop.h"


/******************************************************************************
 * Static global vars.
 ******************************************************************************/
//seconds_remaining is used on multiple threads; modification must
//be protected by this mutex.
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

  //setup the logmask for debug
#if !DEBUG
  setlogmask(setlogmask(0) & ~LOG_MASK(LOG_INFO));
#endif

  syslog(LOG_NOTICE, "CoinOp started");

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



/******************************************************************************
 * set_time - sets the remaining play time to the specified value.
 ******************************************************************************/
void set_time(unsigned int seconds) {
  syslog(LOG_INFO, "set_time: %d seconds", seconds);
  pthread_mutex_lock( &seconds_mutex );
  seconds_remaining = seconds;
  pthread_mutex_unlock( &seconds_mutex );
}

/******************************************************************************
 * add_time - adds the specified number of second to the remaining time.
 ******************************************************************************/
void add_time(unsigned int seconds) {
  syslog(LOG_INFO, "add_time: %d seconds", seconds);
  pthread_mutex_lock( &seconds_mutex );
  seconds_remaining+=seconds; //BUGBUG: need to check for overflow.
  pthread_mutex_unlock( &seconds_mutex );
}


/******************************************************************************
 * get_time_remaining - returns the number of seconds remaining.
 ******************************************************************************/
int get_time_remaining() {
  return seconds_remaining;
}


/******************************************************************************
 * Called once per second by the clock thread - decrements the time remaining.
 ******************************************************************************/
void tick() {
  pthread_mutex_lock( &seconds_mutex );
  if (seconds_remaining > 0) {
    seconds_remaining--;
  }
  pthread_mutex_unlock( &seconds_mutex );
  syslog(LOG_INFO, "tick... %d", seconds_remaining);
}

/******************************************************************************
 * clock_thread_start - manages the clock and manages the switch.
 ******************************************************************************/
void *clock_thread_start(void *arg) {
  int screen_active = 0;
  syslog(LOG_INFO, "clock thread started");
  while(1) {
    usleep(1000000); //sleep 1 sec.
    tick();
    if (screen_active && seconds_remaining == 0) {
      screen_active = 0;
      set_switcher(2);
      syslog(LOG_INFO, "time out, switched to 2");
    }
    if (!screen_active && seconds_remaining != 0) {
      screen_active = 1;
      set_switcher(1);
      syslog(LOG_INFO, "time added, switched to 1");
    }
  }
}
