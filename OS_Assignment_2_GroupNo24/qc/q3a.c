//schedular
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

pid_t pid, pid1, pid2;
int flag = 1;
int turn = 1;
int totalActive = 2;
void childTerminated(int sig){
	
	pid_t pid;
	pid = wait(NULL);
	if(pid == pid1)
		printf("P1 exited.\n");
	else
		printf("P2 exited.\n");
	if(pid == pid2){
		//all process exited
		flag = 0;
	}
	totalActive -= 1;

}

int main(){
	signal(SIGCHLD, childTerminated);
	pid1 = fork();
	if(pid1 == 0){
		//child process
		// system("./a");
		// static char *args[] = {"./q1", "onemil.txt", 1, NULL}; 
		if(execl("q1", "q1", "hundred.txt", "1", 0)){
			perror("error in execv1");
			exit(1);
		}
	}
	else{
		sleep(1);
		pid2 = fork();
		if(pid2 == 0){
			// static char *args[] = {"./q2", "1", 0};
			if(execl("q2", "q2", "1", 0)){
				perror("error in execv2");
				exit(2);
			}
		}
		else{
			while(flag){
				
				usleep(1);
				// printf("sending continue signal to process a\n");
				if(totalActive == 2){
					if(turn == 1){
						kill(pid1, SIGCONT);
						turn = 2;
					}	
					else{
						kill(pid2, SIGCONT);
						turn = 1;
					}
				}
				else{
					kill(pid2, SIGCONT);
				}
			}
		}
	}
	return 0;
}