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
#define CANT_CLIENTES 2
#define CANT_EMPLEADOS 5 

int pipeBurguer[2], pipePapas[2], pipeVegano[2];
int pipeClVip[2], pipeCl[2];
int pipeEntregaBurger[2], pipeEntregaPapas[2], pipeEntregaVegano[2];

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
	close(pipeEntregaBurger[1]);
	close(pipeEntregaPapas[1]);
	close(pipeEntregaVegano[1]);
	
	
	srand(getpid());
	struct msgbuf mOrden,mPedido;
	int tipoCliente = (rand() % 2) +1;
	int tipoMenu = PAPAS;
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
	//seccion de retiro
	switch(tipoMenu){
	
	case PAPAS:
		while(!obtenido){
			if(read(pipeEntregaPapas[0], &mPedido, pedido) > 0 && mPedido.nroCliente == nroCliente) obtenido  = 1;
			
		}
		break;
	case BURGER:
		while(!obtenido){
			if(read(pipeEntregaPapas[0], &mPedido, pedido) > 0 && mPedido.nroCliente == nroCliente) obtenido  = 1;
			
		}
		read(pipeEntregaBurger[0], &mPedido, pedido);
		break;
	case VEGANO:
		while(!obtenido){
			if(read(pipeEntregaPapas[0], &mPedido, pedido) > 0 && mPedido.nroCliente == nroCliente) obtenido  = 1;
			
		}
		read(pipeEntregaVegano[0], &mPedido, pedido);
		break;
	default: 
		("error, recibio un menu no existente \n");
		break;
	}
	
	if(tipoCliente == VIP)
		printf(CLIENTEVIP"Cliente VIP %d recibiendo menu %d, el cliente es %d\n",mPedido.nroCliente,mPedido.tipoMenu,getpid());
	else
		printf(CLIENTE"Cliente %d recibiendo menu %d, el cliente es %d\n",mPedido.nroCliente,mPedido.tipoMenu,getpid());
		
	fflush(stdout);	
	sleep(5);
	exit(0);
	
	
}

void despachador(){
	//cierro pipes que no uso
	close(pipeCl[1]);
	close(pipeClVip[1]);
	close(pipeBurguer[0]);
	close(pipeVegano[0]);
	close(pipePapas[0]);
	close(pipeEntregaBurger[1]);
	close(pipeEntregaPapas[1]);
	close(pipeEntregaVegano[1]);
	close(pipeEntregaBurger[0]);
	close(pipeEntregaPapas[0]);
	close(pipeEntregaVegano[0]);
	
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
		
		sleep(2);
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
	close(pipeEntregaPapas[1]);
	close(pipeEntregaVegano[1]);
	close(pipeEntregaPapas[0]);
	close(pipeEntregaVegano[0]);
	
	
	struct msgbuf mOrden, mPedido;
	
	while(1){
		read(pipeBurguer[0],&mOrden,pedido);
		
		printf(BURGUERCOL"Haciendo hamburguesa\n");
		fflush(stdout);
		sleep(2);
		
	
		mPedido.tipoMenu = BURGER;
		mPedido.tipoCliente = mOrden.tipoCliente;
		mPedido.nroCliente = mOrden.nroCliente;
		
		write(pipeEntregaBurger[1],&mPedido,pedido);
	
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
	close(pipeEntregaBurger[1]);
	close(pipeEntregaVegano[1]);
	close(pipeEntregaBurger[0]);
	close(pipeEntregaVegano[0]);
	
	struct msgbuf mOrden, mPedido;
	
	while(1){
		read(pipePapas[0],&mOrden,pedido);
		
		printf(PAPASCOL"Haciendo papas fritas (empleado %d) para cliente %d\n",empleado,mOrden.nroCliente);
		fflush(stdout);
		sleep(2);
		
		mPedido.tipoMenu = PAPAS;
		mPedido.tipoCliente = mOrden.tipoCliente;
		mPedido.nroCliente = mOrden.nroCliente;
		
		write(pipeEntregaPapas[1],&mPedido,pedido);
	
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
	close(pipeEntregaBurger[1]);
	close(pipeEntregaPapas[1]);
	close(pipeEntregaBurger[0]);
	close(pipeEntregaPapas[0]);
	
	struct msgbuf mOrden, mPedido;
	
	while(1){
		read(pipeVegano[0],&mOrden,pedido);
		
		printf(VEGANOCOL"Haciendo menu vegano\n");
		fflush(stdout);
		sleep(2);
		
		
		mPedido.tipoMenu = VEGANO;
		mPedido.tipoCliente = mOrden.tipoCliente;
		mPedido.nroCliente = mOrden.nroCliente;
		
		write(pipeEntregaVegano[1],&mPedido,pedido);
	
	}
	exit(0);
	
	
}


int main(){
	
	pid_t pidEmpleados[CANT_EMPLEADOS], pidClientes[CANT_CLIENTES];

	
	
	if(pipe(pipeBurguer) == -1){ return 1; }
	
	if(pipe(pipePapas) == -1){ return 1; }
	
    if(pipe(pipeVegano) == -1){ return 1; }
    
    if(pipe(pipeEntregaBurger) == -1){ return 1; }
    
    if(pipe(pipeEntregaPapas) == -1){ return 1; }
    
    if(pipe(pipeEntregaVegano) == -1){ return 1; }
    
    if(pipe(pipeCl) == -1){ return 1; }
    
    if(pipe(pipeClVip) == -1){ return 1; }
	
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


	

	return 0;
	
}
