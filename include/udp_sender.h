#ifndef UDP_CLIENT
#define UDP_CLIENT

// returns socket of call to socket
int udp_sender_init(const char* srv_addr, int srv_port);


void udp_sender_cleanup(int sockfd);
// auth_cb is a callback used to define a custom authentication
// between client and server.
int udp_sender_run(const char* srv_addr, int srv_port, int (*auth_cb)(int),
                   void (*exec_cb)(int));

#endif
