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

void standard_errors(char * x){
  if (errno){
    printf("Error %s: %d - %s\n", x, errno, strerror(errno));
    errno = 0;
  }
}

int main(int argc, char * arvg[])
