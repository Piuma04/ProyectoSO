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
#include <fcntl.h>
#include <semaphore.h>
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
#define CANT_CLIENTES 5
#define CANT_EMPLEADOS 5 

int pipeBurguer[2], pipePapas[2], pipeVegano[2];
int pipeClVip[2], pipeCl[2];
int pipeEntrega[2];
int pipeConfEntreg[2];
const char *sem_name = "/mi_sem";
const char *sem_name_2 = "/mi_sem_2";
sem_t *semEntrega;
sem_t *semRecogida;	
	

struct msgbuf {

	
	long tipoCliente;
	
	long tipoMenu;
	
	long nroCliente;


};

const size_t pedido = sizeof(struct msgbuf);

void cliente(){
	
	//cierro pipes que no uso
	close(pipeCl[0]);
	close(pipeClVip[0]);
	close(pipeBurguer[1]);
	close(pipeBurguer[0]);
	close(pipeVegano[0]);
	close(pipeVegano[1]);
	close(pipePapas[0]);
	close(pipePapas[1]);
	close(pipeEntrega[1]);
	
	
	srand(getpid());
	struct msgbuf mOrden,mPedido;
	int tipoCliente = rand() %2 +1;
	int tipoMenu = rand() %3 +3;
	int nroCliente = getpid();
	mOrden.tipoMenu = tipoMenu; //da un numero random entre 3 y 5, es decir, randomiza el menu
	mOrden.tipoCliente = tipoCliente; //da un valor entre 0 y 1 , es decir, randomiza el hecho de si es vip o no
	mOrden.nroCliente = nroCliente;
	
	
	//seccion de pedido
	if(tipoCliente == VIP){
		close(pipeCl[1]);
		
		write(pipeClVip[1], &mOrden, pedido);
		printf(CLIENTEVIP"Cliente VIP %d enviando menu %d\n",mOrden.nroCliente,mOrden.tipoMenu);
		fflush(stdout);
	}
		
	else {
		close(pipeClVip[1]);
		write(pipeCl[1], &mOrden, pedido);
		printf(CLIENTE"Cliente %d enviando menu %d\n",mOrden.nroCliente,mOrden.tipoMenu);
		fflush(stdout);
		
	}

	int obtenido = 0;	

	
	sleep(2);

		while(!obtenido){
			
			
			sem_wait(semRecogida);
			
			if(read(pipeEntrega[0], &mPedido, pedido) > 0 && mPedido.nroCliente == nroCliente) {  obtenido  = 1;}
				
			write(pipeConfEntreg[1],&obtenido,sizeof(int));
			
			sem_post(semRecogida);
			
		}
	
	
	if(tipoCliente == VIP)
		printf(CLIENTEVIP"Cliente VIP %d recibiendo menu %d, el cliente es %d\n",mPedido.nroCliente,mPedido.tipoMenu,getpid());
	else
		printf(CLIENTE"Cliente %d recibiendo menu %d, el cliente es %d\n",mPedido.nroCliente,mPedido.tipoMenu,getpid());
		
	fflush(stdout);	
	
	exit(0);
	
	
}

void despachador(){
	//cierro pipes que no uso
	close(pipeCl[1]);
	close(pipeClVip[1]);
	close(pipeBurguer[0]);
	close(pipeVegano[0]);
	close(pipePapas[0]);
	close(pipeEntrega[1]);
	close(pipeEntrega[0]);
	
	struct msgbuf mOrden, mPrep;

	int recibioPedido;
	while(1){
		
		recibioPedido = 0;
		if(read(pipeClVip[0],&mOrden,pedido) == -1){
				if(read(pipeCl[0],&mOrden,pedido) != -1) {recibioPedido = 1; }
		}
		else recibioPedido = 1; 
		
		if(recibioPedido){
			
			mPrep.tipoMenu = mOrden.tipoMenu;
			mPrep.tipoCliente = mOrden.tipoCliente;
			mPrep.nroCliente = mOrden.nroCliente;
			
			switch(mOrden.tipoMenu){
	
			case PAPAS:
				write(pipePapas[1], &mPrep, pedido);
				break;
			case BURGER:
				write(pipeBurguer[1], &mPrep, pedido);
				break;
			case VEGANO:
				write(pipeVegano[1], &mPrep, pedido);
				break;
			default: 
				("error, despachador recibio un menu no existente \n");
				break;
			}
		
		}
		
		
	}
	exit(0);
	
	
}

