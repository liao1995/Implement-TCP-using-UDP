/**
 * source file to implement some tcp functions.
 * @author LIAO I033040007
 * 2015.6.12
 */

#include "mytcp.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <memory.h>


int mylisten(short port)
{
  int fd;
  struct sockaddr_in srv;

  fd=socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) return -1;
  
  srv.sin_family = AF_INET;
  srv.sin_port = htons(port);
  srv.sin_addr.s_addr = htonl(INADDR_ANY); // any of server addresses
  
  if (bind(fd, (struct sockaddr *)&srv, sizeof(srv)) < 0) return -1;
  
  // establish connection
  
  
  return fd;
}

int main()
{
  //printf("length of header: %ld\n", sizeof(Segm));
  Segm s;
  memset(&s, 0, sizeof(s));
  printf("ack = %u\n", s.ack);
  s.ack = 1;
  return 0;
}