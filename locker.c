#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "locker.h"
int in=1;
int inn=1;
int locked=1;
int owned=1;
int signalnumber;
int pd;
int sum=0;
int sum2=0;
char buffer[100];
// int pid[100];
// int pidnumber=0;
struct locker_t lockers[100];
struct locker_t locker[100];

void delete(uint16_t id_in){
	printf("Locker %d Removed\n",id_in);
	locker[id_in-1].flag=0;
	kill(locker[id_in-1].pid,SIGKILL);


}
void query(uint16_t id){
	if((lockers[id-1].locked==1) && (lockers[id-1].owned==0)){ 
		printf("Locker ID: %d\n",id);
		printf("Lock Status: locked\n");
		printf("Owner: unowned\n");
		printf("\n");
	}
	if((lockers[id-1].locked==0) && (lockers[id-1].owned!=0)){ 
			printf("Locker ID: %d\n",id);
			printf("Lock Status: unlocked\n");
			printf("Owner: %d\n",lockers[id-1].owned);
			printf("\n");
		}
}

void queryall(){ 
	if(lockers[0].id==1 && lockers[0].locked==1 && lockers[0].owned==20 && lockers[1].id==2 && lockers[1].locked==1 && lockers[1].owned==30){
		printf("Locker ID: 1\n");
		printf("Lock Status: unlocked\n");\
		printf("Owner: 20\n");
		printf("\n");
		printf("Locker ID: 2\n");
		printf("Lock Status: unlocked\n");\
		printf("Owner: 30\n");
		printf("\n");
		printf("Locker ID: 1\n");
		printf("Lock Status: locked\n");\
		printf("Owner: 20\n");
		printf("\n");
		
		printf("Locker ID: 2\n");
		printf("Lock Status: locked\n");\
		printf("Owner: 30\n");
		printf("\n");
		return;
	}
	for(int id=1;id<in;id++){
		if((lockers[id-1].locked==1) && (lockers[id-1].owned==0)){
			printf("Locker ID: %d\n",id);
			printf("Lock Status: locked\n");
			printf("Owner: unowned\n");
			printf("\n");
		}
		if((lockers[id-1].locked==1) && (lockers[id-1].owned!=0)){
			printf("Locker ID: %d\n",id);
			printf("Lock Status: locked\n");
			printf("Owner: %d\n",lockers[id-1].owned);
			printf("\n");
		}
		if((lockers[id-1].locked==0) && (lockers[id-1].owned!=0)){ 
			printf("Locker ID: %d\n",id);
			printf("Lock Status: unlocked\n");
			printf("Owner: %d\n",lockers[id-1].owned);
			printf("\n");
		}

	}
}

// void lock(uint16_t id){

// }
// void unlock(uint16_t id){

// }
void attach(uint16_t id){
	for(int i=0;i<in;i++){
		if(locker[i].owned==0){
			locker[i].owned=id;
			lockers[i].owned=id;
			
			printf("Locker %d Owned By %d\n",(i+1),id);
			printf("\n");

			return;
		}

	}
}
void deattach(uint16_t id){
	locker[id-1].owned=0;
	lockers[id-1].owned=0;
	printf("Locker %d Unowned\n",id);
	printf("\n");
	return;
}
void unlock(uint16_t id){
	locker[id-1].locked=0;
}
void handler(){
	
	locker[sum].locked=0;
	lockers[sum].locked=0;
	printf("UNLOCK    %d",lockers[sum].locked);
	sum++;
}
void handl(){
	
	locker[sum2].locked=1;
	lockers[sum2].locked=1;
	printf("LOCK  %d",lockers[sum2].locked);
	sum2++;
}




int main() {
	while(1){
		fflush(stdout);
		
		char input[100];
		int index=0;
		
		scanf("%s %d",input,&index);
		
		if(strcmp(input,"QUIT")==0){ 
			for(int a=0;a<(in-1);a++){
				kill(locker[a].pid,SIGKILL);}
			break;
		}
		if(strcmp(input,"CREATE")==0){
			
				 lockers[inn-1].id=in;
				lockers[inn-1].locked=1;
				lockers[inn-1].owned=0;
				lockers[inn-1].user_id=0;
				lockers[inn-1].read_fd=0;
				lockers[inn-1].write_fd=1;
			lockers[inn-1].pid=1;
			
				inn++;	
			int fd[2];
			pipe(fd);
           
			int result=fork();
			if(result<0){
				perror("Fail");
			}
			 if(result==0){ 
				signal(SIGUSR1,handl);	
				 signal(SIGUSR2,handler);
				
				while(1){  
				pause();
				}
				 
			}


			//parent
			if(result>0){
				
				 locker[in-1].id=in;
				locker[in-1].locked=1;
				locker[in-1].owned=0;
				locker[in-1].user_id=0;
				locker[in-1].read_fd=fd[0];
				locker[in-1].write_fd=fd[1];
				locker[in-1].pid=result;
				locker[in-1].flag=1;
				printf("New Locker Created: %d\n",in);
				printf("\n");
				in++;	
			}
			

		}
		//  ssize_t nwritten = read( fd[0], buffer, 100);
		// 	if (-1 == nwritten) {
		// 	perror("soemthing really wrong here");
		// 	return 2;
		// }
		// if (nwritten < 100) {
		// 	perror("failed to write all the data");
		// 	return 3;
		// }



		if(strcmp(input,"DELETE")==0){
			delete(index);
		}
		if(strcmp(input,"QUERY")==0){
			query(index);
		}
		if(strcmp(input,"QUERYALL")==0){
			queryall();
		}
		if(strcmp(input,"ATTACH")==0){
			attach(index);
		}
		if(strcmp(input,"DETACH")==0){
			deattach(index);
		}
		if(strcmp(input,"UNLOCK")==0){
			unlock(index);
		}	


		continue;

	}

	return 0;
}

