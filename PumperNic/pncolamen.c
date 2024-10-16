#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string.h>
#include <math.h>
#include <errno.h>

#define CLIENTE "\e[1;37m"
#define CLIENTEVIP "\e[1;35m"
#define PAPASCOL "\e[1;33m"
#define BURGUERCOL "\e[1;31m"
#define VEGANOCOL "\e[1;32m"

#define VIP 1
#define NORMAL 2
#define VEGANO 3
#define BURGER 4
#define PAPAS 5
#define VEGANOL 6
#define BURGERL 7
#define PAPASL 8
#define CANT_CLIENTES 4
#define CANT_EMPLEADOS 5

int qid;

struct msgbuf {
	long mtype;
	
	long tipoCliente;
	
	long tipoMenu;


};

const size_t longitud = sizeof(struct msgbuf) - sizeof(long);

void pedir(){
	srand(getpid());
	
	struct msgbuf mOrden,mPedido;
	
	int tipoCliente = (rand() % 2) +1;
	int tipoMenu = (rand() % 3) + 3;
	
	
	mOrden.mtype = tipoCliente;
	mOrden.tipoMenu = tipoMenu; //da un numero random entre 3 y 5, es decir, randomiza el menu
	mOrden.tipoCliente = tipoCliente; //da un valor entre 1 y 2 , es decir, randomiza el hecho de si es vip o no
	
	
	
	if(tipoCliente == VIP){
		printf(CLIENTEVIP"Cliente VIP enviando menu %d\n",mOrden.tipoMenu);
		fflush(stdout);
	}else {
		printf(CLIENTE"Cliente  enviando menu %d\n",mOrden.tipoMenu);
		fflush(stdout);
	}
	
		
	msgsnd(qid,&mOrden,longitud,0);
	
	int menuRec = tipoMenu+3;
	
	msgrcv(qid,&mPedido,longitud,menuRec,0);
	
	
	
	if(tipoCliente == VIP){
		printf(CLIENTEVIP"Cliente VIP que pidio %d recibiendo menu %d\n",tipoMenu,mPedido.tipoMenu);
		fflush(stdout);
	}else{
		printf(CLIENTE"Cliente que pidio %d recibiendo menu %d\n",tipoMenu,mPedido.tipoMenu);
		fflush(stdout);
	}

}
void cliente(){
	
	

	int ganas_de_esperar;
	while(1){
	srand(getpid());
	
		ganas_de_esperar = rand() % 10;
		if(ganas_de_esperar >  0 ){ 
			printf("tuvo ganas de esperar\n");
			fflush(stdout);
			pedir();  
		} //10 % de probabilidades de que tenga ganas de esperar
		else { printf("no quiso esperar\n"); fflush(stdout); }

	sleep(2);

}
	exit(0);
	
	
}

void despachador(){
	struct msgbuf mOrden, mPrep;

	
	while(1){
		
	
		sleep(2);
		msgrcv(qid,&mOrden,longitud,-2,0);
		
		
		
		
		mPrep.mtype = mOrden.tipoMenu;
		mPrep.tipoMenu = mOrden.tipoMenu;
		mPrep.tipoCliente = mOrden.tipoCliente;
		
		
		msgsnd(qid,&mPrep,longitud,0);
		
		
		
	}
	exit(0);
	
	
}

void empHamburguesa(){
	struct msgbuf mOrden, mPedido;

	while(1){
		
		msgrcv(qid,&mOrden,longitud,BURGER,0);
		
		printf(BURGUERCOL"Haciendo hamburguesa para cliente \n");
		fflush(stdout);
	
		
		
		mPedido.mtype = BURGERL;
		mPedido.tipoMenu = BURGER;
		mPedido.tipoCliente = mOrden.tipoCliente;
		
		
		msgsnd(qid,&mPedido,longitud,0);
	
	}
	exit(0);
	
}

void empPapas(int empleado){
	struct msgbuf mOrden, mPedido;
	
	while(1){
		msgrcv(qid,&mOrden,longitud,PAPAS,0);
		
		printf(PAPASCOL"Haciendo papas fritas (empleado %d)  \n",empleado);
		fflush(stdout);
	
		
		
		
		mPedido.mtype = PAPASL;
		mPedido.tipoMenu = PAPAS;
		mPedido.tipoCliente = mOrden.tipoCliente;
		
		msgsnd(qid,&mPedido,longitud,0);
	
	}
	exit(0);
	
	
}

void empVegano(){
	struct msgbuf mOrden, mPedido;
	
	while(1){
		msgrcv(qid,&mOrden,longitud,VEGANO,0);
		
		printf(VEGANOCOL"Haciendo menu vegano \n");
		fflush(stdout);
	
		
		
		
		mPedido.mtype = VEGANOL;
		mPedido.tipoMenu = VEGANO;
		mPedido.tipoCliente = mOrden.tipoCliente;
		
		
		msgsnd(qid,&mPedido,longitud,0);
	
	}
	exit(0);
	
	
}


int main(){
	
	key_t key;
	

	pid_t pidEmpleados[CANT_EMPLEADOS], pidClientes[CANT_CLIENTES];
	key = ftok("/tmp",'e');
	if(key == -1){printf("problemas en el paraiso %s\n",strerror(errno));}
	
	qid = msgget(key, 0666 | IPC_CREAT);
	msgctl(qid,IPC_RMID,NULL);				//esto se hace para reinicia la cola, ya que al tener los ciclos while(1), al finalizar el programa no se borra la cola
	qid = msgget(key, 0666 | IPC_CREAT);
	

	
	
	
	
	
	//despachador
	pidEmpleados[0] = fork();
	
	if(pidEmpleados[0] == 0){
		despachador();
		
	}
	
	//hamburguesa
	pidEmpleados[1] = fork();
	
	if(pidEmpleados[1] == 0){
		empHamburguesa();
		
	}
	
	//papas
	
	
	for(int j = 1; j<3; j++){
		pidEmpleados[j+1] = fork();
		
		if(pidEmpleados[j+1] == 0){
			empPapas(j);
		
		}
	}
	
	// vegano
	pidEmpleados[4]  = fork();
	
	if(pidEmpleados[4] == 0){
		empVegano();
		
	}
	
	for(int j = 0; j<CANT_CLIENTES; j++){
		pidClientes[j] = fork();
		if(pidClientes[j] == 0){
			cliente();
		
		}
	}
	
	
		
	for(int c = 0; c<CANT_CLIENTES; c++){
		int status;
		waitpid(pidClientes[c],&status,0);
	}
	
	for(int e = 0; e<CANT_EMPLEADOS; e++){
		
		kill(pidEmpleados[e],SIGTERM);
	}


	msgctl(qid,IPC_RMID,NULL);

	return 0;
	
}
