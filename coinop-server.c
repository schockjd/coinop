
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>


#include "coinop-config.h"
#include "coinop.h"

void *server_thread_start(void *arg) {
  int sock;
  struct sockaddr_in co_server;
  struct sockaddr_in co_client;

  syslog(LOG_INFO, "server thread started");
  
  //
  // Initialize coin server socket
  //
  if ((sock == socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    perror("Failed to create socket"); 
    exit(1);
  }

  //
  // Set up server socket addressing
  // 
  memset(&co_server, 0, sizeof(co_server));
  co_server.sin_family = AF_INET;
  co_server.sin_addr.s_addr = htonl(INADDR_ANY);
  co_server.sin_port = htons(COINSERVER_PORT);
 
  //
  // Bind the socket
  //
  if (bind(sock, (struct sockaddr *) &co_server, sizeof(co_server)) < 0) {
    perror("Failed to bind server socket");
    exit(1);
  }

  while (1);  
  
  return NULL;
}
