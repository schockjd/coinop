
#include <stdio.h>
#include <stdlib.h>
#include "coinop-config.h"
#include "coinop.h"

void *server_thread_start(void *arg) {

  printf("Server Started\n");
  sleep(5);
  set_time(50);
  sleep(5);
  return NULL;
}
