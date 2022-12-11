#include <THREAD_SERVER.h>

int IF_THREAD_ALIVE(pthread_t tid)
{
	int err;
	err = pthread_kill(tid,0);
	if(err == ESRCH)
		return 0;
	else
		return 1;
}

void * Manager_Thread(void * arg)
{
	pool_t * ptr = NULL;
	ptr = (pool_t *)arg;
	pthread_detach(pthread_self());
	printf("Mannager Thread 0x%x , Runing...\n",(unsigned int)pthread_self());
	int alive,busy,cur;
	int flags;   //遍历下标
	int Add;  //记录线程创建数量
	int err;
	while(ptr->thread_shutdown){
		pthread_mutex_lock(&ptr->lock);
		alive = ptr->thread_alive;
		busy = ptr->thread_busy;
		cur = ptr->CUR;
		printf("Manager Output Server Thread Infomation: Alive [%d] Busy [%d] Idel [%d] Cur [%d] Busy-Alive [%.2f%%] Alive-All [%.2f%%]\n\n\n",alive,busy,alive-busy,cur,(double)busy/alive * 100,(double)alive / ptr->thread_max * 100);
		pthread_mutex_unlock(&ptr->lock);          //查看是否满足扩容线程的条件
		if((cur >= alive - busy || busy / alive * 100 >= 70) && alive + _DELCODE <= ptr->thread_max)
		{
			for(flags = 0, Add = 0; flags < ptr->thread_max && Add< _DELCODE; flags++)   //遍历消费者线程tid数组
			{
				if(ptr->customer_tids[flags] == 0 || !IF_THREAD_ALIVE(ptr->customer_tids[flags]))  //如果该下标位置没被占用或者下标上到线程id对应到线程已经退出(通过pthread_kill()判断tid对应线程是否存活)
				{
					if((err = pthread_create(&(ptr->customer_tids[flags]),NULL,Customer_Thread,(void*)ptr))>0)
						printf("Server Manager Thread 0x%x Create New Customer Failed;%s\n",(unsigned int)pthread_self(),strerror(err));
					pthread_mutex_lock(&ptr->lock);
					++(ptr->thread_alive);
					pthread_mutex_unlock(&ptr->lock);
					++Add;
				}
			}
		}
		if(busy * 2<(alive - busy) && alive - _DELCODE >= ptr->thread_min){    //查看是否满足缩减线程条件
			pthread_mutex_lock(&ptr->lock);
			ptr->thread_exitcode = _DELCODE;
			pthread_mutex_unlock(&ptr->lock);
			for(int i = 0; i<_DELCODE;i++)
				pthread_cond_signal(&ptr->Not_Empty);  //唤醒闲线程
		}
		sleep(2);  //管理者线程每两秒工作一次
	}
	pthread_exit(NULL);
}
