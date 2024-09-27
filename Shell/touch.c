#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>


int main(int argc, char *argv[]){

	char *filename = argv[1];
	FILE *archivo = fopen(filename,"a");
		
	
	if(archivo == NULL){
		printf("Descripcion del error: %s\n",strerror(errno));
	}
	
	
	fclose(archivo);
	
	return 0;
}
