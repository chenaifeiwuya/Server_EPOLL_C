#include <THREAD_SERVER.h>

int Server_Thread_Destroy(pool_t * ptr)
{
	pthread_mutex_destroy(&(ptr->lock));
	pthread_cond_destroy(&(ptr->Not_Full));
	pthread_cond_destroy(&(ptr->Not_Empty));
	free(ptr->Business_container);
	free(ptr->customer_tids);
	free(ptr);
	return 0;
}
