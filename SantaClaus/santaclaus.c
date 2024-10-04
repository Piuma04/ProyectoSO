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



//usados para garantizar exclusion mutua con santa en la parte de ver si llenaron el grupo
pthread_mutex_t renos = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t elfos = PTHREAD_MUTEX_INITIALIZER;

//sirve para  despertar a santa
sem_t despertar;  


sem_t otros_elfos; //limita la cantidad de elfos que pueden pedir ayuda


sem_t elf_con_dif ;//contador
sem_t ren_vuelta_vacaciones; //contador

sem_t esperando_santa_elf;//hace que esperen a santa
sem_t esperando_en_cabaña; //hace que esperen a santa

void *elfo(void *arg){
	

	int minum = pthread_self();
	
	int dificultad_al_hacer_juguete;
	for(int h = 0; h<2; h++){
		dificultad_al_hacer_juguete = (int) (rand() % 10 + 1 ); //hay un 10% de prob de necesitar ayuda
		printf("%d\n",dificultad_al_hacer_juguete);
		
		if(dificultad_al_hacer_juguete == 4){  //si necesita ayuda
			
			printf("necesito ayuda! elfo %d\n",minum);
			sem_wait(&otros_elfos);
			printf("entre al grupo! elfo %d\n",minum);
			
			
			pthread_mutex_lock(&elfos);
			
				sem_wait(&elf_con_dif);
				if(sem_trywait(&elf_con_dif) != 0){
					printf("a\n");
					sem_post(&despertar); 		
				}else
				sem_post(&elf_con_dif);
					
			pthread_mutex_unlock(&elfos);
			
			sem_wait(&esperando_santa_elf);

			printf("recibiendo ayuda de santa elfo %d\n",minum);

			pthread_mutex_lock(&elfos);

				sem_wait(&elf_con_dif);
				if(sem_trywait(&elf_con_dif) != 0){ //si soy el ultimo
					for(int i = 3; i>0; i--) {sem_post(&elf_con_dif);}	
					for(int j = 3; j>0; j--) {sem_post(&otros_elfos);}
				}else 
					sem_post(&elf_con_dif);
			pthread_mutex_unlock(&elfos);
			
		}
		printf("hacer juguete elfo %d\n",minum);
	}	
	pthread_exit(0);
}


void *reno(void *arg){
	//sleep((int)(rand() % 10 + 1)); //simula el hecho de estar de vacaciones
	pthread_mutex_lock(&renos);
		sem_wait(&ren_vuelta_vacaciones);
		if( sem_trywait(&ren_vuelta_vacaciones) != 0) {//si es el último
				
				sem_post(&despertar);
				printf("llegue de vacaciones y soy el ultimo! ------------\n");
		}
		
		else {
			
			printf("llegue de vacaciones! --------------\n");
			sem_post(&ren_vuelta_vacaciones);
		}
	pthread_mutex_unlock(&renos);
	sem_wait(&esperando_en_cabaña);
	
	pthread_exit(0);
}

void *santa(void *arg){
	do{
	sem_wait(&despertar);
	
	pthread_mutex_lock(&renos);
        if(sem_trywait(&ren_vuelta_vacaciones) != 0){ //veo si no falta ningun reno
			
				printf("trineo hecho ! ------\n"); 
				for(int i = 0; i <= CANT_RENO; i++) {
					sem_post(&esperando_en_cabaña);
				}
		
		}
		else
			sem_post(&ren_vuelta_vacaciones);
	pthread_mutex_unlock(&renos);
    
    pthread_mutex_lock(&elfos);
		if(sem_trywait(&elf_con_dif) != 0){ //veo si el grupo ya tiene 3 elfos
			for(int j = 3; j>0; j--) {sem_post(&esperando_santa_elf); printf("ayudando a un elfo!\n");}
			for(int i = 3; i>0; i--) {sem_post(&elf_con_dif);}	
		}
		else{
			sem_post(&elf_con_dif);
		}
    pthread_mutex_unlock(&elfos);

    
    }while(1);
	pthread_exit(0);
}

int main(){
	
	
	pthread_t santa_claus, elfos_a_usar[CANT_ELFO], renos_a_usar[CANT_RENO];
	
	
	sem_init(&despertar,0,0);
	sem_init(&otros_elfos,0,3);
	sem_init(&elf_con_dif ,0,3);
	sem_init(&esperando_santa_elf,0,0);
	sem_init(&esperando_en_cabaña,0,0);
	sem_init(&ren_vuelta_vacaciones,0,CANT_RENO);

	
	
	
	pthread_create(&santa_claus,NULL,santa,NULL);
	
	for(int a = 0; a<CANT_ELFO; a++){
	
		pthread_create(&elfos_a_usar[a],NULL,elfo,NULL);
	}
	
	for(int b = 0; b<CANT_RENO; b++){
		pthread_create(&renos_a_usar[b],NULL,reno,NULL);
	}
	

	for(int c = 0; c<CANT_ELFO; c++){
		pthread_join(elfos_a_usar[c],NULL);
	}
	for(int d = 0; d<CANT_RENO; d++){
		pthread_join(renos_a_usar[d],NULL);
	}
	
	pthread_cancel(santa_claus);
	
	sem_close(&despertar );
	sem_close(&otros_elfos );
	sem_close(&elf_con_dif );
	sem_close(&esperando_santa_elf );
	sem_close(&esperando_en_cabaña );
	sem_close(&ren_vuelta_vacaciones );


	return 0;
	
}
