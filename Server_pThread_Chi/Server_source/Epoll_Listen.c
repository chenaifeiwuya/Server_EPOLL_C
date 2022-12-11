#include <THREAD_SERVER.h>

int Epoll_Listen(int sockfd,pool_t * ptr)
{
	struct epoll_event ready_array[_EPOLLMAX];
	task_t bs;
	int readycode;
	int flag;
	//开始sock事件监听
	printf("Server Master thread 0x%x , Listen Socket Events..\n",(unsigned int)pthread_self());
	while(ptr->thread_shutdown)
	{
		if((readycode = epoll_wait(Epfd,ready_array, _EPOLLMAX,-1)) == -1)
			perror("Epoll_Listen Error, Call Epoll_wait Failed");
		flag = 0;
		while(readycode)
		{
			if(ready_array[flag].data.fd == sockfd){
				bs.Business = Business_Accept;
				bs.Business_arg = (void *)&(ready_array[flag].data.fd);
				Producer_Add_Task(ptr,bs);
			}else{
					bs.Business = Business_Date;
					bs.Business_arg = (void*)&(ready_array[flag].data.fd);
					Producer_Add_Task(ptr,bs);
			}
			++flag;
			--readycode;
		}
	}
	return 0;
}
