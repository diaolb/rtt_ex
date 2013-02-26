#include <rtthread.h>

static struct rt_semaphore static_sem;

static rt_sem_t dynamic_sem = RT_NULL;

static rt_uint8_t thread1_stack[1024];
struct rt_thread thread1;
static void rt_thread_entry1(void *parameter)
{
    rt_err_t result;
    rt_tick_t tick;

    /* static semaphore demo */
    tick = rt_tick_get();

    /* try to take the sem, wait 10 ticks */
    result = rt_sem_take(&static_sem, 10);

    if (result == -RT_ETIMEOUT)
    {
        if (rt_tick_get() - tick != 10)
        {
            rt_sem_detach(&static_sem);
            return ;
        }
        rt_kprintf("take semaphore timeout\n");
    }   
    else 
    {
        rt_kprintf("take a static semaphore, failed.\n");
        rt_sem_detach(&static_sem);
        return ;
    }   

    /* release the semaphore */     
    rt_sem_release(&static_sem);  
    /* wait the semaphore forever */
    result = rt_sem_take(&static_sem, RT_WAITING_FOREVER);

    if (result != RT_EOK)
    {
        rt_kprintf("take a static semaphore, failed.\n");
        rt_sem_detach(&static_sem);
        return ;
    }

    rt_kprintf("take a static semaphore, done.\n");
    /* detach the semaphore object */
    rt_sem_detach(&static_sem);
//}

/* dynamic thread pointer */
//static void thread2_entry(void *parameter)
//{
//    rt_err_t result;
//    rt_tick_t tick;

    tick = rt_tick_get();

    /* try to take the semaphore, wait for 10 ticks */
    result = rt_sem_take(dynamic_sem, 10);
    if (result == -RT_ETIMEOUT)
    {
        if (rt_tick_get() - tick != 10)
        {
            rt_sem_delete(dynamic_sem);
            return ;
        }        
        rt_kprintf("take semaphore timeout\n");
    }
    else
    {
        rt_kprintf("take a dynamic semaphore, failed.\n");
        rt_sem_delete(dynamic_sem);
        return ;
    }

    /* release the dynamic semaphore */
    rt_sem_release(dynamic_sem);
    /* wait forever */
    result = rt_sem_take(dynamic_sem, RT_WAITING_FOREVER);

    if (result != RT_EOK)
    {
        rt_kprintf("take a dynamic semaphore, failed.\n");
        rt_sem_delete(dynamic_sem);
        return ;
    }

    rt_kprintf("take a dynamic semaphore, done.\n");
    /* delete the semaphore*/
    rt_sem_delete(dynamic_sem);
}
//static rt_thread_t tid = RT_NULL;
int rt_application_init()
{
    rt_err_t  result;

    result = rt_sem_init(&static_sem,
        "ssem",
        0, RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        rt_kprintf("init static semaphore failed. \n");
        return -1;
    }

    dynamic_sem = rt_sem_create("dsem",
        0, RT_IPC_FLAG_FIFO);
    if (dynamic_sem == RT_NULL)
    {
        rt_kprintf("create dynamic semaphore failed. \n");
        return -1;
    }

    /* thread1 init */
    rt_thread_init(&thread1, 
        "t1", 
        rt_thread_entry1, RT_NULL,
        &thread1_stack[0], sizeof(thread1_stack),
        11, 5
        );
    rt_thread_startup(&thread1);

    return 0;
}

/*@}*/  

                 