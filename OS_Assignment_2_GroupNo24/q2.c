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

int main(){
	//this process accepts data from P1 and adds them using threads

    int msgid = msgget(1122, 0666 | IPC_CREAT);
    msgrcv(msgid, &message, sizeof(message),1, 0);
	size_bytes = message.val;
	printf("size in bytes: %d\n", size_bytes);

	//shared memory for data
	shmid = shmget(2041,10000000,0);
	if(shmid == -1){
		perror("error in shmget()\n");
	}
	str = (char*)shmat(shmid, 0, 0);
	if(str == (void*)-1){
		perror("error in shmat()\n");
	}

    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }
	//now str is having the data being sent from P1
	//we will create threads that will read and add the data



	//n = 2; //n is no. of threads

    printf("enter no. of threads: ");
    scanf("%d", &n);

	block_size = size_bytes / n;
	printf("number of threads: %d\n", n);
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
	printf("sum: %ld\n", sum);
	shmdt((void *) str);
	shmctl(shmid,IPC_RMID,NULL);
	secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
	printf("total elapsed time: %f\n", secs);

}