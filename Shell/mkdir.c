#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]){
	
	mode_t mode = 0755;
	char *pathnames = argv[1];
	char *pathname = strtok(pathnames," ");
	int result;
	
	result = mkdir(pathname,mode);
	
	if(result != 0){
		printf("Descripcion del error: %s\n",strerror(errno));
	}	
	
	pathname = strtok(NULL," ");
	
	while(pathname != NULL){
		result = mkdir(pathname,mode);
		
		if(result != 0){
			printf("Descripcion del error: %s\n",strerror(errno));
		}
		pathname = strtok(NULL," ");

	}
	
	
	return 0;	
	
}
