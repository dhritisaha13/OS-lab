#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<pthread.h>
#include<fcntl.h>
#include<errno.h>

struct mem
{
	int buffer;
};

pthread_mutex_t mutex; 


void* func(void *arg)
{
	int count=0;
	struct mem *data = (struct mem*)arg;
	
	while(data->buffer<24)
	{	
		pthread_mutex_lock(&mutex);
		count++;
		data->buffer++;
		printf("Current value of buffer:%d\n",data->buffer);
		printf("Thread id: %u\n",pthread_self());
		printf("Process id: %d\n",getpid());
		pthread_mutex_unlock(&mutex);
		sleep(1);

	}
	printf("Thread %u changed value of buffer for %d times\n",pthread_self(), count);

	return NULL;
}

int main()
{	
	key_t key=1152;
	int shmid;
	struct mem *shm;


	shmid = shmget(key,sizeof(struct mem*),IPC_CREAT|0666);

	shm = (struct mem*)shmat(shmid,NULL,0);

	shm->buffer = 0;

	pthread_t tid1;
	pthread_t tid2;
	pthread_t tid3;

	pthread_mutex_init(&mutex,NULL);

	pthread_create(&tid1,NULL, func,(void*)shm);
	pthread_create(&tid2,NULL, func,(void*)shm);
	pthread_create(&tid3,NULL, func,(void*)shm);


	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	pthread_join(tid3,NULL);
	return 0;
}




 
