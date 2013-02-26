#include <rtthread.h>

rt_thread_t tid1 = RT_NULL;
rt_thread_t tid2 = RT_NULL;

static void thread1_entry(void* parameter)
{
    rt_uint32_t count = 0;
    
    while (1)
    {
        rt_kprintf(" thread1: count = %d\n", count++);

        rt_thread_yield();
    }   
}

static void thread2_entry(void* parameter)
{
    rt_uint32_t count = 0;

    while (1)
    {
        rt_thread_yield();

        rt_kprintf(" thread2: count = %d\n", count++);
    }
}



/* 两者的优先级必须相同，否则一个线程让出调度器之后进入就绪队列， 
   其优先级仍比另一个高，会继续执行，使得另一个线程始终无法得到
   运行。 */
int rt_application_init()
{
    tid1 = rt_thread_create("t1", 
        thread1_entry,
        RT_NULL,
        512,
        6, 10);
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    tid2 = rt_thread_create("t2", 
        thread2_entry,
        RT_NULL,
        512,
        6, 10);
    if (tid2 != RT_NULL)
        rt_thread_startup(tid2);

    return 0;
}   



/*@}*/
