#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include<pthread.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<linux/socket.h>
#include<netdb.h>
/*请在这里添加程序注释
client
非阻塞读标准输入*/

int main(void)
{

	struct sockaddr_in ServerAddr;
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(8080);
	ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
//	ServerAddr.sin_addr.s_addr = inet_addr("42.193.104.238");
	int cSock = socket(AF_INET,SOCK_STREAM,0);
	//int SSock = socket(AF_INET,SOCK_STREAM,0);
	if (-1 == connect(cSock,(struct sockaddr*)&ServerAddr,sizeof(ServerAddr)))
	{
		perror("connect Error!");
		exit(0);
	}
	char buf[1024];
while(1)
{
//	bzero(buf,1024);
//	recv(cSock,buf,1024,MSG_DONTWAIT);
//	printf("%s\n",buf);
	bzero(buf,1024);
	//strcpy(buf, "localtime\n");
	//scanf("%s",buf);
	

	//将标准输入变成非阻塞的:
	int flags;
	int len;
	flags = fcntl(STDIN_FILENO, F_GETFL,0);   //将STDIN默认的flags选项获取并返回
	flags |= O_NONBLOCK;
	fcntl(STDIN_FILENO,F_SETFL,flags);   //设置非阻塞


	read(STDIN_FILENO,buf,1024);  //直接读标准输入
	send(cSock,buf,strlen(buf),MSG_DONTWAIT);
	usleep(100000);  //等待服务器响应
	len = recv(cSock,buf,1024,MSG_DONTWAIT);   //MSG_DONTWEIT表示非阻塞执行
	if(len > 0)
	{
	printf("%s\n",buf);
	}
	//sleep(2);
}
	close(cSock);
	return 0;
}
