

int IF_THREAD_ALIVE(pthread_t tid)
{
	int err;
	err = pthread_kill(tid,0);  //如果tid对应线程已经退出，则会将错误码置成 ESRCH
	if(err == ESRCH)   
		return 0;
	else
		return 1;
}
