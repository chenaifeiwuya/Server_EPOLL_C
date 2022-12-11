#include <THREAD_SERVER.h>
  
  //调用此函数对线程即为生产者线程，调用一次向任务队列中添加一次任务
 int Producer_Add_Task(pool_t *ptr, task_t bs)
 {
   if(ptr->thread_shutdown)
   {
	pthread_mutex_lock(&ptr->lock);
   while(ptr->MAX == ptr->CUR){
    pthread_cond_wait(&ptr->Not_Full,&ptr->lock);  //生产者线程仅有一个，无需考虑误唤醒问题
    if(ptr->thread_shutdown == 0){
    pthread_mutex_unlock(&ptr->lock);
    printf("Server threadshutdown its %d,Producer thread 0x%x Exiting..\n",ptr->thread_shutdown,(unsigned int)pthread_self());
    return -1;
      }
					  }
	  //添加任务
    ptr->Business_container[ptr->FRONT].Business = bs.Business;
    ptr->Business_container[ptr->FRONT].Business_arg = bs.Business_arg;
	ptr->FRONT = (ptr->FRONT + 1)%ptr->thread_max;
	++(ptr->CUR);
	printf("Server Producer thread 0x%x Add Task/..\n",(unsigned int)pthread_self());
	pthread_mutex_unlock(&ptr->lock);
	pthread_cond_signal(&(ptr->Not_Empty));
	return 0;     
     }
	 return -1;  
 }
