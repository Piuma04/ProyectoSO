#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#define BFR_SZ 100 
   
int main(){
	
	int seguir = 1;
	char str[BFR_SZ];
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
		
		if(strlen(str) > 0){
			token = strtok(str," "); //obtiene la primera palabra
			
			if(token != NULL){
				resto = strtok(NULL,""); //captura el resto de la cadena
			
			
				if(strcmp(token,"help") == 0) {
					printf("help (muestra los comandos)\n");
					printf("ls\n");
					printf("mkdir (crea un directorio vacio)\n");
					printf("rmdir (remueve el directorio,el mismo debe estar vacio)\n");
					printf("touch (crea un archivo nuevo si no existe)\n");
					printf("chmod\n");
					printf("cat\n");
					printf("rmfile (remueve un archivo)\n");
					printf("exit (cierra la shell)\n");
				}
				else if(strcmp(token,"exit") != 0) {
					
					pid = fork();
					
					if(pid == 0){
						char para_dir[] = "./"; 
						strcat(para_dir,token);
						char *args[] = {token,resto,NULL}; 
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
			}
		
		}
		
	}while(seguir);
	


 exit(0);
}
