#include <rtthread.h>

static rt_timer_t timer1;
static rt_timer_t timer2;
static rt_uint8_t count;

static void timeout1(void* parameter)
{
    rt_kprintf("periodic timer is timeout\n");
}

static void timeout2(void* parameter)
{
    rt_kprintf("one shot timer is timeout\n");
}
 



int rt_application_init()
{
    timer1 = rt_timer_create("timer1",  
        timeout1, 
        RT_NULL, 
        1000, 
        RT_TIMER_FLAG_PERIODIC); 
    if (timer1 != RT_NULL)
        rt_timer_start(timer1);
    
    
    timer2 = rt_timer_create("timer2",  
        timeout2, 
        RT_NULL,
        3000,
        RT_TIMER_FLAG_ONE_SHOT); 
    if (timer2 != RT_NULL)
        rt_timer_start(timer2);

    
        
    return 0;
}

