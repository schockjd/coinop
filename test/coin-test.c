/* Copyright (c) 2011, John Schock
 * All rights reserved.
 * <GPL here if redistro'd>
 */

/**********************************************************
 * Constants
 **********************************************************/
#include "coin-op.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <syslog.h>

/**********************************************************
 * gpio_set_value
 **********************************************************/
int gpio_set_value(unsigned int gpio, unsigned int value) {
    int fd;
    char gpio_path[MAX_BUF];

    snprintf(gpio_path, sizeof(gpio_path), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

    fd = open(gpio_path, O_WRONLY);
    if (fd < 0) return fd;

    if (value)
        write(fd, "1", 2);
    else
        write(fd, "0", 2);

    close(fd);
    return 0;
}


/**********************************************************
 * gpio_get_value
 **********************************************************/
int gpio_get_value(unsigned int gpio, unsigned int *value) {
    int fd;
    char gpio_path[MAX_BUF];
    char ch;

    snprintf(gpio_path, sizeof(gpio_path), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

    fd = open(gpio_path, O_RDONLY);
    if (fd < 0) return fd;
    
    read(fd, &ch, 1);

    if (ch != '0')
        *value = 1;
    else
        *value = 0;

    close(fd);
    return 0;
}


/**********************************************************
 * gpio_poll
 **********************************************************/
int gpio_poll(unsigned int gpio, unsigned int timeout) {
    struct pollfd fdset[1];
    char gpio_path[MAX_BUF];
    char ch;
    int rc;
    int sample;
    

    memset((void *)fdset, 0, sizeof(fdset));

    snprintf(gpio_path, sizeof(gpio_path), SYSFS_GPIO_DIR "/gpio%d/value", gpio);
    
    fdset[0].fd = open(gpio_path, O_RDONLY | O_NONBLOCK);
    if (fdset[0].fd < 0) return fdset[0].fd;
    fdset[0].events = POLLPRI;

    /* first poll after open always returns interrupt */
    rc = read(fdset[0].fd, &ch, 1);
    if (rc < 0) {
        close(fdset[0].fd);
        return rc;
    }

    rc = poll(fdset, 1, timeout);
    close(fdset[0].fd);
    return rc;
}


/**********************************************************
 * main
 **********************************************************/
int main (int argc, char **argv, char **envp) {
    int rc;
    int gpio_val;
    int sample_count;

    
    printf("coin-test starting.");

    while(1) {
        rc = gpio_poll(COIN_GPIO, POLL_TIMEOUT);
        if (rc < 0) {
            printf("gpio_poll failed");
            return rc;      
        }
	if (rc == 0) continue;
        printf("\n*"); 
	for (sample_count = 0; sample_count < 1000; sample_count++) {
            gpio_get_value(COIN_GPIO, &gpio_val);
            if (gpio_val == 0) {
              printf("_");
            } else {
              printf("^");
            }
            usleep(5);
        }
    }
    return 0;
}

