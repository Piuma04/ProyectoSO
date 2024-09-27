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
			printf("help (muestra los comandos)\n");
			printf("ls\n");
			printf("mkdir (crea un directorio vacio)\n");
			printf("rmdir (remueve el directorio,el mismo debe estar vacio)\n");
			printf("touch (crea un archivo nuevo si no existe)\n");
			printf("\n");
			printf("\n");
		}
		else if(strcmp(token,"exit") != 0) {
			
			pid = fork();
			
			if(pid == 0){
				char para_dir[] = "./"; 
				strcat(para_dir,token);
				char *args[] = {token,resto,NULL}; //preguntar si esto es valido
				if(execvp(para_dir,args) == -1){
					printf("Comando invalido\n");
				}
			
				exit(0);
			}
			wait(NULL);
		}
		
		else {
			seguir = 0;
		}
		
		
		
		
	}while(seguir);
	


 exit(0);
}
