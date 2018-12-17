#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#define SOCKET_BUFFERSIZE 655336
int sock_setup_connection(const char *port) 
{
  int sockfd;
  int rc;
  int opt = 1;
  int backlog = 10;
  struct addrinfo *servinfo, *p;
  struct addrinfo hints = {
    .ai_family = AF_UNSPEC,
    .ai_socktype = SOCK_STREAM,
    .ai_flags = AI_PASSIVE, // use my IP
  };

  if ((rc = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rc));
    return -1;
  }

  for(p = servinfo; p != NULL; p = p->ai_next) {

    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      continue;
    }
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
      fprintf(stderr, "cannot set SO_REUSEADDR: %s\n", strerror(errno));
      freeaddrinfo(servinfo);
    }
    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      fprintf(stderr, "cannot bind: %s\n", strerror(errno));
      close(sockfd);
      continue;
    }
    break;
  }
  
  freeaddrinfo(servinfo);
  if (listen(sockfd, backlog) == -1) {
    fprintf(stderr, "cannot listen: %s\n", strerror(errno));
    close(sockfd);
    return -1;
  }
 
  struct sockaddr_in addr;
  socklen_t addrlen = sizeof(addr);
  int fd = -1;
  
  if ((fd = accept(sockfd, (struct sockaddr *)&addr, &addrlen)) < 0)
    if (!(errno == EAGAIN || errno == EWOULDBLOCK || errno == ECONNABORTED))
      fprintf(stderr, "cannot accept connections: %s\n", strerror(errno));

  char request[SOCKET_BUFFERSIZE];
  int bytes_recvd = recv(fd, request, sizeof(request), 0);
  if (bytes_recvd < 0)
    fprintf(stderr, "cannot recv: %s\n", strerror(errno));
  printf(request);    

  char *buf = "testing connection successful\n";
  int rv = send(fd, buf, strlen(buf), 0);
  
  return fd;
}


int main(int argc, char *argv[])
{
  const char *port = argv[1];
  int fd = -1;
  
  fd = sock_setup_connection(port);  
 

  return 0;
}

