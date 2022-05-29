# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <stdbool.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <sys/ipc.h>
# include <sys/msg.h>
# include <sys/shm.h>
# include <string.h>

struct mesg_buffer{
	long mesg_type;
	int val;
}message;

int count = 0;
int size;
long int size_bytes;
char *input_file;
int block_size;
int n;
int shmid;
pthread_mutex_t lock;
char* str;
long int sum = 0;

void continueHandler(int signum){
	// printf("q2 resumed\n");
}
void sig_handler(int signum){
	// printf("q2 paused\n");
	pause();
}


pthread_mutex_t lock;
void* add(void* temp){
	int init = (int) temp;
	int end = init + block_size;
	if((n - 1) * block_size == init){
		//last block
		end = size_bytes;
	}
	for(int i = init; i < end; i++){
		if(str[i] == '\0'){
			//this index has not been written yet
			i--;
			continue;
		}
		else if(str[i] != ' '){
			pthread_mutex_lock(&lock);
			sum += str[i];
			pthread_mutex_unlock(&lock);
		}
	}
}

int main(int argc, char *argv[]){

	if (argc != 2) {
		printf("Usage: ./a.out total_thread");
		exit(-1);    
	}

	//this process accepts data from P1 and adds them using threads
	signal(SIGALRM,sig_handler); // Register signal handler
	signal(SIGCONT,continueHandler);
  	
    int msgid = msgget(1122, 0666 | IPC_CREAT);
    msgrcv(msgid, &message, sizeof(message),1, 0);
	size_bytes = message.val;
	printf("P2:size in bytes: %d\n", size_bytes);

	//shared memory for data
	shmid = shmget(21410,10000000,0);
	if(shmid == -1){
		perror("P2:error in shmget()\n");
	}
	str = (char*)shmat(shmid, 0, 0);
	if(str == (void*)-1){
		perror("P2:error in shmat()\n");
	}
	ualarm(1000,1000);
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n P2:mutex init has failed\n");
        return 1;
    }
	//now str is having the data being sent from P1
	//we will create threads that will read and add the data



	//n = 2; //n is no. of threads
	n = strtol(argv[1],0,0);

	block_size = size_bytes / n;
	printf("P2:number of threads: %d\n", n);
	pthread_t tid[n];

	struct timeval start,stop;
	double secs = 0;

	gettimeofday(&start, NULL);
	for(int i = 0; i < n; i++){
		int init = i * block_size;
		pthread_create(&tid[i],NULL, &add, (void*)init);
	}
	for(int i = 0; i < n; i++){
		pthread_join(tid[i], NULL);
	}
	gettimeofday(&stop, NULL);
	printf("P2:sum: %ld\n", sum);
	shmdt((void *) str);
	shmctl(shmid,IPC_RMID,NULL);
	secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
	printf("P2:total elapsed time: %f\n", secs);

}