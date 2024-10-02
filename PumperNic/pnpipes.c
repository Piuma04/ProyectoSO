#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/wait.h>

//defino los files descriptors para los pipes
int fdCliRec[2];
int fdRecKit[2];
int fdKitCli[2];
char *pedido;

void pedir(){
    char *pedido = "pedido tomado";
    close(fdCliRec[0]);
    write(fdCliRec[1], pedido, strlen(pedido));
    close(fdCliRec[1]); // El cliente terminó
    printf("El cliente pidió su comida \n");
}

void tomarPedido(){
    close(fdCliRec[1]);
    char buffer[100];
    read(fdCliRec[0], buffer, sizeof(buffer));
    printf("Recepcion: Pedido recibido: %s \n", buffer);
    close(fdCliRec[0]); // El recepcionista termina de leer

    // Envía a la cocina
    close(fdRecKit[0]);
    write(fdRecKit[1], buffer, strlen(buffer));
    close(fdRecKit[1]);
    printf("El recepcionista envió el pedido del cliente a la cocina \n");
}

void cocinar(){
    close(fdRecKit[1]);
    char buffer[100];
    read(fdRecKit[0], buffer, sizeof(buffer));
    printf("La cocina está preparando el pedido: %s \n", buffer);
    close(fdRecKit[0]); // La cocina termina de leer

    // Cocina pasa la comida al cliente
    close(fdKitCli[0]);
    write(fdKitCli[1], buffer, strlen(buffer));
    close(fdKitCli[1]);
    printf("La cocina tiene listo el pedido del cliente \n");
}

void tomarComida(){
    close(fdKitCli[1]);
    char buffer[100];
    read(fdKitCli[0], buffer, sizeof(buffer));
    printf("El cliente se va feliz con su comida: %s\n", buffer);
    close(fdKitCli[0]); // El cliente termina de leer
}


int main(){
    pipe(fdCliRec);
    pipe(fdRecKit);
    pipe(fdKitCli);

    if(fork() == 0) {
        pedir();
        exit(0);
    }

    if(fork() == 0) {
        tomarPedido();
        exit(0);
    }

    if(fork() == 0) {
        cocinar();
        exit(0);
    }

    if(fork() == 0) {
        tomarComida();
        exit(0);
    }

    for(int i = 0; i < 4; i++){
        wait(NULL);
    }
    return 0;
}
