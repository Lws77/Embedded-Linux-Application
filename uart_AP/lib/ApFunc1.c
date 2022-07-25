#include "header.h"

int setSerial(int fd, int nSpeed, int nBits, char nEvent, int nStop, int vmin){
    struct termios newttys1,oldttys1;
/**
 * @ 保存原有串口配置
 */
    if(tcgetattr(fd,&oldttys1)!=0){
		perror("Setupserial 1");
		return -1;
    }
    bzero(&newttys1,sizeof(newttys1));
	/*CREAD 開啓串行數據接收，CLOCAL並打開本地連接模式*/
    newttys1.c_cflag|=(CLOCAL|CREAD ); 
	/*設置數據位*/
    newttys1.c_cflag &=~CSIZE;
/**
 * @ 數據位選擇
 */  
     switch(nBits){
         case 7:
             newttys1.c_cflag |=CS7;
             break;
         case 8:
             newttys1.c_cflag |=CS8;
             break;
     }	 
/**
 * @ 設置奇偶校驗位
 */
	switch( nEvent ){
		/*奇校驗*/
		case '0':  
			/*開啓奇偶校驗*/
			newttys1.c_cflag |= PARENB;
			/*INPCK打開輸入奇偶校驗；ISTRIP去除字符的第八個比特  */
			newttys1.c_iflag |= (INPCK | ISTRIP);
			/*啓用奇校驗(默認爲偶校驗)*/
			newttys1.c_cflag |= PARODD;
			break;
		/*偶校驗*/
		case 'E':
			/* 開啓奇偶校驗 */
			newttys1.c_cflag |= PARENB; 
			/* 打開輸入奇偶校驗並去除字符第八個比特 */
			newttys1.c_iflag |= ( INPCK | ISTRIP);
			/* 啓用偶校驗 */
			newttys1.c_cflag &= ~PARODD;
			break;
		/*無奇偶校驗*/
		case 'N': 
            newttys1.c_cflag &= ~PARENB;
			newttys1.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
			/* 啟動非正規模式，將OPOST選項設為disable */
			newttys1.c_oflag &= ~OPOST;
			/* c_lflag控制串列埠如何處理輸入字元 */
			newttys1.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
			break;
	}
/**
 * @ 設置波特率
 */
    switch( nSpeed ){
        case 2400:
            cfsetispeed(&newttys1, B2400);
            cfsetospeed(&newttys1, B2400);
            break;
        case 4800:
            cfsetispeed(&newttys1, B4800);
            cfsetospeed(&newttys1, B4800);
            break;
        case 9600:
            cfsetispeed(&newttys1, B9600);
            cfsetospeed(&newttys1, B9600);
            break;
        case 115200:
            cfsetispeed(&newttys1, B115200);
            cfsetospeed(&newttys1, B115200);
            break;
		case 1500000:
            cfsetispeed(&newttys1, B1500000);
            cfsetospeed(&newttys1, B1500000);
            break;
        default:
            cfsetispeed(&newttys1, B9600);
            cfsetospeed(&newttys1, B9600);
            break;
    }	
/**
 * @ 設置停止位
 */
	/*設置停止位；若停止位爲1，則清除CSTOPB，若停止位爲2，則激活CSTOPB*/
    if( nStop == 1){
		/*默認爲一位停止位； */
        newttys1.c_cflag &= ~CSTOPB;
    }
    else if( nStop == 2){
		/*CSTOPB表示送兩位停止位*/
        newttys1.c_cflag |= CSTOPB;
    }
	/*非規範模式讀取時的超時時間；*/
	newttys1.c_cc[VTIME] = 0;
	/*非規範模式讀取時的最小字符數*/    
    newttys1.c_cc[VMIN]  = vmin; 
	
	
    /*tcflush清空終端未完成的輸入/輸出請求及數據；TCIFLUSH表示清空正收到的數據，且不讀取出來 */
	tcflush(fd ,TCIFLUSH);

     /*激活配置使其生效*/
    if((tcsetattr( fd, TCSANOW,&newttys1))!=0){
        perror("com set error");
        return -1;
    }
    return 0;
}

int getFileName(int infd, int* outfd, size_t len){
	size_t ret=0;	
	char *fname;
	/* Clear both queued input and output */
	tcflush(infd, TCIOFLUSH);
	
	while(!ret && ret<48){
		/* read from host */
		ret=read(infd, (void*)buf, len);
		IS_WR(ret, "READ", infd);
		if(ret>0 && ret<48){
			buf[ret]='\0';
			fname=(char*)malloc(strlen(buf)*sizeof(char));
			strcpy(fname, buf);
			/* create a new file for copied */
			(*outfd)=open((char *)fname, O_WRONLY|O_CREAT|O_TRUNC, 0644);
			IS_OPEN((*outfd));
		}
		/* wait for host */
		fprintf(stdout,"wait for host, buf=%s, ret=%ld\n", buf, ret);
		sleep(WRITER_WAIT_TIME*2);
	}
	fprintf(stdout,"fname=%s\n", fname);
	
	memset((void*)buf, 0, sizeof(buf));
	free(fname);
	
	return ret;
}

int sendFileName(int outfd, int* infd, int argc, char* argv[]){
	int len;
	char *fname;
	size_t ret=0;
	
	/* check arguments (file name)*/
	if(argc!=3){
		fprintf(stderr, "Usage: %s not enough arguments. <filename>\n", argv[0]);
		return -1;
	}
	len=strlen(argv[2]);
	fname=(char*)malloc(len*sizeof(char));
	
	/* copy file name from argv[2]*/
	strcpy(fname, argv[2]);
	
	/* open the file to transfer */
	(*infd)=open(fname, O_RDONLY);
	IS_OPEN((*infd));
	
	/* write to client */
	ret=write(outfd, (void*)fname, len);
	IS_WR(ret, "WRITE", outfd);
}

int getContent(int infd, int outfd){
	int count, ctlnum=0;
	size_t inret=0, outret=0;
	char *bufadr;
	memset((void*)buf, 0, sizeof(buf));
	
	do{
		/* read from local file or host  */
		inret=read(infd, (void*)buf, BUFSIZE); 
		if(inret==-1){
			fprintf(stderr, "read() failed, errno=%d\n", errno);
			close(infd);
			return -1;
		}else if(!inret && ctlnum){
			break;
		}else if(inret>0){
			ctlnum++;
		}
		fprintf(stdout, "just read the following %ld bytes from the host\n", inret);		
		sleep(WRITER_WAIT_TIME);
		count=inret;
		bufadr=buf;
		while(count>0){
			/* write to local file or client */
			outret=write(outfd, (void*)bufadr, count);
			if(outret<0){
				fprintf(stderr, "write() failed, errno=%d", errno);
				close(outfd);
				return -1;
			}
			count=count-outret;
			bufadr=bufadr+outret;
			fprintf(stdout, "just write the following %ld bytes to the file\n", outret);
		}
	}while(inret>0 || !ctlnum);
	return 0;
}

