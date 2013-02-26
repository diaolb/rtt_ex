#include <rtthread.h>

static rt_timer_t timer1;
static rt_uint8_t count;

static void timeout1(void *parameter)
{
    rt_kprintf("periodic timer is timeout, %d.\n", count);
    
    count++;
    
    if (count >= 8)
    {
        rt_kprintf("stop it!\n");
        
        rt_timer_stop(timer1);
        
        count = 0;
    }
}

int rt_application_init()
{
    timer1 = rt_timer_create("timer1",
        timeout1,
        RT_NULL,
        10,
        RT_TIMER_FLAG_PERIODIC);
    if (timer1 != RT_NULL)
        rt_timer_start(timer1);
        
    return 0;
}

