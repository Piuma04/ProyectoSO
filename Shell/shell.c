#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#define BFR_SZ 100; //porque da error este?
   
int main(){
	
	int seguir = 1;
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
			printf("help\n");
			printf("ls\n");
			printf("mkdir\n");
			printf("\n");
			printf("\n");
			printf("\n");

		}
		
		//listar directorio (falta hacer para qeu se peuda poner el directorio (?))
		else if(strcmp(token,"ls") == 0) {
			
			pid = fork();
			if(pid == 0){
				char *args[] = {"ls",resto,NULL}; //preguntar si esto es valido
				execvp("./ls",args);
				
			 
			}
			wait(NULL);
		}
		
		//crear directorio
		else if(strcmp(token,"mkdir") == 0){
			
			
			pid = fork();
			if(pid == 0){
				char *args[] = {"mkdir",resto,NULL}; 
				execvp("./mkdir",args);
				
			 
			}
			wait(NULL);
		}
		//mostrar contenido archivo
		else if(strcmp(token,"cat") == 0){
			
			
			pid = fork();
			if(pid == 0){
				char *args[] = {"cat",resto,NULL}; 
				execvp("./cat",args);
				
			 
			}
			wait(NULL);
		}
		//remover directorio
		else if(strcmp(token,"rmdir") == 0){
			
			
			pid = fork();
			if(pid == 0){
				char *args[] = {"rmdir",resto,NULL}; 
				execvp("./rmdir",args);
				
			 
			}
			wait(NULL);
		}
		//cambiar permisos archivo
		else if(strcmp(token,"chmod") == 0){
			
			
			pid = fork();
			if(pid == 0){
				char *args[] = {"chmod",resto,NULL}; 
				execvp("./chmod",args);
				
			 
			}
			wait(NULL);
		}
		//crear archivo
		else if(strcmp(token,"touch") == 0){
			
			
			pid = fork();
			if(pid == 0){
				char *args[] = {"touch",resto,NULL}; 
				execvp("./touch",args);
				
			 
			}
			wait(NULL);
		}
		
		else if(strcmp(token,"exit") == 0){
			seguir = 0;
		}
		
		else{	
			printf("\nComando incorecto\n");
			
		}
		
		
		
	}while(seguir);
	


 exit(0);
}