void empHamburguesa(){
	//cierro pipes que no uso
	close(pipeCl[0]);
	close(pipeClVip[0]);
	close(pipeCl[1]);
	close(pipeClVip[1]);
	close(pipeBurguer[1]);
	close(pipeVegano[1]);
	close(pipePapas[1]);
	close(pipeVegano[0]);
	close(pipePapas[0]);
	close(pipeEntrega[0]);
	
	
	struct msgbuf mOrden, mPedido;
	
	while(1){
		read(pipeBurguer[0],&mOrden,pedido);
		
		printf(BURGUERCOL"Haciendo hamburguesa para cliente %d\n",mOrden.nroCliente);
		fflush(stdout);
		
		
	sleep(2);
		mPedido.tipoMenu = BURGER;
		mPedido.tipoCliente = mOrden.tipoCliente;
		mPedido.nroCliente = mOrden.nroCliente;
		
		int pudeEntregar = 0;
		sem_wait(semEntrega);
		while(!pudeEntregar){
			
			write(pipeEntrega[1],&mPedido,pedido);
			read(pipeConfEntreg[0],&pudeEntregar,sizeof(int));
		}
		
		sem_post(semEntrega);
	
	}
	exit(0);
	
}

//FIJARSE SI HAY QUE SINCRONIZARLO CON SEMAFOROS
void empPapas(int empleado){
	
	//cierro pipes que no uso
	close(pipeCl[0]);
	close(pipeClVip[0]);
	close(pipeCl[1]);
	close(pipeClVip[1]);
	close(pipeBurguer[1]);
	close(pipeVegano[1]);
	close(pipePapas[1]);
	close(pipeVegano[0]);
	close(pipeBurguer[0]);
	
	
	struct msgbuf mOrden, mPedido;
	
	while(1){
		read(pipePapas[0],&mOrden,pedido);
		
		printf(PAPASCOL"Haciendo papas fritas (empleado %d) para cliente %d\n",empleado,mOrden.nroCliente);
		fflush(stdout);
		
		sleep(2);
		mPedido.tipoMenu = PAPAS;
		mPedido.tipoCliente = mOrden.tipoCliente;
		mPedido.nroCliente = mOrden.nroCliente;
		
		
		int pudeEntregar = 0;
		sem_wait(semEntrega);
		while(!pudeEntregar){
			
			write(pipeEntrega[1],&mPedido,pedido);
			read(pipeConfEntreg[0],&pudeEntregar,sizeof(int));
		}
		
		sem_post(semEntrega);
	
	}
	exit(0);
	
	
}

void empVegano(){
	//cierro pipes que no uso
	close(pipeCl[0]);
	close(pipeClVip[0]);
	close(pipeCl[1]);
	close(pipeClVip[1]);
	close(pipeBurguer[1]);
	close(pipeVegano[1]);
	close(pipePapas[1]);
	close(pipePapas[0]);
	close(pipeBurguer[0]);
	close(pipeEntrega[0]);
	
	struct msgbuf mOrden, mPedido;
	
	while(1){
		read(pipeVegano[0],&mOrden,pedido);
		
		printf(VEGANOCOL"Haciendo menu vegano para cliente %d\n",mOrden.nroCliente);
		fflush(stdout);
		
		
		sleep(2);
		mPedido.tipoMenu = VEGANO;
		mPedido.tipoCliente = mOrden.tipoCliente;
		mPedido.nroCliente = mOrden.nroCliente;
		
		int pudeEntregar = 0;
		sem_wait(semEntrega);
		while(!pudeEntregar){
			
			write(pipeEntrega[1],&mPedido,pedido);
			read(pipeConfEntreg[0],&pudeEntregar,sizeof(int));
		}
		
		sem_post(semEntrega);
		
	}
	exit(0);
	
	
}


int main(){
	
	

	
	pid_t pidEmpleados[CANT_EMPLEADOS], pidClientes[CANT_CLIENTES];
	semEntrega = sem_open(sem_name, O_CREAT , 0644, 1); //inicia con 1, es decir,el primer wait no importa
	semRecogida  = sem_open(sem_name_2, O_CREAT , 0644,1);
	if(semEntrega == SEM_FAILED || semRecogida == SEM_FAILED) printf("problemas en el paraiso %s\n",strerror(errno));
	if(pipe(pipeBurguer) == -1){ return 1; }
	
	if(pipe(pipePapas) == -1){ return 1; }
	
    if(pipe(pipeVegano) == -1){ return 1; }
    
    if(pipe(pipeEntrega) == -1){ return 1; }
    
    if(pipe(pipeCl) == -1){ return 1; }
    
    if(pipe(pipeClVip) == -1){ return 1; }
    
    if(pipe(pipeConfEntreg) == -1){ return 1; }
	
	fcntl(pipeCl[0], F_SETFL,O_NONBLOCK);
    fcntl(pipeClVip[0], F_SETFL,O_NONBLOCK);
    
	
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
		
		if(pidEmpleados[j+1] == 0 == 0){
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

	sem_close(semEntrega);
	sem_close(semRecogida);
	sem_unlink(sem_name); 
	printf(" %s\n",strerror(errno));
	sem_unlink(sem_name_2);
	printf(" %s\n",strerror(errno));
	

	return 0;
	
}
