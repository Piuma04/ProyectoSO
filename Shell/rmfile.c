#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>


int main(int argc, char *argv[]){
	
	
	char *pathnames = argv[1];
	char *pathname = strtok(pathnames," ");
	int result;
	
	result = unlink(pathname);
	
	if(result != 0){
		printf("Descripcion del error: %s\n",strerror(errno));
	}	
	
	pathname = strtok(NULL," ");
	
	while(pathname != NULL){
		result = unlink(pathname);
		
		if(result != 0){
			printf("Descripcion del error: %s\n",strerror(errno));
		}
		pathname = strtok(NULL," ");

	}
	
	
	return 0;	
	
}
