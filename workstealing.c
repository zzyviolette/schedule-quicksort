#include "deque.h"
#include "sched.h"
#include <time.h>

struct scheduler *sched;
_Atomic int reussi=0;
_Atomic int echec=0;

void *thread_func(void *args){
  int actif= 1;
  int me = *((int*)args);
  pthread_setspecific(sched->p_key,&me);
  int i;
  while(1){
    Task * task = poplast(sched->deques[me]);
    if(task != NULL){
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
      if(sched->flag <= 0){
	    break;
      }
      else{
	int before = reussi;
	for(i = 0; i< sched->nthreads; i++){	  
	  if((task = popfirst(sched->deques[i])) != NULL){
	    sched->flag++;
	    actif=1;
	    reussi++;
	    (task->func)(task->arg, sched);
	    break;
	  }
	}
	if(before != reussi)
	  echec++;
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
  pthread_key_create(&sched->p_key,NULL);
  int i;
  for(i =0; i<nthreads; i++){
    sched->deques[i]=malloc(sizeof(Deque));
    Init(sched->deques[i], qlen);
  }
  Task *task;
  task = malloc(sizeof(Task));
  task->func = f;
  task -> arg = closure;
  push(sched->deques[0], task);

  pthread_mutex_init(&sched->mutexflag, NULL);
  sched->pthreads= malloc(nthreads * sizeof(pthread_t));

  
  for(i = 0; i<nthreads; i++){
    int *i2= malloc(sizeof(int));
    *i2=i;
    pthread_create(&sched->pthreads[i], NULL, thread_func,i2);
  }
  destroyScheduler();
  return 0;
}
int sched_spawn(taskfunc f, void *closure, struct scheduler *s){
  Task *task;
  task = malloc(sizeof(Task));
  task->func = f;
  task -> arg = closure;
  int *tmp = (int*)pthread_getspecific(sched->p_key);
  push(s->deques[*tmp] , task);

  return 0;
}
void destroyScheduler(){
   int i;
   for(i = 0; i<sched->nthreads; i++){
     pthread_join(sched->pthreads[i], NULL);
   }
   pthread_key_delete(sched->p_key);
   for(i = 0; i< sched->nthreads; i++){
     Destroy(sched->deques[i]);
   }
   pthread_mutex_destroy(&sched->mutexflag);
   free(sched->pthreads);
   free(sched);
}


int get_reussi(){
  return reussi;
}

int get_echec(){
  return echec;
}
