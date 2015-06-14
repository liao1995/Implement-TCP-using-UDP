#include "mytcp.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int fd;
  fd = mylisten(SRV_PORT);
  if (fd < 0)
    perror("mylisten");
  return 0;
}