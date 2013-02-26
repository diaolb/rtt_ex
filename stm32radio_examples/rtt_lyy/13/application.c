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



/* ���ߵ����ȼ�������ͬ������һ���߳��ó�������֮�����������У� 
   �����ȼ��Ա���һ���ߣ������ִ�У�ʹ����һ���߳�ʼ���޷��õ�
   ���С� */
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
