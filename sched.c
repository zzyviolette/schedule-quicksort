#include "deque.h"
#include "sched.h"

struct scheduler *sched;

int get_reussi(){
  return 0;
}
int get_echec(){
  return 0;
}

void *thread_func(){
  int actif= 1;
  while(1){
    Task * task = poplast(sched->deques[0]);
    if(task!=NULL){
      if(!actif){
	sched->flag++;
	actif=1;
      }
      (task->func)(task->arg, sched);
    }
    else{
      if(actif){
	sched->flag--;
	actif=0;
      }
      
      if(sched->flag<=0 ){
	    break;
      }
    }

      free(task);
  }

  pthread_exit(NULL);
}



int sched_init(int nthreads, int qlen, taskfunc f, void *closure){

  sched= malloc(sizeof(struct scheduler));
  sched->nthreads = nthreads;
  sched->flag=nthreads;
  sched->deques = malloc(nthreads*sizeof(Deque*));
  sched->deques[0]=malloc(sizeof(Deque));
  Init(sched->deques[0], qlen);
  sched_spawn(f, closure, sched);

  pthread_mutex_init(&sched->mutexflag, NULL);
  sched->pthreads= malloc(nthreads * sizeof(pthread_t));

  int i;
  for(i = 0; i<nthreads; i++){
    pthread_create(&sched->pthreads[i], NULL, thread_func, NULL);
  }
  destroyScheduler();
  return 0;
}


int sched_spawn(taskfunc f, void *closure, struct scheduler *s){
  Task *task;
  task= malloc(sizeof(Task));
  task->func = f;
  task->arg= closure;
  push(s->deques[0] , task);
  return 0;
}

void destroyScheduler(){
      int i;
      for(i = 0; i<sched->nthreads; i++){
        pthread_join(sched->pthreads[i], NULL);
      }
      Destroy(sched->deques[0]);
      pthread_mutex_destroy(&sched->mutexflag);
      free(sched->pthreads);
      free(sched);
}
