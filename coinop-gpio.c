/*
 * coin-op gpio routines
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include "coinop-config.h"
#include "coinop.h"


/******************************************************************************
 * gpio_set_value
 ******************************************************************************/
int gpio_set_value (unsigned int gpio, unsigned int vlaue) {
  return 0;
}

int gpio_get_value (unsigned int gpio, unsigned int *value) {
  return 0;
}

int gpio_poll (unsigned int gpio, unsigned int timeout) {
  return 0;
}
