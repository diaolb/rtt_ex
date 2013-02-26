#include <rtthread.h>

static struct rt_thread thread1;

static struct rt_thread thread2;

static rt_uint8_t thread1_stack[512];

static rt_uint8_t thread2_stack[512];

/* entry for thread1 */
static void thread1_entry(void* parameter)
{
    rt_uint32_t count = 0;

    while(1)
    {
        rt_kprintf("thread count: %d\n", count++);
        rt_thread_delay(RT_TICK_PER_SECOND);
    }
}   

static void thread2_entry(void* parameter)
{
    rt_thread_delay(RT_TICK_PER_SECOND * 10);

    rt_thread_detach(&thread1);

    rt_thread_delay(10);
}

int rt_application_init()
{
    rt_err_t result;

    result = rt_thread_init(&thread1, "t1",
        thread1_entry, RT_NULL,
        &thread1_stack[0], sizeof(thread1_stack), 
        7, 10);
    if (result == RT_EOK)
        rt_thread_startup(&thread1);

    result = rt_thread_init(&thread2, "t2",
        thread2_entry, RT_NULL,
        &thread2_stack[0], sizeof(thread2_stack), 
        6, 10);
    if (result == RT_EOK)
        rt_thread_startup(&thread2);
    return 0;
}
