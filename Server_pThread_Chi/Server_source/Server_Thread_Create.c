#include<THREAD_SERVER.h>

pool_t * Server_Thread_Create(int Max, int Min, int Container_size)
{
	//内存申请与初始化
	pool_t * ptr = NULL;
	if((ptr = (pool_t*)malloc(sizeof(pool_t))) == NULL)
		perror("Server_Thread_Create , Malloc pool_tFailed");
		ptr->thread_shutdown = _TRUE; //开启线程池
		ptr->thread_max = Max;
		ptr->thread_min = Min;
		ptr->thread_alive = 0;
		ptr->thread_busy = 0;
		ptr->thread_exitcode = 0;
			

			//给任务队列分配空间
		if((ptr->Business_container = (task_t *)malloc(sizeof(task_t) * Container_size)) == NULL)
			perror("Server_Thread_Create, Malloc Business_Cotainer Failed");
			ptr->FRONT = 0;
			ptr->REAR = 0;
			ptr->CUR = 0;
			ptr->MAX = Container_size;

		//初始化互斥量和条件变量
		if(pthread_mutex_init(&(ptr->lock),NULL) != 0 || pthread_cond_init(&(ptr->Not_Full), NULL)!=0 || pthread_cond_init(&(ptr->Not_Empty),NULL)!= 0)
			printf("Server_Thread_Create, init Lock or Cond Failed");

		//给消费者线程id数组分配空间
		if((ptr->customer_tids = (pthread_t *)malloc(sizeof(pthread_t) * Max)) ==NULL)
			perror("Server_Thread_Create, Malloc Tids Failed");

	printf("1\n");
		//预创建消费者线程
		int err;
		for(int i =0;i<Min;i++)
		{
			if((err = pthread_create(&(ptr->customer_tids[i]),NULL,Customer_Thread,(void *)ptr))>0)
				printf("Server_Thread_Create, pthread_create Customer Failed:%s",strerror(err));
			++(ptr->thread_alive);
		}
printf("2\n");
		//创建管理者线程
		if((err = pthread_create(&(ptr->manager_tid),NULL,Manager_Thread,(void *)ptr))>0)
			printf("Server_Thread_Create, pthread_create Customer Failed:%s",strerror(err));
printf("3\n");
		return ptr;
}
