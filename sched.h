#ifndef SCHED
#define SCHED

#include "deque.h"
#include <unistd.h>

struct scheduler{
  int nthreads;
  pthread_t * pthreads;
  Deque ** deques;
  _Atomic int flag;
  pthread_mutex_t mutexflag;
  pthread_key_t p_key;
};

static inline int
sched_default_threads()
{
    return sysconf(_SC_NPROCESSORS_ONLN);
}

int get_reussi();
int get_echec();
int sched_init(int nthreads, int qlen, taskfunc f, void *closure);
int sched_spawn(taskfunc f, void *closure, struct scheduler *s);
void* thread_func();
void destroyScheduler();

#endif
