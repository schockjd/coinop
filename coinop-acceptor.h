/* Copyright (c) 2011, John Schock
 * All rights reserved.
 * <GPL here if redistro'd>
 */

/**********************************************************
 * Constants
 **********************************************************/
#define DEBUG 0

#define SYSFS_GPIO_DIR "/sys/class/gpio"

#define COIN_GPIO  24
#define HDMI1_GPIO 25
#define HDMI2_GPIO 8
#define HDMI3_GPIO 7

#define POLL_TIMEOUT (1*1000)     /* 1 second poll(2) interval */
#define DEBOUNCE_PERIOD  5        /* Sample signal every 500us for debounce */
#define TRIGGER_THRESH 20         /* If sampled low for more than this count, considered a trigger */ 
#define RESET_THRESH 100           /* breaks out of debounce loop if observed high for this many samples */
#define COIN_TIMEOUT (30*60)      /* token value, in seconds tokens are worth 30 minutes */

#define MAX_BUF 64
