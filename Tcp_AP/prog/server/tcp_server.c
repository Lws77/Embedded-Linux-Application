#include "header.h"

int SD;
struct sockaddr_in serverIp;
struct client client[MAX_LISTEN]={0, 0, 0, 0, 0};

void *pthFunc(void *arg){
	int ret, num=*(int*)arg;
	int target=!num;
	printf("%s: pthread[%d] is being started...(%s)\n", __func__, num, inet_ntoa(client[num].remoteIp.sin_addr));
	
	/* Set the AD to nonblock*/
	fcntl(client[num].AD, F_SETFL, fcntl(client[num].AD, F_GETFL, 0) | O_NONBLOCK); 
	
	/* Read/Write */
	while(1){
		sleep(1);
		pthread_mutex_lock(&(client[num].mutex));
		
		ret=recv(client[num].AD, client[num].buf, BUF_SIZE, 0);	
		
		if(ret>0 && client[target].AD){
			ret=write(client[target].AD, client[num].buf, strlen(client[num].buf));
		}else if(ret>0){
			printf("%s: No client for write()\n", __func__);
		}else if(!ret){
			break;
		}
		
		memset(client[num].buf, 0, sizeof(client[num].buf));
		pthread_mutex_unlock(&(client[num].mutex));	
	}
	
	/* Shutdown the client */
	pthread_mutex_destroy(&(client[num].mutex));
	close(client[num].AD);
	client[num].AD=0;
	printf("%s: client[%d] was closed\n", __func__, num);	
	return NULL;
}

int main(int argc, char* argv[]){
	int ret, i=0;
	socklen_t serverLen;
	serverLen=sizeof(struct sockaddr);
	
	/* Initializing the server */
	ret=serverInit(7777, 0);
	if(ret){
		goto err;
	}
	
	/* Creating a pthread for new client */
	while(1){
		
		findEmptyClt(&i);
		/*  Accept a connection request  */
		ret=acceptNewClt(i, &serverLen);
		if(ret){
			goto err;
		}
		sleep(1);
	}
	printf("%s: Server is exiting\n", __func__);
	
	/* Shutdown the server */
	close(SD);
	return 0;

err:
	close(SD);
	return errno;
}





/**
	int pthread_create (pthread_t * thread, pthread_attr_t * attr, void * (*start_routine)(void *), void * arg) 
*/


/**
	int socket (int namespace, int style, int protocol)  
	struct sockaddr_in { 
		short      sin_family;    // 2 bytes e.g. AF_INET, AF_INET6 
		unsigned short  sin_port;  // 2 bytes e.g. htons(3490) 
		struct in_addr  sin_addr;   // 4 bytes see struct in_addr, below 
		char       sin_zero[8];   // 8 bytes zero this if you want to 
		};
	struct in_addr {
		uint32_t  s_addr;     // address in network byte order 
	};

	int bind (int socket, struct sockaddr *addr, socklen_t length) 

*/


/**
	sudo nmap -sT -O localhost 
*/
