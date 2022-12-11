#include<THREAD_SERVER.h>

int Epoll_Create(int sockfd)
{
	struct epoll_event Node;
	if((Epfd = epoll_create(_EPOLLMAX)) == -1)
		perror("Epoll_Create , epoll_create Call Failed");
	Node.events = EPOLLIN|EPOLLET|EPOLLONESHOT;//边缘触发模式以及设置为EPOLLONESHOT（当监听成功后将就绪节点从红黑数删除，但是后续记得添加回去！）
	Node.data.fd = sockfd;

	if((epoll_ctl(Epfd,EPOLL_CTL_ADD,sockfd,&Node)) == -1)
		perror("Epoll_Create , epoll_Ctl Failed, set Serverfd");
		
	return 0;
}
