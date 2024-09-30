#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>
#define CANT_ELFO 3
#define CANT_RENO 4

sem_t despertar;
sem_t otros_elfos;
pthread_mutex_t renos = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t elfos = PTHREAD_MUTEX_INITIALIZER;
sem_t elf_con_dif ;//contador
sem_t esperando_santa_elf ;//hace que esperen a santa
sem_t ren_vuelta_vacaciones; //contador
sem_t esperando_en_cabaña; //hace que esperen a santa

void *elfo(void *arg){
	int dificultad_al_hacer_juguete;
	for(int i = 0; i<2; i++){
		dificultad_al_hacer_juguete = (int) (1); //hay un 10% de prob de necesitar ayuda
		if(dificultad_al_hacer_juguete){  
			printf("necesito ayuda!\n");
			sem_wait(&otros_elfos);
			printf("entre al grupo! \n");
			pthread_mutex_lock(&elfos);
				if( sem_trywait(&elf_con_dif) != 0)
					sem_post(&despertar); 	
			pthread_mutex_unlock(&elfos);
			sem_wait(&esperando_santa_elf);

			printf("recibiendo ayuda de santa \n");

			pthread_mutex_lock(&elfos);
				if(sem_trywait(&elf_con_dif) != 0){
					for(int i = 2; i>0; i--) {sem_post(&elf_con_dif);}	
					for(int j = 3; j>0; j--) {sem_post(&otros_elfos);}
				}
				printf("recibí ayuda de santa !\n");
			pthread_mutex_unlock(&elfos);
			printf("juguete creado !\n");
		}
	}	
	pthread_exit(0);
}


void *reno(void *arg){
	//sleep((int)(rand() % 10 + 1));
	pthread_mutex_lock(&renos);
	
		if( sem_trywait(&ren_vuelta_vacaciones) != 0) {//si es el último
				sem_post(&despertar);
				printf("llegue de vacaciones y soy el ultimo!\n");
		}
		else printf("llegue de vacaciones!\n");
	pthread_mutex_unlock(&renos);
	//sem_wait(&esperando_en_cabaña);
	pthread_exit(0);
}

void *santa(void *arg){
	do{
	sem_wait(&despertar);
	pthread_mutex_lock(&renos);
        if(sem_trywait(&ren_vuelta_vacaciones) == 0) //veo si estan los renos
			sem_post(&ren_vuelta_vacaciones); //si no estan, devuelvo el turno que tome	
		else{
			printf("trineo hecho ! \n"); //si no hay lugares libres, preparo el trineo
			sem_post(&ren_vuelta_vacaciones);
		}
	pthread_mutex_unlock(&renos);
    pthread_mutex_lock(&elfos);
		if(sem_trywait(&elf_con_dif) == 0) //veo si hay elfos
			sem_post(&elf_con_dif); //si no hay, devuelvo el turno que tome	
		else{
			for(int j = 3; j>0; j--) {sem_post(&esperando_santa_elf); printf("ayudando a un elfo!\n");}
			for(int i = 2; i>0; i--) {sem_post(&elf_con_dif);}	
		}
    pthread_mutex_unlock(&elfos);

    
    }while(1);
	pthread_exit(0);
}

int main(){
	
	
	pthread_t santa_claus, elfos_a_usar[CANT_ELFO], renos_a_usar[CANT_RENO];
	
	
	sem_init(&despertar,0,0);
	sem_init(&otros_elfos,0,3);
	sem_init(&elf_con_dif ,0,2);
	sem_init(&esperando_santa_elf,0,0);
	sem_init(&esperando_en_cabaña,0,0);
	sem_init(&ren_vuelta_vacaciones,0,CANT_RENO-1);

	
	
	
	pthread_create(&santa_claus,NULL,santa,NULL);
	
	for(int i = 0; i<CANT_ELFO; i++){
	pthread_create(&elfos_a_usar[i],NULL,elfo,NULL);
	}
	for(int i = 0; i<CANT_RENO; i++){
	pthread_create(&renos_a_usar[i],NULL,reno,NULL);
	}
	

	for(int i = 0; i<CANT_ELFO; i++){
	pthread_join(elfos_a_usar[i],NULL);
	}
	for(int i = 0; i<CANT_RENO; i++){
	pthread_join(renos_a_usar[i],NULL);
	}
	
	pthread_cancel(santa_claus);
	


	return 0;
	
}
