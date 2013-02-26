#include <rtthread.h>

static rt_thread_t tid = RT_NULL;

static struct rt_messagequeue mq;

static rt_uint8_t msg_pool[2048];

static struct rt_timer timer;

static rt_uint16_t no = 0;

static void timer_timeout(void *parameter)
{
    char buf[32];
    rt_uint32_t length;
    
    length = rt_snprintf(buf, sizeof(buf), "message %d", no++);
    
    rt_mq_send(&mq, &buf[0], length);
}

static void thread_entry(void *parameter)
{
    char buf[64];
    rt_err_t result;
    
    rt_timer_init(&timer, "timer",
        timer_timeout,
        RT_NULL,
        1000,
        RT_TIMER_FLAG_PERIODIC);
    
    rt_timer_start(&timer);
    
    while (1)
    {
        rt_memset(&buf[0], 0, sizeof(buf));
        
        result = rt_mq_recv(&mq, &buf[0], sizeof(buf), RT_WAITING_FOREVER);
        if (result == RT_EOK)
        {
            rt_kprintf("recv msg: %s.\n", buf);
        }
        else if (result == -RT_ETIMEOUT)
        {
            rt_kprintf("recv msg timeout.\n");
        }
    }       
}

int rt_application_init()
{
    rt_err_t result;
    
    result = rt_mq_init(&mq, "mqt",
        &msg_pool[0],
        128 - sizeof(void *),
        sizeof(msg_pool),
        RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        rt_kprintf("init message queue failed.\n");
        return -1;
    }
    
    tid = rt_thread_create("tid",
        thread_entry, RT_NULL,
        1024,
        8, 50);
    if (tid != RT_NULL)
        rt_thread_startup(tid);
    
    return 0;
}
