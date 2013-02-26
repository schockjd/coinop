
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>


#include "coinop-config.h"
#include "coinop.h"
#include "coinop-server.h"

void handleRequest(int socket);

void *server_thread_start(void *arg) {
  int sock;
  struct sockaddr_in co_server;

  syslog(LOG_INFO, "server thread started");
  
  //
  // Initialize coin server socket
  //
  if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
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

  while (1) {
    handleRequest(sock); 
  }  
  
  return NULL;
}

/*****************************************************************************
 * handleRequest - deals with client requests to the coinop server.
 *****************************************************************************/
void handleRequest(int sock) {
  struct sockaddr_in co_client;
  char buffer[MAX_BUF];
  int received, co_client_len;
  co_client_len = sizeof (co_client);
  if (received = recvfrom(sock, buffer, MAX_BUF, 0, (struct sockaddr *) &co_client, &co_client_len) <= 0) {
    return;
  }
  syslog(LOG_INFO, "Client connected: %s", inet_ntoa(co_client.sin_addr));
  switch (buffer[0]) {
    case CMD_TURN_ON:
      syslog(LOG_INFO, "executing turn-on request");
      force_on();
      break;
    case CMD_TURN_OFF:
      syslog(LOG_INFO, "executing turn-off request");
      force_off();
      break;
    case CMD_SET_TIME:
    case CMD_ADD_TIME:
    case CMD_GET_TIME: 
    default:
      syslog(LOG_INFO, "unimplemented server command 0x%02x", buffer[0]);
  }
}
