
#include <stdlib.h>
#include <stdio.h>
#include "coinop-config.h"
#include "coinop.h"

void *acceptor_thread_start(void *arg) {
 printf("Acceptor started\n");
 add_time(3);
 sleep(10);
 return NULL;
}
