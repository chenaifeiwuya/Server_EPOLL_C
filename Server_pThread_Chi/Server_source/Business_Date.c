#include<THREAD_SERVER.h>

void * Business_Date(void * arg)
{
	char Buffer[_BUFSIZE];
	char Result[_BUFSIZE];
	int Recvlen;
	int ClientFd = *(int *)arg;
	bzero(Buffer,sizeof(Buffer));
	bzero(Result,sizeof(Result));
	while((Recvlen = recv(ClientFd,Buffer,sizeof(Buffer),MSG_DONTWAIT))>0)  //注意此处一定要用非阻塞接收
	{
		//Forward to
		sprintf(Result,"unknow Say : %s\n",Buffer);
		pthread_mutex_lock(&array_lock);
		for(int i=0;i<_EPOLLMAX;i++)
			if(Sock_array[i]!= -1){
				send(Sock_array[i], Result, strlen(Result),MSG_NOSIGNAL);
			}
		pthread_mutex_unlock(&array_lock);
	}
	
	//EPOLL模型采用oneshot时，监听成功后会将就绪节点从监听树中删除，所以需要重新加上
	struct epoll_event Node;
	Node.events = EPOLLIN|EPOLLET|EPOLLONESHOT;
	Node.data.fd=ClientFd;
	if((epoll_ctl(Epfd,EPOLL_CTL_MOD,ClientFd,&Node)) == -1)
		perror("Business_Accept, epoll_Ctl() Listne Clientfd Failed");

	if(Recvlen == 0){  //对方断开连接
		pthread_mutex_lock(&array_lock);
		for(int i=0;i<_EPOLLMAX;i++)
			if(ClientFd == Sock_array[i]){
				Sock_array[i] = -1;
				break;
			}
		pthread_mutex_unlock(&array_lock);
		epoll_ctl(Epfd,EPOLL_CTL_DEL,ClientFd,NULL);  //删除监听
		close(ClientFd);
		printf("Client its Done, Close Sockfd, Delete Events.\n");
	}

	return NULL;
}
