/*
 * 程序清单：哲学家就餐问题
 * 参考《Operating Systems Design and Implementation》 By Andrew S. Tanenbaum
 */

#include <rtthread.h>

void rt_init_thread_entry(void *parameter)
{

}

static struct rt_mailbox mb;

static rt_uint8_t  mb_pool[128];

static rt_uint8_t mb_str1[] = "I'm a mail!";
static rt_uint8_t mb_str2[] = "This is another mail!";
static rt_uint8_t mb_str3[] = "Over!";

static rt_uint8_t thread1_stack[1024];
struct rt_thread thread1;
static void thread1_entry(void *parameter)
{
    char *str;
    
    while (1)
    {
        rt_kprintf("thread1: try to receive a mail.\n");
        
        /* receive a mail from the mailbox. */
        if (rt_mb_recv(&mb, (rt_uint32_t *)&str, RT_WAITING_FOREVER) == RT_EOK)
        {
            rt_kprintf("thread1: get a mail from mailbox.\n");
            if (str == mb_str3)
                break;
                
            rt_thread_delay(10);
        }
    }
    
    rt_mb_detach(&mb);
}

static rt_uint8_t thread2_stack[1024];
struct rt_thread thread2;
static void thread2_entry(void *parameter)
{
    rt_uint8_t count;
    
    count = 0;
    while (count < 10)
    {
        count++;
        if (count & 0x01)
        {
            rt_mb_send(&mb, (rt_uint32_t)&mb_str1[0]);
        }
        else
        {
            rt_mb_send(&mb, (rt_uint32_t)&mb_str2[0]);
        }
        
        rt_thread_delay(20);
    }
    
    rt_mb_send(&mb, (rt_uint32_t)&mb_str3[0]);
}

int rt_application_init()
{
    rt_thread_t init_thread;
    rt_err_t result;
    
    result = rt_mb_init(&mb,
        "mbt",
        &mb_pool[0], sizeof(mb_pool)/4,
        RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        rt_kprintf("init mailbox failed.\n");
        return -1;
    }
    
    init_thread = rt_thread_create("init",
        rt_init_thread_entry, RT_NULL,
        2048,
        8, 20);
    if (init_thread != RT_NULL)
        rt_thread_startup(init_thread);
        
    rt_thread_init(&thread1,
                   "thread1",
                   thread1_entry,
                   RT_NULL,
                   &thread1_stack[0],
                   sizeof(thread1_stack),10,5);
    rt_thread_startup(&thread1);


    rt_thread_init(&thread2,
                   "thread2",
                   thread2_entry,
                   RT_NULL,
                   &thread2_stack[0],
                   sizeof(thread2_stack),10,5);
    rt_thread_startup(&thread2);
    
    return 0;
}
