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
    int count_hi;
    int count_lo;
    int trigger;

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

    while (1) {
        rc = poll(fdset, 1, timeout);

        if (rc <= 0) 
        {
            close(fdset[0].fd);
            return rc;        
        }
        
        /* Interrupt. Do some debounce */
	count_hi = 0;
        count_lo = 0;
        trigger = 0;
        while (1) {
            usleep(DEBOUNCE_PERIOD);
            lseek(fdset[0].fd, 0, SEEK_SET);
            rc = read(fdset[0].fd, &ch, 1);
            if (rc < 0) return rc;
            if (ch == '0') { //sampled low
              count_hi = 0;
              count_lo++;
              if (count_lo > TRIGGER_THRESH) {
                trigger = 1;
              }
            } else { //sampled high
              count_lo = 0;
              count_hi++;
              if (count_hi > RESET_THRESH) {
                break;
              }
            }
        }
        
        if (trigger == 1) {
            close(fdset[0].fd);
            return 1;
        }
        syslog(LOG_NOTICE, "Spurious interrupt.");
    }
    return 0;
}


/**********************************************************
 * main
 **********************************************************/
int main (int argc, char **argv, char **envp) {
    int child_pid;
    int rc;
    int coin_timeout=0;
    int coin_active=0;

    child_pid = fork();
    if (child_pid != 0) {
        exit(0);
    }

    umask(0);
    setsid();
    
    rc = gpio_set_value(HDMI1_GPIO, 0);
    if (rc < 0) {
        syslog(LOG_CRIT, "GPIO set for HDMI1 failed.");
        return rc;
    }
    rc = gpio_set_value(HDMI3_GPIO, 0);
    if (rc < 0) {
        syslog(LOG_CRIT, "GPIO set for HDMI3 failed.");
        return rc;
    }
    rc = gpio_set_value(HDMI2_GPIO, 1);
    if (rc < 0) {
        syslog(LOG_CRIT, "GPIO set for HDMI2 failed.");
        return rc;
    }

    syslog(LOG_NOTICE, "coin-op starting.");

    while(1) {
        rc = gpio_poll(COIN_GPIO, POLL_TIMEOUT);
        if (rc < 0) {
            syslog(LOG_CRIT, "gpio_poll failed");
            return rc;      
        }
        if (rc ==0) {
            if (coin_timeout > 0) {
                coin_timeout--;
#if DEBUG
                syslog(LOG_NOTICE, "Debug: coin timeout: %i seconds", coin_timeout);
#endif
            }
            if (coin_timeout == 0 && coin_active == 1) {
                coin_active = 0;
                rc = gpio_set_value(HDMI2_GPIO, 1);
                if (rc < 0) {
                    syslog(LOG_CRIT, "GPIO set for HDMI2 failed.");
                    return rc;
                }
                rc = gpio_set_value(HDMI1_GPIO, 0);
                if (rc < 0) {
                    syslog(LOG_CRIT, "GPIO set for HDMI1 failed.");
                    return rc;
                }
                syslog(LOG_NOTICE, "Coin Expired.");
            }
        } else {
            if (coin_active == 0) {
                rc = gpio_set_value(HDMI2_GPIO, 0);
                if (rc < 0) {
                    syslog(LOG_CRIT, "GPIO set for HDMI2 failed.");
                    return rc;
                }
                rc = gpio_set_value(HDMI1_GPIO, 1);
                if (rc < 0) {
                    syslog(LOG_CRIT, "GPIO set for HDMI1 failed.");
                }
                coin_timeout = COIN_TIMEOUT;
                coin_active = 1;
            } else {
                coin_timeout += COIN_TIMEOUT;
            }
            syslog(LOG_NOTICE, "Coin Inserted. Adding %i minutes. %i minutes remaining.", COIN_TIMEOUT/60, coin_timeout/60);
        }
    }
    return 0;
}

