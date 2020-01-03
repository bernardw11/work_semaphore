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
  state_errors("accessing the shared memory");
  char * mem;
  mem = shmat(shm, 0, 0);
  state_errors("attaching the shared memory");
  sem = semget(KEY, 1, 0);
  state_errors("getting the semaphore");
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_op = -1;
  printf("trying to get in\n");
  semop(sem, &sb, 1);
  state_errors("getting semaphore");
  printf("Last addition: %s\n", mem);
  //opening the text file
  fd = open("telephone.txt", O_WRONLY | O_APPEND);
  state_errors("opening the file");
  printf("Your addition: ");
  char buff[SIZE];
  fgets(mem, SIZE, stdin);
  *strchr(mem, '\n') = 0;
  strcpy(buff, mem);
  write(fd, buff, strlen(buff));
  write(fd, "\n", 1);
  state_errors("writing to the file");
  sb.sem_op = 1;
  semop(sem, &sb, 1);
  state_errors("updating the semaphore");
  shmdt(mem);
  state_errors("detaching the shared memory");
  return 0;
}
