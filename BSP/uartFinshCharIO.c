
#include "uartFinshCharIO.h"

/******RingBuffer-Begin******/
#define rt_ringbuffer_space_len(rb) ((rb)->buffer_size - rt_ringbuffer_data_len(rb))

struct rt_ringbuffer
{
    rt_uint8_t *buffer_ptr;

    rt_uint16_t read_mirror : 1;
    rt_uint16_t read_index : 15;
    rt_uint16_t write_mirror : 1;
    rt_uint16_t write_index : 15;

    rt_int16_t buffer_size;
};

enum rt_ringbuffer_state
{
    RT_RINGBUFFER_EMPTY,
    RT_RINGBUFFER_FULL,
    /**half full is neither full nor empty */
    RT_RINGBUFFER_HALFFULL,
};

rt_inline enum rt_ringbuffer_state rt_ringbuffer_status(struct rt_ringbuffer *rb)
{
    if (rb->read_index == rb->write_index)
    {
        if (rb->read_mirror == rb->write_mirror)
            return RT_RINGBUFFER_EMPTY;
        else
            return RT_RINGBUFFER_FULL;
    }
    return RT_RINGBUFFER_HALFFULL;
}

/** 
 * get the size of data in rb 
 */
rt_size_t rt_ringbuffer_data_len(struct rt_ringbuffer *rb)
{
    switch (rt_ringbuffer_status(rb))
    {
    case RT_RINGBUFFER_EMPTY:
        return 0;
    case RT_RINGBUFFER_FULL:
        return rb->buffer_size;
    case RT_RINGBUFFER_HALFFULL:
    default:
        if (rb->write_index > rb->read_index)
            return rb->write_index - rb->read_index;
        else
            return rb->buffer_size - (rb->read_index - rb->write_index);
    };
}

void rt_ringbuffer_init(struct rt_ringbuffer *rb,
                        rt_uint8_t           *pool,
                        rt_int16_t            size)
{
    RT_ASSERT(rb != RT_NULL);
    RT_ASSERT(size > 0);

    /**initialize read and write index */
    rb->read_mirror = rb->read_index = 0;
    rb->write_mirror = rb->write_index = 0;

    /**set buffer pool and size */
    rb->buffer_ptr = pool;
    rb->buffer_size = RT_ALIGN_DOWN(size, RT_ALIGN_SIZE);
}

/**
 * put a character into ring buffer
 */
rt_size_t rt_ringbuffer_putchar(struct rt_ringbuffer *rb, const rt_uint8_t ch)
{
    RT_ASSERT(rb != RT_NULL);

    /**whether has enough space */
    if (!rt_ringbuffer_space_len(rb))
        return 0;

    rb->buffer_ptr[rb->write_index] = ch;

    /**flip mirror */
    if (rb->write_index == rb->buffer_size-1)
    {
        rb->write_mirror = ~rb->write_mirror;
        rb->write_index = 0;
    }
    else
    {
        rb->write_index++;
    }

    return 1;
}
/**
 * get a character from a ringbuffer
 */
rt_size_t rt_ringbuffer_getchar(struct rt_ringbuffer *rb, rt_uint8_t *ch)
{
    RT_ASSERT(rb != RT_NULL);

    /**ringbuffer is empty */
    if (!rt_ringbuffer_data_len(rb))
        return 0;

    /**put character */
    *ch = rb->buffer_ptr[rb->read_index];

    if (rb->read_index == rb->buffer_size-1)
    {
        rb->read_mirror = ~rb->read_mirror;
        rb->read_index = 0;
    }
    else
    {
        rb->read_index++;
    }

    return 1;
}

/******RingBuffer-End******/


/******IO Port******/
rt_uint8_t uart_rx_buf[FinshUartRxBufLen] = {0};
struct rt_ringbuffer  uart_rxcb;         /**???????????? ringbuffer cb */
static struct rt_semaphore shell_rx_sem; /**??????????????????????????? */

/*
 * @brief ??????????????????????????????RTT?????????
 * @retn  ????????????
*/
char rt_hw_console_getchar(void)
{
    char ch = 0;

    /**??? ringbuffer ??????????????? */
    while (rt_ringbuffer_getchar(&uart_rxcb, (rt_uint8_t *)&ch) != 1)
    {
        rt_sem_take(&shell_rx_sem, RT_WAITING_FOREVER);
    } 
    return ch;   
}

/*
 * @brief ??????????????????IO
*/
void FinshIoInit()
{
    rt_ringbuffer_init(&uart_rxcb, uart_rx_buf, FinshUartRxBufLen);
    
    rt_sem_init(&(shell_rx_sem), "shell_rx", 0, 0);
    
    __HAL_UART_ENABLE_IT(&FinshUartHandle, UART_IT_RXNE);
}

/*
 * @brief ??????????????????????????????RTT?????????
 * @arg1  ??????????????????
*/
void rt_hw_console_output(const char *str)
{
    HAL_UART_Transmit(&FinshUartHandle,(uint8_t *)str,rt_strlen(str),0xff);
}

void FinshUartIRQHandler(void)
{
    int ch = -1;
    /**enter interrupt */
    rt_interrupt_enter();          //??????????????????????????????????????????????????????

    if ((__HAL_UART_GET_FLAG(&(FinshUartHandle), UART_FLAG_RXNE) != RESET) &&
        (__HAL_UART_GET_IT_SOURCE(&(FinshUartHandle), UART_IT_RXNE) != RESET))
    {
        while (1)
        {
            ch = -1;
            if (__HAL_UART_GET_FLAG(&(FinshUartHandle), UART_FLAG_RXNE) != RESET)
            {
                ch =  FinshUartHandle.Instance->RDR & 0xff;
            }
            if (ch == -1)
            {
                break;
            }  
            /**????????????????????????????????? ringbuffer */
            rt_ringbuffer_putchar(&uart_rxcb, ch);
        }        
        rt_sem_release(&shell_rx_sem);
    }

    /**leave interrupt */
    rt_interrupt_leave();    //??????????????????????????????????????????????????????
}

