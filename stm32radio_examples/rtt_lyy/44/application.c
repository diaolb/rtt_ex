#include <rtthread.h>

static rt_timer_t timer1;
//static rt_timer_t timer2;
static rt_uint8_t count;
static rt_tick_t tick;

static void timeout1(void* parameter)
{
    rt_tick_t timeout = 3000;
    rt_kprintf("periodic timer is timeout\n");
    
    rt_kprintf("the time set is %d\n", (rt_tick_get()-tick));
    tick = rt_tick_get();
    count ++;
   
    if (count >= 8)
    {
        rt_timer_control(timer1, RT_TIMER_CTRL_SET_ONESHOT, (void *)&timeout);
    
        rt_timer_control(timer1, RT_TIMER_CTRL_SET_TIME, (void *)&timeout);
        count = 0;
    }
}

int rt_application_init()
{
    timer1 = rt_timer_create("timer1",  
        timeout1, 
        RT_NULL, 
        100, 
        RT_TIMER_FLAG_PERIODIC); 
    
    if (timer1 != RT_NULL)
        rt_timer_start(timer1);
  
    return 0;
}

