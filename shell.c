#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>
#define BFR_SZ 100; //porque da error este?
   
int main(){
	
	char str[100];
	char *remp, *token;
	pid_t pid;

	
	do{
		//recibe toda la linea hasta el enter
		printf("Ingrese un comando: ");
		fgets(str,sizeof(str),stdin);
		fflush(stdin);
		
		//remplaza el entre por el end of character
		remp = strchr(str, '\n');
		if(remp) *remp = '\0';
		
		//obtiene la primera palabra
		token = strtok(str," ");
		
		
		//listar directorio (falta hacer para qeu se peuda poner el directorio (?))
		if(strcmp(token,"ls") == 0) {
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
