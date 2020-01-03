#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>

//write program!

#define KEY 1000000
#define SIZE 1000

void state_errors(char * x){
  if (errno){
    printf("Error %s: %d - %s\n", x, errno, strerror(errno));
    errno = 0;
  }
}

int main(){
  int shm, sem, fd;
  shm = shmget(KEY, SIZE, 0);
  state_errors("accessing shared memory");
  char * mem;
  mem = shmat(shm, 0, 0);
  state_errors("attaching shared memory");
  sem = semget(KEY, 1, 0);
  state_errors("getting semaphore");
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_op = -1;
  printf("getting in... (don't type until \"Your addition\" appears)\n");
  semop(sem, &sb, 1);
  state_errors("getting semaphore");
  printf("Last addition:\n%s\n", mem);
  printf("Your addition:\n");
  char text[SIZE];
  fgets(mem, SIZE, stdin);
  *strchr(mem, '\n') = 0;
  strcpy(text, mem);
  fd = open("story", O_WRONLY | O_APPEND);
  state_errors("opening file");
  write(fd, text, strlen(text));
  write(fd, "\n", 1);
  state_errors("writing to file");
  sb.sem_op = 1;
  semop(sem, &sb, 1);
  state_errors("updating semaphore");
  shmdt(mem);
  state_errors("detaching shared memory");
  return 0;
}
