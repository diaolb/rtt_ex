#include <rtthread.h>

struct rt_thread thread1;
static rt_uint8_t thread1_stack[512];
static void thread1_entry(void *parameter)
{
    int i,j;
    char *ptr[20];
    
    for (j = 0; j < 20; j++)
        ptr[j] = RT_NULL;
    
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 20; j++)
        {
            ptr[j] = rt_malloc(1 << j);
            
            if (ptr[j] != RT_NULL)
            {
                rt_kprintf("get memory: 0x%x.\n", ptr[j]);
                
                rt_free(ptr[j]);
                ptr[j] = RT_NULL;
            }
        }  
    }   
}

int rt_application_init()
{
    rt_thread_init(&thread1,
        "thread1",
        thread1_entry, RT_NULL,
        &thread1_stack[0], sizeof(thread1_stack),
        10, 100);
    rt_thread_startup(&thread1);
    
    return 0;
}
