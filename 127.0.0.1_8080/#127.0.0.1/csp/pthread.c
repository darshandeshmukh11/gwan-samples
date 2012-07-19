//demonstrates the dangers of thread-unsafe code
//http://forum.gwan.com/index.php?p=/discussion/comment/3952/#Comment_3952

#include "gwan.h"
#include "stdio.h"
#include "sys/syscall.h"
#include "pthread.h"
#include "sys/time.h"
#include <sys/types.h>
#include "unistd.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void tsleep(int seconds)
{
  struct timespec wait;
  clock_gettime(CLOCK_REALTIME, &wait);
  wait.tv_sec += seconds;

  pthread_cond_timedwait(&cond, &mutex, &wait);
}

static char *myfriend = "";
static int slept = 0;

void *servlet_main(void *name)
{
  printf("%zu: setting myfriend = %s\n", syscall(SYS_gettid), (char*)name);
  
  myfriend = (char*)name;
  
  //the first thread sleeps to force the issue to occur
  if (!slept)
  {
    slept = 1;
    tsleep(1);
  }
  
  printf("%zu: myfriend is actually = %s\n", syscall(SYS_gettid), myfriend);
  
  return 0;
}

int main(int argc, char *argv[])
{
  pthread_t threads[2];
  
  pthread_create(&threads[0], 0, servlet_main, "queirogab");
  pthread_create(&threads[1], 0, servlet_main, "nilsonrio");
  
  pthread_join(threads[0], 0);
  pthread_join(threads[1], 0);
  
  slept = 0;
  
  return 204;
}
