/*
 * coin-op gpio routines
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>

#include "coinop-config.h"
#include "coinop.h"

int open_gpio(unsigned int gpio);

/******************************************************************************
 * gpio_set_value
 ******************************************************************************/
int gpio_set_value (unsigned int gpio, unsigned int value) {
  int gpio_fd;
  
  gpio_fd = open_gpio(gpio);

  if (gpio_fd < 0) { 
    perror("gpio/set-value");
  }

  if (value) {
    write(gpio_fd, "1", 2);
  } else {
    write(gpio_fd, "0", 2);
  }

  close(gpio_fd);

  return 0;
}

/******************************************************************************
 * gpio_get_value
 ******************************************************************************/
int gpio_get_value (unsigned int gpio, unsigned int *value) {
  int gpio_fd;
  char ch;

  gpio_fd = open_gpio(gpio);
  
  if (gpio_fd < 0) {
    perror("gpio/get-value");
  }

  read(gpio_fd, &ch, 1);
  if (ch != '0') {
    *value = 1;
  } else {
    *value = 0;
  }

  close(gpio_fd);
  return 0;
}

/******************************************************************************
 * gpio_poll - 
 ******************************************************************************/
int gpio_poll (unsigned int gpio, unsigned int lvl, unsigned int timeout) {
  struct pollfd fdset[1];
  int rc, reading, count_trig, count_notrig, trigger;
  char ch;

  memset((void*)fdset, 0, sizeof(fdset));
  
  fdset[0].fd = open_gpio(gpio);
  
  if (fdset[0].fd < 0) {
    perror("gpio/get-poll");
  }
  
  fdset[0].events = POLLPRI;

  do {
    rc = poll(fdset, 1, timeout);
    if (rc <= 0) {
      close(fdset[0].fd);
    }
    
    /* Interrupt. Do some debounce */
    count_trig = 0;
    count_trig = 0;
    trigger = 0;
    while(1) {
      usleep(DEBOUNCE_PERIOD);
      lseek(fdset[0].fd, 0, SEEK_SET);
      rc = read(fdset[0].fd, &ch, 1);
      if (rc < 0) return rc;
      
      if (ch == '0') {
         reading = 0;
      } else {
         reading = 1;
      }
      
      if (reading == lvl) {
        count_notrig = 0;
        count_trig++;
        if (count_trig > TRIGGER_THRESH) {
          trigger = 1;
        }
      } else { 
        count_trig = 0;
        count_notrig++;
        if (count_notrig > RESET_THRESH) {
          break;
        }
      }
    }
  } while (!trigger);
  close(fdset[0].fd);

  return trigger;
}

/******************************************************************************
 * open_gpio - opens a file descriptor for the specified gpio.
 ******************************************************************************/
int open_gpio(unsigned int gpio) {
  int fd;
  char buf[MAX_BUF];

  snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);
  return open(buf, O_RDWR | O_NONBLOCK);
}
