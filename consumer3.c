/*THIS IS THE CONSUMER PROCESS*/
/*IN THIS EXAMPLE START THE CONSUMER PROCESS FIRST*/
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<unistd.h>

typedef struct {
  int x;
  int f;
  int prid;
  int cnid;
  int full,empty;
}mydata;

void UnBlock(int *value,mydata *data){
  *value = *value + 1;
  if (*value <= 0)
  {
	  kill(data->prid,SIGUSR1);
  }
}

void Block(int *value){
	*value = *value - 1;
  if(*value<0){
    pause();
  }
  
}

void my_handler(){}

int main(){
  printf("Consumer Program Started...\n");
  signal(SIGUSR1,my_handler);
  int status, i, n;
  int shmid;
  mydata * data;
	key_t key;
	key = ftok("buffer.txt", 12);
	if(key<0){
		perror("error0:");
		exit(1);
	}
	shmid = shmget(key, sizeof(mydata), IPC_CREAT | 0666);
	if(shmid<0){
		perror("error1:");
		exit(1);
	}
	data = (mydata *) shmat(shmid, NULL, 0);
	if(data == (void *) -1){
		perror("error2:");
		exit(1);
	}

  data->cnid = getpid();
  n = 10000;
  pause();

  for(i = 1; i<=n; i++){
    Block(&data->full);
    (data->x)--;
	//printf("%d - consumer - %d\n",data->x,i);
    UnBlock(&data->empty,data);
  }
  data->f = 1;
}
