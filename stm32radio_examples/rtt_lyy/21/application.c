#include <rtthread.h>

static rt_uint32_t share_var;

static char thread1_stack[1024];
struct rt_thread thread1;
static void rt_thread_entry1(void *parameter)
{
    rt_uint32_t i;
    share_var = 0;
    rt_kprintf("share_var = %d\n", share_var);
    rt_thread_delay(1000);
    for (i = 0; i < 10000; i++)
    {
        share_var++; 
    }
    rt_kprintf("\t share_var = %d\n", share_var);
    rt_kprintf("\t share_var = %d\n", share_var);
}

static char thread2_stack[1024];
struct rt_thread thread2;
static void rt_thread_entry2(void *parameter)
{
    //rt_thread_delay(1);
    share_var++;
}

int rt_application_init()
{
    rt_err_t result;
    
    result =  rt_thread_init(&thread1,
        "thread1",
        rt_thread_entry1, RT_NULL,
        &thread1_stack[0], sizeof(thread1_stack),
        5, 5);
    if (result == RT_EOK)
        rt_thread_startup(&thread1);
        
    result = rt_thread_init(&thread2,
        "thread2",
        rt_thread_entry2, RT_NULL,
        &thread2_stack[0], sizeof(thread2_stack),
        5, 5);
    if (result == RT_EOK)
        rt_thread_startup(&thread2);

    return 0;
}

/*@}*/  

                 