#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h> // AF_INET, SOCK_STREAM, metode de tip Socket
#include <arpa/inet.h>  //htons
#include <errno.h>
#include <signal.h>

void signal_callback_handler(int signum);
void client_f(int socket_accept);
void server();
