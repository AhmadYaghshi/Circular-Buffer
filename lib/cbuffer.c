#include "cbuffer.h"

#if ((BUFFER_SIZE < 8U) || (BUFFER_SIZE > 32U))
#error BUFFER_SIZE should be an integer between 8 and 32
#endif

static bool full = false;
static uint8_t tail = 0U;
static uint8_t head = 0U, read = 0U;

static uint8_t elements_count = 0U;
static uint8_t buffer[BUFFER_SIZE] = {0};

void cbuffer_init(void)
{
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        buffer[i] = 0;
    }
    tail = 0U;
    head = 0U;
    read = 0U;
    elements_count = 0U;
    full = false;
}

void cbuffer_write(uint8_t value)
{
    buffer[tail] = value;

    /* if the buffer is full the data gets overwritten. The head is then moved to the next element.
    The elements count doesn't change in this case. */   
    if (cbuffer_isfull())
    {       
        head = (head + 1) % BUFFER_SIZE; 
    }
    else
    {
        elements_count++;
    }   
    tail = (tail + 1) % BUFFER_SIZE;
}

uint8_t cbuffer_read(void)
{   
    /*assign the current value of head to the variable read.
    read will be returned to the user. */
    read = head;

    /* if the buffer is not empty, Move the head to the next element. 
    The elments count decreases by (1) */
    if (cbuffer_available())     
    {       
        head = (head  + 1) % BUFFER_SIZE; 
        elements_count--;      
    }
    return buffer[read];
}

bool cbuffer_isfull(void)
{
    if (elements_count == (BUFFER_SIZE-1))
    {
        full = true;
    }
    else
    {
        full = false;
    }
    return full;
    
}

uint8_t cbuffer_peek(void)
{
    return buffer[head];
}

uint8_t cbuffer_available(void)
{
    return elements_count;
}
