#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <ev.h>

static int ctr = 0;
static int FLAG = 1;
static  char *path = "test.txt";

static void signal_cb(EV_P_ ev_signal *sigterm, int revents)
{
  ev_break (EV_A_ EVBREAK_ALL);
  FLAG = 0;
}

static void timer_cb(struct ev_loop *loop, ev_timer *w, int revents) 
{
  ev_break (EV_A_ EVBREAK_ALL);
  FLAG = 0;
}

static void rw() 
{
  int fd = -1;
  char buf[64];
  int len;

  fd = open(path, O_CREAT | O_RDWR);    
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
  ctr++;
}


int main()
{
  
  ev_signal sig;
  ev_signal_init(&sig, signal_cb, SIGINT);
  ev_signal_start(EV_DEFAULT, &sig);
 
  ev_timer timer;
  ev_timer_init(&timer, timer_cb, 300, 0);   
  ev_timer_start(EV_DEFAULT, &timer);
  
  ev_idle idle;
  ev_idle_init(&idle, rw);
  ev_idle_start(EV_DEFAULT, &idle);

  struct timespec times, timee;
  if (clock_gettime(CLOCK_REALTIME, &times) != 0) {
    fprintf(stderr, "cannot clock_gettime(): %m\n");
    goto _err;
  }
  
  ev_run(EV_DEFAULT, 0);

  if (clock_gettime(CLOCK_REALTIME, &timee) != 0) {
    fprintf(stderr, "cannot clock_gettime(): %m\n");
    goto _err;
  }

  double time = (timee.tv_sec - times.tv_sec) + (timee.tv_nsec - times.tv_nsec)*1e-9; 
  printf("o/w/c #: %d\n", ctr);
  printf("Time[s]: %f\n", time);
  printf("    #/s: %f\n", ctr/time);

 _err:
  return 0;
}

