#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/wait.h>
#define max_text 50

//defino los files descriptors para los pipes
int fdCliRec[2];
int fdRecKit[2];
int fdKitCli[2];
char *pedido;

void pedir(){
	pedido = "pedido tomado";
	close(fdCliRec[0]);
	close(fdRecKit[0]);
	close(fdRecKit[1]);
	close(fdKitCli[0]);
	close(fdKitCli[1]);
	
	write(fdCliRec[1], &pedido, strlen(pedido));
	printf("El cliente pidio su comida\n");
}

void tomarPedido(){
	close(fdCliRec[1]);
	close(fdRecKit[0]);
	close(fdRecKit[1]);
	close(fdKitCli[0]);
	close(fdKitCli[1]);
	
	read(fdCliRec[0], pedido, sizeof(pedido));
	printf("Recepcion: Pedido recibido...\n");
}
	
void enviarPedido(){
	close(fdCliRec[0]);
	close(fdCliRec[1]);
	close(fdRecKit[0]);
	close(fdKitCli[0]);
	close(fdKitCli[1]);
	
	write(fdRecKit[1], &pedido, strlen(pedido));
	printf("El recepcionista envio el pedido del cliente a la cocina..\n");
}

void cocinar(){
	close(fdCliRec[0]);
	close(fdCliRec[1]);
	close(fdRecKit[1]);
	close(fdKitCli[0]);
	close(fdKitCli[1]);
	
	read(fdRecKit[0], pedido, sizeof(pedido));
	printf("La cocina esta preparando el pedido..\n");
}

void darComida(){
	close(fdCliRec[0]);
	close(fdCliRec[1]);
	close(fdRecKit[1]);
	close(fdRecKit[0]);
	close(fdKitCli[0]);
	
	write(fdKitCli[1], &pedido, strlen(pedido));
	printf("La cocina tiene listo el pedido del cliente \n");
}

void tomarComida(){
	close(fdCliRec[0]);
	close(fdCliRec[1]);
	close(fdRecKit[1]);
	close(fdRecKit[0]);
	close(fdKitCli[1]);
	
	read(fdKitCli[0], pedido, sizeof(pedido));
	printf("El cliente se va feliz con su comida\n");
}
	
void sincronizar(){
	//falta hacer un sincronizador para que vaya en orden 
}

int main(){
	pipe(fdCliRec);
	pipe(fdRecKit);
	pipe(fdKitCli);
	
	if(fork()==0){
		pedir();
	}
	wait(NULL);
	
	if(fork()==0){
		tomarPedido();
		}
	wait(NULL);
	
	if(fork() == 0){
		enviarPedido();
		}
	wait(NULL);
		
	if(fork()==0){
		cocinar();
	}
	wait(NULL);
	
	if(fork()==0){
		darComida();
	}
	wait(NULL);
	
	if(fork()==0){
		tomarComida();
	}
	wait(NULL);
	 exit(0);
	
	return 0;
	}
