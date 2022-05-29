# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <stdbool.h>
# include <pthread.h>
# include <unistd.h>
# include <time.h>
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
int k = 0;

void* reader(void* init_){
	long int init = (long int)init_;

	FILE *fp = fopen(input_file, "r");
	fseek(fp, init, SEEK_SET);
	int c = 0;
	int end_byte = init + block_size;
	int x;
	char ch;
	while(ftell(fp) < end_byte){
		ch = fgetc(fp);
		// if(ch == ' ' || ch == EOF){
		// 	continue;
		// }
		x = (int)ch;
		// printf("%c ", ch);
		pthread_mutex_lock(&lock);
		str[k++] = ch;
		pthread_mutex_unlock(&lock);
		c += 1;

	}
	if((n-1) * block_size == init){
		//thread reading the last block
		ch = fgetc(fp);
		while(ch != EOF){
			// if(ch == ' '){
			// 	ch = fgetc(fp);
			// 	continue;
			// }
			x = (int)ch;
			pthread_mutex_lock(&lock);
		    str[k++] = ch;
		    pthread_mutex_unlock(&lock);
			c += 1;
			ch = fgetc(fp);
		}
	}
	count += c;
}

void file_reader(){
	printf("Number of Threads: %d\n", n);
	block_size = size_bytes / n;
	pthread_t tid[n];
	for(int i = 0; i < n; i++){
		long int init = block_size * i;
		pthread_create(&tid[i], NULL, &reader, (void*) init);
	}
	for(int i = 0; i < n; i++){
		pthread_join(tid[i], NULL);
	}
	printf("\nTotal integers read: %d\n", count);
	str[k] = 'x';
}
int main(int argc, char *argv[]){	



	if (argc != 3) {
		printf("Usage: ./a.out size inputfile");
		exit(-1);    
	}



    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }

    //calculating size of file
	FILE* fp = fopen(argv[2], "r");
    if (fp == NULL) {
        printf("File Not Found!\n");
        return -1;
    }
    fseek(fp, 0L, SEEK_END);
    size_bytes = ftell(fp);
    fclose(fp);
    printf("size in bytes is: %ld\n",size_bytes);

    //sending the size to P2 using message queue

    int msgid = msgget(1122, 0666 | IPC_CREAT);
    message.mesg_type = 1;
    message.val = size_bytes;
    msgsnd(msgid, &message, sizeof(message), 0);




    shmid = shmget(2041, 10000000, 0666 | IPC_CREAT);
    printf("shmid:%d \n", shmid);
    str = (char*) shmat(shmid, 0, 0);
    str[0] = '0';
    
    printf("enter no. of threads: ");
    scanf("%d", &n);
	count = 0;
	clock_t t;
	size = atoi(argv[1]);
	input_file = argv[2];
	struct timeval start,stop;
	double secs = 0;
	gettimeofday(&start, NULL);
	file_reader();
	gettimeofday(&stop, NULL);
	secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
	printf("Total elapsed time: %f\n\n",secs);
		//sleep(1);
   

	return 0;
}