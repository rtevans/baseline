#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

int main()
{

  int fd  = -1;
  int ctr = 0;

  char buf[64];
  int len;

  char *path = "test.txt";

  while(1) {    
     fd = open("test.txt", O_CREAT | O_RDWR);    
     if (fd < 0) {
       fprintf(stderr, "cannot open test file `%s': %s\n", path, strerror(errno));
       exit(1);
     }

     len = snprintf(buf, sizeof(buf), "%d\n", ctr++);
     
     write(fd, "%s\n", buf);
     if (write(fd, buf, len) != len) {
       fprintf(stderr, "cannot write to test file `%s': %s\n", path, strerror(errno));
       exit(1);
     }
     close(fd); 
  }

  return 0;
}

