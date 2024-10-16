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
#include <signal.h>
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
#define MUCHOS_CLIENTES 3

int pipeBurguer[2], pipePapas[2], pipeVegano[2];
int pipeClVip[2], pipeCl[2];
int pipeEntregaB[2], pipeEntregaP[2], pipeEntregaV[2];
int pipeClientesEnLocal[2];


struct msgbuf {
    long tipoCliente;
    long tipoMenu;
};

const size_t pedido = sizeof(struct msgbuf);


void pedir(){

    
    srand(getpid());
    struct msgbuf mOrden, mPedido;
    int tipoCliente = rand() % 2 + 1;
    int tipoMenu = rand() % 3 + 3;
    mOrden.tipoMenu = tipoMenu;
    mOrden.tipoCliente = tipoCliente;

    if (tipoCliente == VIP) {
        close(pipeCl[1]);
        write(pipeClVip[1], &mOrden, pedido);
        printf(CLIENTEVIP"Cliente VIP enviando menu %d\n", mOrden.tipoMenu);
        fflush(stdout);
    } else {
        close(pipeClVip[1]);
        write(pipeCl[1], &mOrden, pedido);
        printf(CLIENTE"Cliente enviando menu %d\n", mOrden.tipoMenu);
        fflush(stdout);
    }
  

    sleep(2);
    
    switch(mOrden.tipoMenu){
        case PAPAS:
            read(pipeEntregaP[0], &mPedido, pedido);
            break;
            
        case BURGER:
            read(pipeEntregaB[0], &mPedido, pedido);
            break;
            
        case VEGANO:
            read(pipeEntregaV[0], &mPedido, pedido);
            break;
        }
    
    if (tipoCliente == VIP) {
        printf(CLIENTEVIP"Cliente VIP recibiendo menu %d\n", mPedido.tipoMenu);
        fflush(stdout);
    } else {
        printf(CLIENTE"Cliente recibiendo menu %d\n", mPedido.tipoMenu);
        fflush(stdout);
    }


}

void cliente(){
    // Cerramos los pipes que no necesitamos
    close(pipeCl[0]);
    close(pipeClVip[0]);
    close(pipeBurguer[0]);
    close(pipeBurguer[1]);
    close(pipeVegano[0]);
    close(pipeVegano[1]);
    close(pipePapas[0]);
    close(pipePapas[1]);
    close(pipeEntregaB[1]);
    close(pipeEntregaP[1]);
    close(pipeEntregaV[1]);
    
    srand(getpid());
	struct msgbuf hayClientes; 
    int ganas_de_esperar;
	if(read(pipeClientesEnLocal[0], &hayClientes, pedido)== -1){  //es decir, si hay mucha gente
		ganas_de_esperar = rand() % 10;
		if(ganas_de_esperar == 1){ 
			printf("tuvo ganas de esperar\n");
			fflush(stdout);
			pedir();  
		} //10 % de probabilidades de que tenga ganas de esperar
		else { printf("no quiso esperar\n"); fflush(stdout); }
	}
	else{
		pedir();
		write(pipeClientesEnLocal[1], &hayClientes, pedido);
	}
	sleep(2);
    
    
    exit(0);
}

void despachador(){
    close(pipeCl[1]);
    close(pipeClVip[1]);
    close(pipeBurguer[0]);
    close(pipeVegano[0]);
    close(pipePapas[0]);
    close(pipeEntregaB[0]);
    close(pipeEntregaB[1]);
    close(pipeEntregaP[0]);
    close(pipeEntregaP[1]);
    close(pipeEntregaV[0]);
    close(pipeEntregaV[1]);
    close(pipeClientesEnLocal[0]);
    close(pipeClientesEnLocal[1]);
    
    struct msgbuf mOrden, mPrep;

    int recibioPedido;
    while(1){
        recibioPedido = 0;
        if(read(pipeClVip[0], &mOrden, pedido) == -1){
            if(read(pipeCl[0], &mOrden, pedido) != -1) {
                recibioPedido = 1;
            }
        } else {
            recibioPedido = 1;
        }

        if(recibioPedido){
            mPrep.tipoMenu = mOrden.tipoMenu;
            mPrep.tipoCliente = mOrden.tipoCliente;

            switch(mOrden.tipoMenu){
                case PAPAS:
                {
                    write(pipePapas[1], &mPrep, pedido);
                    break;
                }
                case BURGER:
                {
                    write(pipeBurguer[1], &mPrep, pedido);
                    break;
                }
                case VEGANO:
                {
                    write(pipeVegano[1], &mPrep, pedido);
                    break;
                }
                default: 
                    printf("error, despachador recibio un menu no existente \n");
                    break;
            }
            recibioPedido = 0;
        }
    }
    exit(0);
}

