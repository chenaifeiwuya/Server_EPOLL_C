#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include<pthread.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<arpa/inet.h>
#include<errno.h>
/*请在这里添加程序注释
线程池代码的头文件
*/

#define _SERVER_IP "192.168.132"
#define _SERVER_PORT 8080
#define _TRUE 1
#define _FALSE 0
#define _BACKLOG 128
#define _BUFSIZE 1500
#define _IPSIZE 16
#define _DELCODE 10
#define _EPOLLMAX 200



int Epfd;  //epoll监听树描述符
int Sock_array[_EPOLLMAX];
pthread_mutex_t array_lock;

//business struct 
typedef struct
{
	void *(*Business)(void*);
	void * Business_arg;
}task_t;

//tomer_tids/thread pool struct
typedef struct
{
	int thread_shutdown;  //线程池开关：0-关闭  1-开启
	int thread_max;   //最大阈值，线程池最大线程数量
	int thread_min;  //最小阈值，  线程池最小线程数量
	int thread_alive;  //当前存活线程数量
	int thread_busy;  //当前繁忙线程数量。  闲置线程数量 = 存活数量  -  繁忙数量
	int thread_exitcode;  //线程退出数量

	task_t * Business_container;  //任务队列（环形队列）
	int FRONT;  //头索引 
	int REAR;  //尾索引
	int CUR;  //当前任务数
	int MAX;  //任务队列最大值

	pthread_cond_t Not_Full;  //生产者条件变量
	pthread_cond_t Not_Empty;  //消费者条件变量
	pthread_mutex_t lock;   //互斥锁
	pthread_t * customer_tids;  //存储消费者tid
	pthread_t * manager_tid;   //存储管理者tid
}pool_t;

int Server_Start(void);  //服务端启动接口
int Server_Net_INIT(void); //服务器网络初始化
pool_t * Server_Thread_Create(int Max, int Min, int Container_size);  //线程池创建初始化，返回线程池地址
int Server_Thread_Destroy(pool_t *);  //销毁线程池
void * Customer_Thread(void *);    //消费者线程工作
void * Manager_Thread(void *);    //管理者线程工作
int Epoll_Create(int sockfd);    //epoll创建初始化
int Epoll_Listen(int epfd,pool_t *ptr);   //阻塞监听
int Producer_Add_Task(pool_t *, task_t);  //向线程池任务队列添加一次任务

void * Business_Accept(void *);  //tcp连接服务
void * Business_Date(void *);  //数据处理业务
int Server_Start(void); 

