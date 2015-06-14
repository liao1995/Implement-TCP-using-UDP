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

/**
 * set segment header length
 * @param s pointer to segment
 * @param len the low 4-bit: new header length to set  
 */
// void header_len(PSegm s, short len)
// {
//   s->head_len = (char)(((len & 0xf) << 4) | s->head_len & 0x0f);
// }

/**
 * set segment control bits
 * @param s pointer to segment
 * @param bits the low 6-bit: new contorl bits to set
 */
// void set_ctrl_bits(PSegm s, short bits)
// {
//   s->ctrl_bits = (char)(bits & 0x3f | s->ctrl_bits & 0xc0); 
// }

/**
 * listen on port - waiting for connection
 * @param port listen port
 * @return On success, a file descriptor for the socket is returned.
 *         On error, -1 is returned, and errno is set appropriately.
 */
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