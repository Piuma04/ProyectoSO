#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#define CANTIDAD_MOTOS 5




sem_t OP1,OP2,OP3, P, OP4;

void *Rueda(void *arg){
	int rueda_a_poner;
	for(int i = 0; i<CANTIDAD_MOTOS*2; i++){
		sem_wait(&OP1);
			rueda_a_poner = (i % 2) + 1;
			printf("poniendo rueda %d\n",rueda_a_poner);
		sem_post(&OP2);
	}
	pthread_exit(0);
}

void *Chasis(void *arg){

	for(int i = 0; i <CANTIDAD_MOTOS; i++){
		sem_wait(&OP2);
		sem_wait(&OP2);
			printf("poniendo chasis\n");
		sem_post(&OP3);
	}
	
	pthread_exit(0);
}

void *Motor(void *arg){

	for(int i = 0; i <CANTIDAD_MOTOS; i++){
		sem_wait(&OP3);
			printf("poniendo motor\n");
		sem_post(&P);
	}
	
	pthread_exit(0);
}

void *PintarVerde(void *arg){

	for(int i = 0; i <CANTIDAD_MOTOS; i++){
		sem_wait(&P);
			printf("pintando de verde\n");
		sem_post(&OP4);
	}
	
	pthread_exit(0);
}

void *PintarRojo(void *arg){

	for(int i = 0; i <CANTIDAD_MOTOS; i++){
		sem_wait(&P);
			printf("pintando de rojo\n");
		sem_post(&OP4);
	}
	
	pthread_exit(0);
}

void *EqExtra(void *arg){

	for(int i = 0; i <CANTIDAD_MOTOS; i++){
		sem_wait(&OP4);
		if(i%2 == 0) //es decir, si i es par (pasa 1 de cada 2 veces) recibe el equipamiento
			printf("poniendo eq extra\n");
		printf("\n");
		sem_post(&OP1);
		sem_post(&OP1);
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
	
	pthread_cancel(pinturaV);
	pthread_cancel(pinturaR);

	sem_close(&OP1 );
	sem_close(&OP2 );
	sem_close(&OP3);
	sem_close(&P );
	sem_close(&OP4 );
	

	return 0;
	
}
