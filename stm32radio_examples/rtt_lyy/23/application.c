#include <rtthread.h>

#include "stm32f10x.h"

#define KEY_PORT    GPIOB
#define KEY_PIN     (GPIO_Pin_0 | GPIO_Pin_1)
#define KEY_CLOCK   RCC_AHB1Periph_GPIOB

void rt_init_thread_entry(void *parameter)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//¿ªÆôPB0Ê±ÖÓ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}   

static rt_uint8_t thread1_stack[1024];
struct rt_thread thread1;

static int key;
static struct rt_semaphore sem;

static void rt_thread_entry1(void *parameter)
{
    int temp;
    
    while (1)
    {
        key = GPIO_ReadInputData(KEY_PORT);
        
        if (key & KEY_PIN)
        {
            temp = key;
            rt_thread_delay(RT_TICK_PER_SECOND / 50);
            key = GPIO_ReadInputData(KEY_PORT);
            if (key == temp);
                rt_sem_release(&sem);
        }
        
        rt_thread_delay(RT_TICK_PER_SECOND / 10);
    }
}

static rt_uint8_t thread2_stack[1024];
struct rt_thread thread2;

//static int key;
static void rt_thread_entry2(void *parameter)
{
    while (1)
    {
        rt_sem_take(&sem, RT_WAITING_FOREVER);
        
        if (key & KEY_PIN)
        {
            rt_kprintf("some keys has been pressed: %x\n", key);
        }
    }
}

int rt_application_init()
{
    rt_thread_t init_thread;
    rt_err_t result;
    
    result = rt_sem_init(&sem, "sem",
        0, RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        rt_kprintf("error, init sem failed!\n");
        return 0;
    }
    
    init_thread = rt_thread_create("init", 
        rt_init_thread_entry,
        RT_NULL,
        2048, 8, 20);
    if (init_thread != RT_NULL)
        rt_thread_startup(init_thread);
    
    /* init thread1 */
    rt_thread_init(&thread1, 
        "keyp", // producer
        rt_thread_entry1, 
        RT_NULL, 
        &thread1_stack[0], sizeof(thread1_stack),
        11, 25);
    rt_thread_startup(&thread1);

    rt_thread_init(&thread2,
        "keyc", // consumer
        rt_thread_entry2, 
        RT_NULL,
        &thread2_stack[0], sizeof(thread2_stack),
        11, 24);
    rt_thread_startup(&thread2);
    
    return 0;
}
/*@}*/  
