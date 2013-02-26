#include <rtthread.h>

static struct rt_timer timer1;
static struct rt_timer timer2;
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
    rt_timer_init(&timer1, "timer1",  
        timeout1, 
        RT_NULL, 
        1000, 
        RT_TIMER_FLAG_PERIODIC); 
    
    rt_timer_start(&timer1);
    
    
    rt_timer_init(&timer2, "timer2",  
        timeout2, 
        RT_NULL,
        3000,
        RT_TIMER_FLAG_ONE_SHOT); 
    
    rt_timer_start(&timer2);

    
        
    return 0;
}

