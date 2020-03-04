#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <poll.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define DEBUG_INFO(fmt, args...) printf("\033[33m[%s:%s:%d]\033[0m "#fmt"\r\n", __FILE__,__FUNCTION__, __LINE__, ##args)

int Unicastfd = -1;
int UnicastInit(void)
{
	//建立数据报套接字
    Unicastfd = socket(AF_INET, SOCK_DGRAM/*表示采用数据报套接字*/, 0);
    if (Unicastfd < 0){
        perror("create socket failed:");
        return -1;
    }
	/*客户端如果没有指定端口，内核会随机指定一个端口*/
}

/*发送数据*/
void UnicastSend(char *buff , int len,struct sockaddr_in *addr)
{
	/*当udp调用connect时,第五和第六个参数可以指定为NULL和0*/
	int ret = sendto(Unicastfd , buff , len , MSG_DONTWAIT/*非阻塞*/,(struct sockaddr*)addr, sizeof(*addr));
	if(ret < 0){
		DEBUG_INFO("Unicast_Send Fail");
	}
	DEBUG_INFO("ret=%d",ret);
}

/*处理udp数据*/
void UnicastReceiveData(void)
{
	static int count = 1;
	struct sockaddr_in addr; 
	int len = sizeof(addr);
	int ret;
	char RevBuf[1024]={0};
	ret = recvfrom(Unicastfd,RevBuf,1024,MSG_DONTWAIT/*非阻塞，没有数据可读立即返回*/,(struct sockaddr*)&addr,&len);
    DEBUG_INFO("ip:%s,port:%d,buf:%s",inet_ntoa(addr.sin_addr), ntohs(addr.sin_port),RevBuf);
}

/*udp处理线程*/
void *UnicastPoll(void *data)
{
	pthread_detach(pthread_self());
	while(1){
		struct pollfd pollfds[1];
		pollfds[0].fd=Unicastfd;
		pollfds[0].events=POLLIN;
		poll(pollfds,1,-1);//-1表示，没有数据可读，将永久阻塞
		if(pollfds[0].revents){
			UnicastReceiveData();
		}
	}
	pthread_exit(NULL);
}

int main(void)
{
	int ret = UnicastInit();
	if(ret < 0){
		DEBUG_INFO("%d",ret);
		exit(-1);
	}	
	pthread_t t1;
	//pthread_create(&t1,NULL,UnicastPoll,NULL);
	
	while(1){
		static int count = 10000;
		unsigned char buf[1507];
		memset(buf,'a',sizeof(buf));
		sprintf(buf,"%d",count++);
		struct sockaddr_in addr;
		memset((void*) &addr, 0, sizeof(struct sockaddr_in));
		addr.sin_family = AF_INET;
		//服务端地址
		addr.sin_addr.s_addr = inet_addr("172.16.2.175");
		//服务端端口
		addr.sin_port = htons(9090);
		sleep(5);
		//connect(Unicastfd, (struct sockaddr*)&addr,sizeof(addr));    
	    UnicastSend(buf,sizeof(buf),&addr);
		exit(0);
	}
	return 0;
}
