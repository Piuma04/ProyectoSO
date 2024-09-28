#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>


int main(int argc, char *argv[]){

	char *permisos = strtok(argv[1]," "), *endptr;
	mode_t perm =  strtol(permisos,&endptr,8);
	if(endptr == permisos || *endptr != '\0'){
		printf("Hay caracteres invalidos en el numero de permiso\n");
		return 0;
	}
	
	
	int cheq_perm = perm;
	
	int temp, digits = 1;
	while(cheq_perm != 0 && digits <=3){
		temp = cheq_perm % 10;
		if(temp >7 || temp <0){ //si no es un digito valido se termina
			printf("Numero de permiso invalido\n");
			return 0;
			
		} 
		digits++;
	
		cheq_perm = cheq_perm / 10;
	
	}
	
	if(cheq_perm != 0){
		printf("Numero de permiso invalido\n");
		return 0;
	}
	
	
	
	char *archivo = strtok(NULL," ");
	
	if(chmod(archivo,perm) != 0){
		printf("Descripcion del error: %s\n",strerror(errno));
	}
	
	
		
	return 0;
}
