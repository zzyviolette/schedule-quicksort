#include "deque.h"

void Init(Deque *deque, int maxSize){
  deque->maxSize= maxSize;
  deque->size=0;
  pthread_mutex_init(&deque->mutex, NULL);
}

void Destroy(Deque *deque){
  pthread_mutex_destroy(&deque->mutex);
  free(deque);
}

int push(Deque *deque, Task* task){
  if(task == NULL)
    return 0;
  if(IsFull(deque)){
    fprintf(stderr, "Can't push task on stack: stack is full.\n");
    exit(1);  /* Exit, returning error code. */
  }
  Element *temp= malloc(sizeof(Element));
  temp->task= task;
  
  pthread_mutex_lock (&deque->mutex);
  
  temp->previous= deque->last;
  if(deque->last !=NULL)
    deque->last->next = temp;
  
  deque->last=temp;
  if(IsEmpty(deque)){
    deque->first=temp;
  }
  deque->size++;
  pthread_mutex_unlock (&deque->mutex);
  return 1;
}

Task* popfirst(Deque *deque){
  pthread_mutex_lock (&deque->mutex);
  if (IsEmpty(deque)) {
    pthread_mutex_unlock (&deque->mutex);
    return NULL;
  }
  deque->size --;
  Element *chaintemp= deque->first;
  deque->first = chaintemp->next;
  Task* temp = chaintemp->task;
  if(deque->size == 0){
    deque->last = NULL;
    free(chaintemp);
  }
  pthread_mutex_unlock (&deque->mutex);
  return temp;
}
Task* poplast(Deque *deque){
  pthread_mutex_lock (&deque->mutex);
  if (IsEmpty(deque)) {
    pthread_mutex_unlock (&deque->mutex);
    return NULL;
  }
  deque->size --;
  Element *chaintemp= deque->last;
  deque->last = chaintemp->previous;

  Task* temp = chaintemp->task;

  if(deque->size == 0){
    free(chaintemp);
    deque->first = NULL;
  }
  pthread_mutex_unlock (&deque->mutex);
  return temp;
}

int IsEmpty(Deque *deque){
  return deque->size == 0;
}
int IsFull(Deque *deque){
  return deque->size == deque->maxSize;
}
