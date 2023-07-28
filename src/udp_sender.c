#include "udp_sender.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

static struct sockaddr_in g_servaddr;

int udp_sender_init(const char* srv_addr, int srv_port) {
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  if (sockfd < 0) {
    return -1;
  }

  // assign IP, PORT
  g_servaddr.sin_family = AF_INET;
  g_servaddr.sin_addr.s_addr = inet_addr(srv_addr);
  g_servaddr.sin_port = htons(srv_port);

  return sockfd;
}


void udp_sender_cleanup(int sockfd) { close(sockfd); }

int udp_sender_run(const char* srv_addr, int srv_port, int (*auth_cb)(int),
                   void (*exec_cb)(int)) {

  int sockfd = udp_sender_init(srv_addr, srv_port);

  if (sockfd < 0) return -1;


  if (auth_cb == NULL || !(*auth_cb)(sockfd)) (*exec_cb)(sockfd);

  udp_sender_cleanup(sockfd);

  return 0;
}

#ifdef TEST_UDP_SENDER

#define BUF_SIZE 2
static unsigned int num_of_datagrams=0;

void send_datagrams(int sockfd) {
  printf("Starting to send %u datagrams\n", num_of_datagrams);

  unsigned char buf[BUF_SIZE];
  memset(buf, 3, BUF_SIZE); 

  for( int i = 0; i < num_of_datagrams; i++ ) {
     sendto(sockfd, buf, BUF_SIZE,
        MSG_CONFIRM, (const struct sockaddr *) &g_servaddr, 
            sizeof(g_servaddr));
    printf("Sent datagram %u\n", i+1);
    sleep(1);
  }

  printf("\nTest finished\n");
}

int main(int argc, char** argv) {
  if (argc != 4) {
    printf("Usage: %s <Dest_IP> <Dest_Port> <Num_of_datagrams>\n", argv[0]);
    return -1;
  }

  int port_num = atoi(argv[2]);
  num_of_datagrams = atoi(argv[3]);
  return udp_sender_run(argv[1], port_num, NULL, send_datagrams);
}
#endif

