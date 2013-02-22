
#include <stdlib.h>
#include <stdio.h>
#include "coinop-config.h"
#include "coinop.h"

void *acceptor_thread_start(void *arg) {
 printf("Acceptor started\n");
 sleep(2);
 return NULL;
}
