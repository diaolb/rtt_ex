#include <rtthread.h>

void rt_init_thread_entry(void *parameter)
{

}

#define THREAD_STACK_SIZE 1024

#define MAXSEM 5

//rt_uint32_t array[MAXSEM];

/* 用于放置生产的整数数组 */
rt_uint32_t array[MAXSEM];
/* 指向生产者、消费者在array数组中的读写位置 */
static rt_uint32_t set, get;

struct rt_semaphore sem_lock;
struct rt_semaphore sem_empty, sem_full;

/* entry for the producer */
void producer_thread_entry(void *parameter)
{
    int cnt = 0;
    
    while (cnt < 20)
    {
        rt_sem_take(&sem_empty, RT_WAITING_FOREVER);
         
        /* change the contents of the buffer*/
        rt_sem_take(&sem_lock, RT_WAITING_FOREVER);
        array[set%MAXSEM] = cnt + 1;
        rt_kprintf("the producer generates a number: %d\n", array[set%MAXSEM]);
        set++;
        rt_sem_release(&sem_lock);
        
        rt_sem_release(&sem_full);
        cnt++;
    }
    
    rt_kprintf("the producer exit!\n");
}

void consumer_thread_entry(void *parameter)
{
    rt_uint32_t no;
    rt_uint32_t sum;
    
    no = (rt_uint32_t) parameter;
    sum = 0;
    
    while (1)
    {
        rt_sem_take(&sem_full, RT_WAITING_FOREVER);
        
        rt_sem_take(&sem_lock, RT_WAITING_FOREVER);
        sum += array[get%MAXSEM];
        rt_kprintf("the consumer[%d] get a number: %d\n", no, array[get%MAXSEM] );
        get++;
        rt_sem_release(&sem_lock);
        
        rt_sem_release(&sem_empty);
        
        if (get == 20)
            break;
         
        rt_thread_delay(10);
    }
    
    rt_kprintf("the consumer[%d] exits, sum is %d \n ", no, sum);
}

int rt_application_init()
{
    rt_thread_t tid;
    rt_thread_t init_thread;
    rt_err_t result;
    
    result = rt_sem_init(&sem_lock, "lock", 1, RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        rt_kprintf("init semaphore failed.\n");
        return -1;
    }
    
    result = rt_sem_init(&sem_empty, "empty", MAXSEM, RT_IPC_FLAG_FIFO);   
    if (result != RT_EOK)
    {
        rt_kprintf("init empty sem failed.\n");
        return -1;
    } 
    
    result = rt_sem_init(&sem_full, "full", 0, RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        rt_kprintf("init sem failed.\n");
        return -1;
    }
    
    init_thread = rt_thread_create("init", 
            rt_init_thread_entry, RT_NULL,
            2048, 8, 20);
    
    if (init_thread != RT_NULL)
        rt_thread_startup(init_thread);
        
    tid = rt_thread_create("producer", 
        producer_thread_entry,
        RT_NULL,
        THREAD_STACK_SIZE, 10, 5);
    if (tid != RT_NULL)
        rt_thread_startup(tid);
        
    tid = rt_thread_create("consumer1",
        consumer_thread_entry,
        (void *)1,
        THREAD_STACK_SIZE, 11, 2);
    if (tid != RT_NULL)
        rt_thread_startup(tid);
        
    return 0;
}

/*@}*/  

                 