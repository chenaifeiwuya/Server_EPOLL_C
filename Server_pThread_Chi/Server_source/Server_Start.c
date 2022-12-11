#include<THREAD_SERVER.h>

int Server_Start(void)
{
	printf("1\n");
	int sockfd;
	pool_t * ptr = NULL;
	sockfd = Server_Net_INIT();
	printf("2\n");
	Epoll_Create(sockfd);
	printf("3\n");
	pthread_mutex_init(&array_lock,NULL);
	printf("4\n");
//	pthread_mutex_init(&accept_lock,NULL);
	for(int i=0;i<_EPOLLMAX;i++)
		Sock_array[i] = -1;
	ptr = Server_Thread_Create(200,10,500);
	printf("5\n");
	Epoll_Listen(sockfd,ptr);
	printf("6\n");
	Server_Thread_Destroy(ptr);
	return 0;
}
