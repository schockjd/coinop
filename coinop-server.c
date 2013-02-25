
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

#include "coinop-config.h"
#include "coinop.h"

void *server_thread_start(void *arg) {
  syslog(LOG_INFO, "server thread started");
  while (1);  
  
  return NULL;
}
