#include "header.h"

char buf[BUFSIZE];

int main(int argc, char* argv[]){
	
	int infd, outfd;
	size_t len=BUFSIZE, ret=0;
	char *mod;
	memset((void*)buf, 0, sizeof(buf));
	
	/* check arguments */
	if(argc>1){
		mod=argv[1];
	}else{
		fprintf(stderr, "Usage: %s not enough arguments\n", argv[0]);
		return -1;
	}

	fprintf(stdout,"mod=%s\n", mod);

	switch(mod[0]){
		case 'w':
			/* open inport fd form the host */
			outfd=open(DEV_PATH, O_RDWR|O_NOCTTY|O_NDELAY);
			IS_OPEN(outfd);
			/* initialize Serial */
			setSerial(outfd, 115200, 8, 'N', 1, 0);
			/* send file name to the client */
			ret=sendFileName(outfd, &infd, argc, argv);
			/* content transmiting */
			ret=getContent(infd, outfd);
			break;
			
		case 'r':
			/* open inport fd form the host */
			infd=open(DEV_PATH, O_RDWR|O_NOCTTY|O_NDELAY); 
			IS_OPEN(infd);
			/* initialize Serial */
			setSerial(infd, 115200, 8, 'N', 1, 0);
			/* Read() get file name form the host */
			ret=getFileName(infd, &outfd , len);
			/* content transmiting */			
			ret=getContent(infd, outfd);                   
			break;
			
		default:
			fprintf(stderr, "Usage: %s arg 'r' for read, 'w' for write. \n", argv[0]);	
			break;
	}
	close(outfd);
	close(infd);
	return 0;
}
