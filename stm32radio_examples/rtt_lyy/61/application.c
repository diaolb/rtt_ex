#include <rtthread.h>

static rt_thread_t tid = RT_NULL;

static void thread_entry(void *parameter)
{

    
    while (1)
    {
        rt_thread_delay(1000);
    }       
}

int rt_application_init()
{

    
    tid = rt_thread_create("tid",
        thread_entry, RT_NULL,
        1024,
        8, 50);
    if (tid != RT_NULL)
        rt_thread_startup(tid);
    
    return 0;
}
