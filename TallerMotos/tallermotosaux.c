#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>


sem_t OP1,OP2,OP3, P, OP4, sec;

void *Rueda(void *arg){
	
	while(1){
		sem_wait(&sec);
		sem_wait(&OP1);
		printf("poniendo rueda \n");
		sem_post(&OP2);
	}
	
	pthread_exit(0);
}

void *Chasis(void *arg){

	while(1){
		sem_wait(&OP2);
		sem_wait(&OP2);
			printf("poniendo chasis\n");
		sem_post(&OP3);
	}
	
	pthread_exit(0);
}

void *Motor(void *arg){

	while(1){
		sem_wait(&OP3);
			printf("poniendo motor\n");
		sem_post(&P);
	}
	
	pthread_exit(0);
}

void *PintarVerde(void *arg){

	while(1){
		sem_wait(&P);
			printf("pintando de verde\n");
		sem_post(&OP4);
		sem_post(&OP1);
		sem_post(&OP1);
	}
	
	pthread_exit(0);
}

void *PintarRojo(void *arg){

	while(1){
		sem_wait(&P);
			printf("pintando de rojo\n");
		sem_post(&OP4);
		sem_post(&OP1);
		sem_post(&OP1);
	}
	
	pthread_exit(0);
}

void *EqExtra(void *arg){

	while(1){
		sem_wait(&OP4);
		sem_wait(&OP4);		
		printf("poniendo eq extra\n");
		printf("\n");
		
		sleep(2);
		sem_post(&sec);
		sem_post(&sec);
		sem_post(&sec);
		sem_post(&sec);
	}
	
	pthread_exit(0);
}

int main(){
	
	
	pthread_t op1, op2, op3,pinturaV,pinturaR, op4;
	
	
	sem_init(&OP1,0,2);
	sem_init(&OP2,0,0);
	sem_init(&OP3,0,0);
	sem_init(&P,0,0);
	sem_init(&OP4,0,0);
	sem_init(&sec,0,4);
	
	
	pthread_create(&op1,NULL,Rueda,NULL);
	pthread_create(&op2,NULL,Chasis,NULL);
	pthread_create(&op3,NULL,Motor,NULL);
	pthread_create(&pinturaV,NULL,PintarVerde,NULL);
	pthread_create(&pinturaR,NULL,PintarRojo,NULL);
	pthread_create(&op4,NULL,EqExtra,NULL);
	

	pthread_join(op1,NULL);
	pthread_join(op2,NULL);
	pthread_join(op3,NULL);
	pthread_join(op4,NULL);
	pthread_join(pinturaV,NULL);
	pthread_join(pinturaR,NULL);

	
	
	sem_close(&OP1);
	sem_close(&OP2);
	sem_close(&OP3);
	sem_close(&P);
	sem_close(&OP4);
	sem_close(&sec);
	

	return 0;
	
}

