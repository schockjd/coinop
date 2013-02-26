#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define MAX_BUF 64

int main(int argc, char *argv[]) {
  int sock,i;
  struct sockaddr_in co_server;
  char buffer[MAX_BUF];

  if (argc != 4) { 
    fprintf(stderr, "USAGE: %s <server_ip> <port> <data0> .. <datan>\n", argv[0]);
    exit(1);
  }

  if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    perror("Failed to create socket"); exit(1);
  }

  memset(&co_server, 0, sizeof(co_server));
  co_server.sin_family = AF_INET;
  co_server.sin_addr.s_addr = inet_addr(argv[1]);
  co_server.sin_port = htons(atoi(argv[2]));

  for (i = 3; i < argc; i++) {
    buffer[i-3] = (char)atoi(argv[i]);
  }

  if (sendto(sock, buffer, argc-3, 0, (struct sockaddr *) &co_server, sizeof (co_server)) != argc-3) {
    perror("Mismatch in sent bytes");
  }
  close(sock);
  exit(0);
}
