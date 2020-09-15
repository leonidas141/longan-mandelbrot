#include "lcd/lcd.h"
#include <string.h>

unsigned char image[12800];
int w,h;
int xaxis;
int yaxis;
int scalew;
int scaleh;
int maxit;
void init_uart0(void)
{	
	/* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);

    /* connect port to USARTx_Tx */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    /* connect port to USARTx_Rx */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

	/* USART configure */
    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200U);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);

    usart_interrupt_enable(USART0, USART_INT_RBNE);
}

int mandelbrot(int z1, int z2)
{
    float x1 = 0.0;
    float x2 = 0.0;
    // f(z) = z^2 +c
    float c1 = (float)(z1-120)/scalew;
    float c2 = (float)(z2-40)/scaleh;
    float f1 = x1 * x1 - x2 * x2 + c1;
    float f2 = -2 * x1 * x2 + c2;
    for(int i=0;i<maxit;i++)
    {
        f1 = x1 * x1 - x2 * x2 + c1;
        f2 = 2 * x1 * x2 + c2;
        x1 = f1;
        x2 = f2;
        if(f1*f1+f2*f2>=4.0)
        {
            return i;
        }
    }
    return 255 - (int)(f1*f1+f2*f2);
    
}

int main(void)
{
    w = 160;
    h = 80;
    xaxis = w;
    yaxis = h/2;
    maxit = 60;
    scalew = 40;
    scaleh = 40;
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1|GPIO_PIN_2);

    init_uart0();

    Lcd_Init();			// init OLED
    LCD_Clear(WHITE);
    BACK_COLOR=WHITE;
    int ans;
    while(1)
    {
        LCD_Address_Set(0,0,159,79);
        for(int i=0;i<h;i++)
        {
            for(int j=0;j<w;j++)
            {
                ans = mandelbrot(j,i);
                LCD_WR_DATA8((unsigned char)ans*8);
                LCD_WR_DATA8((unsigned char)ans*4);
            }
        }
        delay_1ms(1000);
    }
}