void empHamburguesa(){
    close(pipeCl[0]);
    close(pipeCl[1]);
    close(pipeBurguer[1]);
    close(pipeClVip[0]);
    close(pipeClVip[1]);
    close(pipeVegano[0]);
    close(pipeVegano[1]);
    close(pipePapas[0]);
    close(pipePapas[1]);
    close(pipeEntregaB[0]);
    close(pipeEntregaP[0]);
    close(pipeEntregaP[1]);
    close(pipeEntregaV[0]);
    close(pipeEntregaV[1]);
    close(pipeClientesEnLocal[0]);
    close(pipeClientesEnLocal[1]); 

    struct msgbuf mOrden, mPedido;

    while(1){
        read(pipeBurguer[0], &mOrden, pedido);
        
        printf(BURGUERCOL"Haciendo hamburguesa \n");
        fflush(stdout);

        sleep(2);

        mPedido.tipoMenu = BURGER;
        mPedido.tipoCliente = mOrden.tipoCliente;
        write(pipeEntregaB[1], &mPedido, pedido);
    }
    exit(0);
}

void empPapas(int empleado){
    close(pipeCl[0]);
    close(pipeClVip[0]);
    close(pipeCl[1]);
    close(pipeClVip[1]);
    close(pipeBurguer[0]);
    close(pipeBurguer[1]);
    close(pipePapas[1]);
    close(pipeVegano[1]);
    close(pipeVegano[0]);
    close(pipeEntregaB[0]);
    close(pipeEntregaB[1]);
    close(pipeEntregaP[0]);
    close(pipeEntregaV[0]);
    close(pipeEntregaV[1]);
    close(pipeClientesEnLocal[0]);
    close(pipeClientesEnLocal[1]);

    struct msgbuf mOrden, mPedido;

    while(1){
        read(pipePapas[0], &mOrden, pedido);

        printf(PAPASCOL"Haciendo papas fritas (empleado %d)\n", empleado);
        fflush(stdout);

        sleep(2);
        mPedido.tipoMenu = PAPAS;
        mPedido.tipoCliente = mOrden.tipoCliente;

        write(pipeEntregaP[1], &mPedido, pedido);
    }
    exit(0);
}

void empVegano(){
    close(pipeCl[0]);
    close(pipeClVip[0]);
    close(pipeCl[1]);
    close(pipeClVip[1]);
    close(pipeBurguer[0]);
    close(pipeBurguer[1]);
    close(pipePapas[0]);
    close(pipePapas[1]);
    close(pipeVegano[1]);
    close(pipeEntregaB[0]);
    close(pipeEntregaB[1]);
    close(pipeEntregaP[0]);
    close(pipeEntregaP[1]);
    close(pipeEntregaV[0]);
    close(pipeClientesEnLocal[0]);
    close(pipeClientesEnLocal[1]);

    struct msgbuf mOrden, mPedido;

    while(1){
        read(pipeVegano[0], &mOrden, pedido);

        printf(VEGANOCOL"Haciendo menu vegano \n");
        fflush(stdout);

        sleep(2);
        mPedido.tipoMenu = VEGANO;
        mPedido.tipoCliente = mOrden.tipoCliente;

        write(pipeEntregaV[1], &mPedido, pedido);
    }
    exit(0);
}

int main(){
    pid_t pidEmpleados[CANT_EMPLEADOS], pidClientes[CANT_CLIENTES];

    if(pipe(pipeBurguer) == -1){ return 1; }
    if(pipe(pipePapas) == -1){ return 1; }
    if(pipe(pipeVegano) == -1){ return 1; }
    if(pipe(pipeCl) == -1){ return 1; }
    if(pipe(pipeClVip) == -1){ return 1; }
    if(pipe(pipeEntregaB) == -1){ return -1; }
    if(pipe(pipeEntregaP) == -1){ return -1; }
    if(pipe(pipeEntregaV) == -1){ return -1; }
    if(pipe(pipeClientesEnLocal) == -1){ return -1; }

    fcntl(pipeCl[0], F_SETFL, O_NONBLOCK);
    fcntl(pipeClVip[0], F_SETFL, O_NONBLOCK);
    fcntl(pipeClientesEnLocal[0], F_SETFL, O_NONBLOCK);


    struct msgbuf muchosClientes;

    for(int e = 0; e<MUCHOS_CLIENTES; e++){
		
		write(pipeClientesEnLocal[1],&muchosClientes,pedido);
	}
	
        
    for(int j = 0; j < CANT_CLIENTES; j++){
        pidClientes[j] = fork();
        if(pidClientes[j] == 0){
            cliente();
        }
    }

    pidEmpleados[0] = fork();
    if(pidEmpleados[0] == 0){
        despachador();
    }

    pidEmpleados[1] = fork();
    if(pidEmpleados[1] == 0){
        empHamburguesa();
    }

    for(int j = 1; j < 3; j++){
        pidEmpleados[j + 1] = fork();
        if(pidEmpleados[j + 1] == 0){
            empPapas(j);
        }
    }

    pidEmpleados[4]  = fork();
    if(pidEmpleados[4] == 0){
        empVegano();
    }

    for(int c = 0; c < CANT_CLIENTES; c++){
        int status;
        waitpid(pidClientes[c], &status, 0);
    }

    for(int e = 0; e < CANT_EMPLEADOS; e++){
        kill(pidEmpleados[e], SIGTERM);
    }

    printf(" %s\n", strerror(errno));
    return 0;
}
