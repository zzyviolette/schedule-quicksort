CC = gcc
FLAGS= -Wall
LINK = -lpthread

all: worksharing workstealing

workstealing: quicksort.c deque.o workstealing.o
	$(CC) $(FLAGS) -o $@ $^ $(LINK)

worksharing: quicksort.c deque.o sched.o
	$(CC) $(FLAGS) -o $@ $^ $(LINK)

%.o: %.c
	$(CC) $(FLAGS) -c $< $(LINK)

.PHONY: clean mrproper

clean:
	$(RM) *.o

mrproper: clean
	$(RM) worksharing workstealing
