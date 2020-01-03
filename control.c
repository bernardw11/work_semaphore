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

//control program! reads in cmd line. creating: -c. removing: -r. viewing: -v

#define KEY 1000000
#define SIZE 1000000

union semun {
  int              val;    /* Value for SETVAL */
  struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short  *array;  /* Array for GETALL, SETALL */
  struct seminfo  *__buf;  /* Buffer for IPC_INFO
                              (Linux-specific) */
};

void state_errors(char * x){
  if (errno){
    printf("Error %s: %d - %s\n", x, errno, strerror(errno));
    errno = 0;
  }
}

int main(int argc, char * arvg[]){
  int shm, sem, fd;
  union semun su;
  struct sembuf sb;
  su.val = 1;
  sb.sem_num = 0;
  sb.sem_op = -1;

  char flag [4];
  strcpy(flag, argv[1])
  //creating
  if (strcmp(flag, "-c") == 0){
    //creating the semaphore
    sem = semget(KEY, 1, IPC_CREATE | 0644);
    state_errors("making the semaphore");
    printf("semaphore created\n")
    semctl(semd, 0, SETVAL, su);
    //creating the shared memory
    shm = shmget(KEY, SIZE, IPC_CREAT | 0644);
    state_errors("making the shared memory");
    printf("shared memory created\n");
    //opening the text file
    fd = open("telephone.txt", O_CREAT | O_TRUNC | O_RDWR, 0644);
    state_errors("opening the file");
    close(fd);
    printf("file created\n");
  }
  //removing
  else if (strcmp(flag, "-r") == 0){
    //getting the semaphore
    sem = semget(KEY, 1, 0);
    state_errors("getting the semaphore");
    printf("trying to get in\n");
    semop(sem, &sb, 1);
    //getting the shared memory
    shm = shmget(KEY, SIZE, 0);
    state_errors("getting the shared memory");
    //opening the text file
    fd = open("telephone.txt", O_RDONLY);
    state_errors("opening the file");
    //printing everything
    printf("The story so far:\n");
    char buff[SIZE];
    //THIS MAY SCREW EVERYTHING UP
    buff[0] = '\0';
    read(fd, buff, SIZE);
    if (strlen(buff) != 0) {
      *(strrchr(buff, '\n') + 1) = '\0';
    }
    printf("%s\n", buff);
    close(fd);
    //removing the shared memory
    shmctl(shm, IPC_RMID, 0);
    state_errors("removing the shared memory");
    printf("shared memory removed\n");
    //removing semaphore
    semctl(sem, IPC_RMID, 0);
    state_errors("removing the semaphore");
    printf("semaphore removed\n");
  }
  //viewing
  else if (strcmp(flag, "-v") == 0){
    fd = open("telephone.txt", O_RDONLY);
    state_errors("opening the file");
    char buff[SIZE];
    buff[0] = '\0';
    read(fd, buff, SIZE);
    if (strlen(buff) != 0) {
      *(strrchr(buff, '\n') + 1) = '\0';
    }
    printf("The story so far:\n");
    printf("%s\n", buff);
    close(fd);
  }
  else{
    printf("No flag.\n");
    return 0;
  }
}
