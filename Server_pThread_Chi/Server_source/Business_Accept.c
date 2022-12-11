#include<THREAD_SERVER.h>

void * Business_Accept(void * arg)
{
	int Serverfd = *(int *)arg;
	struct sockaddr_in ClientAddr;
	int ClientFd;
	struct epoll_event Node, NodeServer;
	char Response[_BUFSIZE];
	char client_ip[_IPSIZE];

	socklen_t Addrlen;
	Addrlen = sizeof(ClientAddr);//Addrlen一定要先赋值，否则会出错
	if((ClientFd = accept(Serverfd,(struct sockaddr *)&ClientAddr,&Addrlen)) == -1)
		perror("Business_Accept error, Accept Failed");
	
	NodeServer.events = EPOLLIN|EPOLLET|EPOLLONESHOT;
	NodeServer.data.fd=Serverfd;
	if((epoll_ctl(Epfd,EPOLL_CTL_MOD,Serverfd,&NodeServer)) == -1)  //注意此处要用EPOLL_CTL_MOD而非ADD   倒数第二个参数是红黑数的索引
		perror("Business_Accept, epoll_Ctl() Listne Clientfd Failed");
	
	for(int i=0;i<_EPOLLMAX;i++)
		if(Sock_array[i] == -1){
			Sock_array[i] = ClientFd;
			break;
		}
		
	//设置监听
	Node.events = EPOLLIN|EPOLLET|EPOLLONESHOT;
	Node.data.fd=ClientFd;
	if((epoll_ctl(Epfd,EPOLL_CTL_ADD,ClientFd,&Node)) == -1) 
		perror("Business_Accept, epoll_Ctl() Listne Clientfd Failed");
	inet_ntop(AF_INET,&ClientAddr.sin_addr.s_addr,client_ip,_IPSIZE);
	sprintf(Response,"hi %s Welcom Linux thread Server Demo.",client_ip);
	send(ClientFd,Response,strlen(Response),MSG_NOSIGNAL);
	printf("Server Customer 0x%x , Exec Business_Accept, Client Connection success, ip %s, port %d, sockfd %d\n",(unsigned int)pthread_self(),client_ip,ntohs(ClientAddr.sin_port),ClientFd);
	return NULL;
}
