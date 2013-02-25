
#include <stdlib.h>
#include <stdio.h>

#include "coinop-config.h"
#include "coinop.h"

void coin_accepted();

/*****************************************************************************
 * acceptor_thread_start - acceptor thread loop
 *****************************************************************************/
void *acceptor_thread_start(void *arg) {
  int rc;	
  while(1) {
    rc = gpio_poll(COIN_GPIO, -1, COIN_ACTIVE_LVL);
    if (rc < 0) {
      return NULL;
    }
    if (rc > 0) {
      coin_accepted();
    }
  }
  return NULL;
}

/*****************************************************************************
 * coin_accepted - executed when coin acceptor detects a coin insertion
 *****************************************************************************/
void coin_accepted() {
  add_time(COIN_TIMEOUT);
}
