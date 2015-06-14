#include "mytcp.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int fd;
  
  fd = mylisten(SRV_PORT);
  
  if (fd < 0)
    perror("mylisten");

  myclose(fd);
  myreceive(fd);
  if(isfin(fd))
    printf("Complete the four way handshake\n");

  return 0;
}