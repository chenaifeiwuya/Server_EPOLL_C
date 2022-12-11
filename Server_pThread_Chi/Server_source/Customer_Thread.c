#include<THREAD_SERVER.h>

void * Customer_Thread(void * arg)
{
	pool_t * ptr = NULL;
	ptr = (pool_t *)arg;
	pthread_detach(pthread_self());  //分离线程
	task_t bs;
	printf("Server Customer 0x%x Waiting Business...\n",(unsigned int)pthread_self());
	while(ptr->thread_shutdown)  //如果线程池未被销毁
	{
		pthread_mutex_lock(&ptr->lock);
		while(ptr->CUR == 0){  //用while的好处：唤醒后会再检查一遍条件是否满足
			pthread_cond_wait(&(ptr->Not_Empty),&ptr->lock);
			if(ptr->thread_shutdown == 0){  //唤醒之后要再检查一遍线程池是否存在，如果不存在
				pthread_mutex_unlock(&ptr->lock);
				printf("Server Customer Thread 0x%x, shutdown its %d, EXITING..\n",(unsigned int)pthread_self(),ptr->thread_shutdown);
				pthread_exit(NULL);
			}
			if(ptr->thread_exitcode > 0)  //如果当前需要缩减线程
			{
				--(ptr->thread_exitcode);
				--(ptr->thread_alive);
				printf("Customer Thread 0x%x , exitcode-1 %d, Exiting...\n",(unsigned int)pthread_self(),ptr->thread_exitcode);
			}
		}


		bs.Business = ptr->Business_container[ptr->REAR].Business;
		bs.Business_arg = ptr->Business_container[ptr->REAR].Business_arg;
		--(ptr->CUR);
		ptr->REAR = (ptr->REAR + 1) % ptr->thread_max;
		++(ptr->thread_busy);
		printf("This is Customer Thread\n\n");
		pthread_mutex_unlock(&ptr->lock);
		pthread_cond_signal(&ptr->Not_Full);
		bs.Business(bs.Business_arg);  //业务到执行不能在锁中 
		pthread_mutex_lock(&ptr->lock);
		--(ptr->thread_busy);
		pthread_mutex_unlock(&ptr->lock);
	}
	pthread_exit(NULL);
}
