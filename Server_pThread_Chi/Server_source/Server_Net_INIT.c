#include<THREAD_SERVER.h>

int Server_Net_INIT(void)
{
	int sockfd;
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(_SERVER_PORT);//将主机字节序转换为网络字节序。（主机和网络存储方式不一样（大/小端））
	inet_pton(AF_INET,_SERVER_IP,&server_addr.sin_addr.s_addr);//inet_pton点分十进制ip地址转换为二进制对网络地址
	if((sockfd = socket(AF_INET, SOCK_STREAM,0)) == -1)
		perror("Server_NET_INIT, Socket Create error");
	if((bind(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr))) == -1)
		perror("Server_NET_INIT, Bind Failed");
	if((listen(sockfd,_BACKLOG)) == -1)
		perror("Server_NET_INIT,Listen Failed");
	return sockfd;
	
}
