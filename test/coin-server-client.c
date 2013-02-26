#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define MAX_BUF 64

int main(int argc, char *argv[]) {
  int sock, i, received, server_len;
  struct sockaddr_in co_server;
  char buffer[MAX_BUF];

  if (argc < 4) { 
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

  printf("sending: ");
  for (i = 0; i < argc-3; i++) {
    buffer[i] = (char)atoi(argv[i+3]);
    printf("0x%02x ", buffer[i]);
  }
  printf("\n");

  if (sendto(sock, buffer, argc-3, 0, (struct sockaddr *) &co_server, sizeof (co_server)) != argc-3) {
    perror("Mismatch in sent bytes");
  }
  server_len = sizeof(co_server);
  if ( (received = recvfrom(sock, buffer, MAX_BUF, 0, (struct sockaddr *) &co_server, &server_len)) <= 0) {
     perror("Error getting packet from server.");
  }
  printf("received: ");
  for (i=0; i < received; i++) {
    printf("0x%02x ", buffer[i]);
  }
  printf("\n");

  close(sock);
  exit(0);
}
