#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

int main(int argc, char *argv[]){

	char *dirname = argv[1];
	if(dirname == NULL) dirname = ".";
	DIR *dir = opendir(dirname);
	struct dirent *entrada;
	
	if(dir == NULL){
		printf("Descripcion del error: %s\n",strerror(errno));
		return 1;
	}
	
	while((entrada = readdir(dir)) != NULL){
		printf("%s\n",entrada->d_name);
		
	}
	
	
	closedir(dir); // es necesario cerrarlo en == null tmb?
		
	return 0;
}
