#ifndef DEQUE
#define DEQUE

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
struct scheduler;

typedef void (*taskfunc)(void*, struct scheduler *);

typedef struct Task{
    taskfunc func;
    void *arg;
}Task;


typedef struct Element{
  Task * task;
  struct Element *next;
  struct Element *previous;
} Element;

typedef struct Deque{
  int maxSize;
  int size;
  pthread_mutex_t mutex;
  Element *first;
  Element *last;
} Deque;

void Init(Deque *deque, int maxSize);

void Destroy(Deque *deque);

int push(Deque *deque, Task* task);
Task* popfirst(Deque *deque);
Task* poplast(Deque *deque);

int IsEmpty(Deque *deque);
int IsFull(Deque *deque);

#endif
