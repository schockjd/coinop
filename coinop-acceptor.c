
#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include "coinop-config.h"
#include "coinop.h"

void coin_accepted();

/*****************************************************************************
 * acceptor_thread_start - acceptor thread loop
 *****************************************************************************/
void *acceptor_thread_start(void *arg) {
  int rc;
  syslog(LOG_INFO, "acceptor thread started");	
  while(1) {
    rc = gpio_poll(COIN_GPIO, COIN_ACTIVE_LVL, -1);
    if (rc < 0) {
      break;
    }
    if (rc > 0) {
      coin_accepted();
    }
  }
  syslog(LOG_INFO, "acceptor thread exiting");
  return NULL;
}

/*****************************************************************************
 * coin_accepted - executed when coin acceptor detects a coin insertion
 *****************************************************************************/
void coin_accepted() {
  syslog(LOG_NOTICE, "coin accepted");
  add_time(COIN_TIMEOUT);
}
