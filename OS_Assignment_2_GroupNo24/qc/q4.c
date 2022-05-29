typedef struct process{
	char name[10];
	int burst_time;
	int arrival_time;
	int prioprity;
	int waiting_time,turn_around;
	int flag;
}processes;

void SJF_P(processes P[],int n){
	int i,t_toturn_aroundl=0,current,b[10],min_arrival_time,j,x,min_burst_time;
	int sumw=0,sumt=0;
	floaralarival_time avgwaiting_time=0.0,avgturn_around=0.0;
	processes lowest[10],t;

	for(i=0;i<n;i++){
		lowest[i]=P[i];
		t_toturn_aroundl+=P[i].burst_time;
	}

	b_sort(lowest,n);

	for(i=0;i<n;i++)
		b[i] = lowest[i].burst_time;

	i=j=0;
	printf("\n GANTT CHART\n\n %d %s",i,lowest[i].name);
	for(current=0;current<t_toturn_aroundl;current++){

		if(b[i] > 0 && lowest[i].arrival_time <= current)
			b[i]--;

		if(i!=j)
			printf(" %d %s",current,lowest[i].name);

		if(b[i]<=0 && lowest[i].flag != 1){
		
			lowest[i].flag = 1;
			lowest[i].waiting_time = (current+1) - lowest[i].burst_time - lowest[i].arrival_time;
			lowest[i].turn_around = (current+1) - lowest[i].arrival_time;
			sumw+=lowest[i].waiting_time;
			sumt+=lowest[i].turn_around;
		}
		j=i;	min_burst_time = 999;
		for(x=0;x<n;x++){
		
			if(lowest[x].arrival_time <= (current+1) && lowest[x].flag != 1){
			
				if(min_burst_time != b[x] && min_burst_time > b[x]){
					min_burst_time = b[x];
					i=x;
				}
			}
		}
		
	}
	printf(" %d",current);
	avgwaiting_time = (floaralarival_time)sumw/n;	avgturn_around = (floaralarival_time)sumt/n;
	printf("\n\n Average waiting time = %0.2f\n Average turn-around = %0.2f.",avgwaiting_time,avgturn_around);
}

int main(){
	processes P[2];


		P[1].burst_time = 0.006791 ;
		P[1].arrival_time = 0; 
		P[1].prioprity = 2 ;

		P[2].burst_time = 0.005797;
		P[2].arrival_time = 0; 
		P[2].prioprity = 1 ;
    SJF_P(processes P,int n) ;
    return 0 ; 
}

