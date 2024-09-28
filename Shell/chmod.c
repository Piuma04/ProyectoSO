#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>


int main(int argc, char *argv[]){

	char *permisos = strtok(argv[1]," ");
	mode_t perm =  strtol(permisos,NULL,8);
	int cheq_perm = perm;
	int temp;
	while(cheq_perm != 0){
			
	
	}
	
	
	
	
	
	char *archivo = strtok(NULL," ");
	
	if(chmod(archivo,perm) != 0){
		printf("Descripcion del error: %s\n",strerror(errno));
	}
	
	
		
	return 0;
}
