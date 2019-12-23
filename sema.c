#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>

#include "err.h"
#include "sema.h"

union semun {
  int              val;    /* Value for SETVAL */
  struct  semid_ds *buf;     /*  Buffer for  IPC_STAT,
				 IPC_SET */
  unsigned short  *array;  /* Array for GETALL, SETALL */
  struct seminfo  *__buf;  /* Buffer for IPC_INFO
			      (Linux-specific) */
};

int sem_config(int key,int flags,int n,...){
  int i,r,semid;

  semid = semget(key,n,flags);
  exit_err("configuring semaphore array",semid);

  va_list init_vals;
  va_start(init_vals,n);

  short vals[n];
  for( i=0; i<n; i++ ) vals[i] = (short)(va_arg(init_vals,int));

  union semun us;
  us.array = vals;
  r = semctl(semid,n,SETALL,us);
  exit_err("initializing semaphore values",r);

  va_end(init_vals);
  return semid;
}
int sem_connect(int key,int n){
  int r = semget(key,n,0);
  exit_err("connecting to semaphores",r);
  return r;
}
int sem_getval(int semid,int idx){
  int r = semctl(semid,idx,GETVAL);
  exit_err("checking semaphore value",r);
  return r;
}
void sem_remove(int semid){
  int r = semctl(semid,0,IPC_RMID);
  exit_err("removing semaphore",r);
}

void sem_claim(int semid,int idx){
  struct sembuf op;
  op.sem_op = -1;
  op.sem_num = idx;
  int r = semop(semid,&op,1);
  exit_err("claiming semaphore",r);
}
void sem_release(int semid,int idx){
  struct sembuf op;
  op.sem_op = 1;
  op.sem_num = idx;
  int r = semop(semid,&op,1);
  exit_err("releasing semaphore",r);
}
void sem_listen(int semid, int idx){
  struct sembuf op;
  op.sem_op = 0;
  op.sem_num = idx;
  int r = semop(semid,&op,1);
  exit_err("listening to semaphore",r);
}
