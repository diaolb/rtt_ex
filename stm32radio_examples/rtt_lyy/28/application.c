
#include <rtthread.h>
#include <string.h>
void rt_init_thread_entry(void *parameter)
{

}

#define MSG_VIP "over"

static struct rt_messagequeue mq;

static rt_uint8_t msg_pool[2048];

static rt_uint8_t thread1_stack[1024];
struct rt_thread thread1;
static void thread1_entry(void *parameter)
{
    char buf[128];
    
    while (1)
    {
        rt_memset(&buf[0], 0, sizeof(buf));
        
        if (rt_mq_recv(&mq, &buf[0], sizeof(buf), RT_WAITING_FOREVER) == RT_EOK)
        {
            rt_kprintf("thread1: recv msg from msg queue, the content:%s\n", buf);
            
            if (strcmp(buf, MSG_VIP) == 0)
                break;
        }
        
        rt_thread_delay(RT_TICK_PER_SECOND);
    }
    
    rt_kprintf("thread1: got an urgent message, leave.\n");
}

static rt_uint8_t thread2_stack[1024];
struct rt_thread thread2;
static void thread2_entry(void *parameter)
{
    int i, result;
    char buf[128];
    
    i = 0;
    while (1)
    {
        rt_snprintf(buf, sizeof(buf), "this is message NO.%d", i);
        
        result = rt_mq_send(&mq, &buf[0], sizeof(buf));
        if (result == -RT_EFULL)
            break ;
        
        rt_kprintf("thread2: send message - %s\n", buf);
        
        i++;
    }
    
    rt_kprintf("message queue full, thread2 leave.\n");
}   

static rt_uint8_t thread3_stack[1024];
struct rt_thread thread3;
static void thread3_entry(void *parameter)
{
    char msg[] = MSG_VIP;
    rt_err_t result;
    
    rt_thread_delay(RT_TICK_PER_SECOND * 5);
    rt_kprintf("thread3: send an urgent message <%s> \n", msg);
    
    do 
    {
        result = rt_mq_urgent(&mq, &msg[0], sizeof(msg));
        
        if (result != RT_EOK)
            rt_thread_delay(20);
    } while (result != RT_EOK);
}

int rt_application_init()
{
    rt_thread_t init_thread;
    rt_err_t result;
    
    result = rt_mq_init(&mq, "mqt", 
        &msg_pool[0], /* 内存池指向msg_pool */ 
        128 - sizeof(void*), /* 每个消息的大小是 128 - void* */
        sizeof(msg_pool), /* 内存池的大小是msg_pool的大小 */
        RT_IPC_FLAG_FIFO);/* 如果有多个线程等待，按照先来先得到的方法分配消息 */
    if (result != RT_EOK)
    {
        rt_kprintf("init message queue failed.\n");
        return -1;
    }
    
    init_thread = rt_thread_create("init",
        rt_init_thread_entry, RT_NULL,
        2048, 8, 20);
    if (init_thread != RT_NULL)
        rt_thread_startup(init_thread);
    
    rt_thread_init(&thread1,
        "t1",
        thread1_entry, RT_NULL,
        &thread1_stack[0], sizeof(thread1_stack),
        10, 50);
    rt_thread_startup(&thread1);  
    
    rt_thread_init(&thread2,
                   "thread2",
                   thread2_entry,
                   RT_NULL,
                   &thread2_stack[0],
                   sizeof(thread2_stack),10,50);
    rt_thread_startup(&thread2);

    rt_thread_init(&thread3,
                   "thread3",
                   thread3_entry,
                   RT_NULL,
                   &thread3_stack[0],
                   sizeof(thread3_stack),10,50);
    rt_thread_startup(&thread3); 
    
    return 0;
}
