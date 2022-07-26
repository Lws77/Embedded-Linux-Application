#include "header.h"
#define _SERVER

#ifdef _CLIENT
int clientInit(uint16_t port, uint32_t ip){
	int ret;
	if(!ip){
		ip=INADDR_ANY;
	}
	
	/* Initialize the socket */ 
	SD=socket(AF_INET, SOCK_STREAM, 0);
	if(SD<0){
		printf("%s: socket() was failed, errno=%d\n", __func__, errno);
		return errno;
	}
	
	/* Socket seting  */
	serverIp.sin_family=AF_INET;
	serverIp.sin_port=htons(port);
	serverIp.sin_addr.s_addr=htonl(ip);
	memset(serverIp.sin_zero, 0, 8);
	
	/* Connect with server */
	ret=connect(SD, (struct sockaddr *)&serverIp, sizeof(struct sockaddr));
	if(ret<0){
		printf("%s: Connect() was failed, errno=%d\n", __func__, errno);
		return errno;
	}
	return 0;
}
#endif

#ifdef _SERVER
int serverInit(uint16_t port, uint32_t ip){
	int ret;
	if(!ip){
		ip=INADDR_ANY;
	}
	
	/* Initialize the socket */
	SD=socket(AF_INET, SOCK_STREAM, 0);
	if(SD<0){
		printf("%s: socket() was failed, errno=%d\n", __func__, errno);
		return errno;
	}
	/* Socket seting  */
	serverIp.sin_family=AF_INET;
	serverIp.sin_port=htons(port);
	serverIp.sin_addr.s_addr=htonl(ip);
	memset(serverIp.sin_zero, 0, 8);
	
	/* Assigns an address to the socket socket */
	ret=bind(SD, (struct sockaddr *)&serverIp, sizeof(struct sockaddr));
	if(ret<0){
		printf("%s: bind() was failed, errno=%d\n", __func__, errno);
		return errno;
	}
	
	/* Enables to accept connections */
	ret=listen(SD, MAX_LISTEN);
	if(ret<0){
		printf("%s: listen() was failed, errno=%d\n", __func__, errno);
		return errno;
	}
	return 0;
}

void findEmptyClt(int *i){
	int currUsrNum=0, timer=0; 
	while(client[(*i)].AD){
		currUsrNum++;
		(*i)++;
		(*i)%=MAX_LISTEN;
		if(currUsrNum>=MAX_LISTEN){
			printf("%s: The server is full, please wait for a minute...(%d)\n", __func__, timer++);
			sleep(2);
		}
	}
}

int acceptNewClt(int i, socklen_t *serverLen){
	int cltNum=i, ret=0;
	
	/* Accept */
	client[cltNum].AD=accept(SD, (struct sockaddr *)&(client[cltNum].remoteIp), serverLen);
	if(client[cltNum].AD<0){
		printf("%s: accept() was failed, errno=%d\n", __func__, errno);
		return errno;
	}
	/* Pthread_mutex_init */
	ret=pthread_mutex_init(&(client[cltNum].mutex), NULL);
	if(ret){
		printf("%s: pthread_mutex_init() was failed\n", __func__);
		close(client[cltNum].AD);
		return errno;
	}
	/* Pthread_create */
	ret=pthread_create(&(client[cltNum].pthId), NULL, pthFunc, (void *)&cltNum);
	if(ret){
		printf("%s: pthread_create() was failed\n", __func__);
		close(client[cltNum].AD);
		return errno;
	}
	
	return 0;
}
#endif

