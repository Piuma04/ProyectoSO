#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>
#define BFR_SZ 100; //porque da error este?
   
int main(){
	
	char str[100];
	char *remp, *token, *resto;
	pid_t pid;

	
	do{
		//recibe toda la linea hasta el enter
		printf("Ingrese un comando: ");
		fgets(str,sizeof(str),stdin);
		fflush(stdin);
		
		
		//remplaza el enter que deja fgets al final por el end of character
		remp = strchr(str, '\n');
		if(remp) *remp = '\0';
		
		
		token = strtok(str," "); //obtiene la primera palabra
		resto = strtok(NULL,""); //captura el resto de la cadena
		
		
		if(strcmp(token,"help") == 0) {
			//implementar el help
		}
		//listar directorio (falta hacer para qeu se peuda poner el directorio (?))
		else if(strcmp(token,"ls") == 0) {
			pid = fork();
			if(pid == 0)
			 execlp("ls","ls",NULL);
			wait(NULL);
		}
		//crear directorio
		else if(strcmp(token,"mkdir") == 0){
			pid = fork();
			token = strtok(NULL," ");//capturar si no hay nombre de directorio
			if(pid == 0){
			 execlp("mkdir","mkdir", token ,NULL);
			}
			wait(NULL);
		}
		else{
			printf("\nComando incorecto\n");
			
		}
		
		
		
	}while(strcmp(token,"exit") != 0);
	


 exit(0);
}
