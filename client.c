#include "mytcp.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int fd;
  if (argc > 2)
  {
    fd = myconnect(argv[1], (short)atoi(argv[2]));
    if (fd<0)
      perror("myconnect");
  }
  else{
    fprintf(stderr, "too few arguments.\n");
    exit(1);
  }
  return 0;
}