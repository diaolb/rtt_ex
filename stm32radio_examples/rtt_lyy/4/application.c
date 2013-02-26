#include <rtthread.h>

struct rt_thread thread1;
struct rt_thread thread2;

static rt_uint8_t thread1_stack[512];
static rt_uint8_t thread2_stack[512];

static void thread1_entry(void *parameter)
{
    static rt_uint32_t count = 0;

    for (; count<5; count++)
    {
        rt_thread_delay(RT_TICK_PER_SECOND * 3);
        
        rt_kprintf("count = %d\n", count);
    }
}

static void thread2_entry(void *parameter)
{
    rt_tick_t tick;
    rt_uint32_t i;

    for (i=0; i<15; i++)
    {
        tick = rt_tick_get();
        rt_thread_delay(RT_TICK_PER_SECOND);

        rt_kprintf("tick = %d\n", tick);
    }
}

int rt_application_init()
{
    rt_err_t result;

    result = rt_thread_init(&thread1,
        "t1",
        thread1_entry, RT_NULL, 
        &thread1_stack[0], sizeof(thread1_stack), 
        5, 5);

    if (result == RT_EOK)
        rt_thread_startup(&thread1);

    result = rt_thread_init(&thread2, 
        "t2",
        thread2_entry, RT_NULL, 
        &thread2_stack[0], sizeof(thread2_stack), 
        6, 5);

    if (result == RT_EOK)
        rt_thread_startup(&thread2);

    return 0;
}

/*@}*/  

                 