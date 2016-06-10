/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V1.0.2
  * @date    06-June-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f2x7_eth.h"
#include "netconf.h"
#include "main.h"
#include "udp_echoserver.h"
#include "serial_debug.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SYSTEMTICK_PERIOD_MS  10

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t LocalTime = 0; /* this variable is used to create a time reference incremented by 10ms */
uint32_t timingdelay;

/* Private functions ---------------------------------------------------------*/

uint16_t Segment1TableE [10] =  {0x13  , 0x01 , 0x0B  , 0x0B , 0x19   , 0x1A  , 0x1A  , 0x03 , 0x1B  , 0x1B }; 
uint16_t Segment1TableB [10] =  {0x380 , 0x80 , 0x300 , 0x180 , 0x080 , 0x180 , 0x380 , 0x80 , 0x380 , 0x180}; 

uint16_t Segment2TableD [10] =  {0x07  , 0x01 , 0x03  , 0x03 , 0x05   , 0x06, 0x06  , 0x03  , 0x07 , 0x07 }; 
uint16_t Segment2TableG [10] =  {0x3400 , 0x0400 , 0x3800 , 0x1C00 , 0x0C00 , 0x1C00 , 0x3C00 , 0x0400 , 0x3C00 , 0x1C00}; 


TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
uint16_t CCR3_Val = 166;
uint16_t PrescalerValue = 0;

int fps = 0;

void init_Timer()
{

	  GPIO_InitTypeDef GPIO_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;
	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	  uint16_t PrescalerValue = 0;

	  /* TIM3 clock enable */
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	  /* Enable the TIM3 gloabal Interrupt */
	  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);
	
	  /* ---------------------------------------------------------------
	    TIM3 Configuration: Output Compare Timing Mode:
	    TIM3 counter clock at 6 MHz
	    CC1 Update Rate : 128 Hz
	  --------------------------------------------------------------- */
	
	  /* Compute the prescaler value */
	  PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 6000000) - 1;
	
	  /* Time base configuration */
	  TIM_TimeBaseStructure.TIM_Period = 6000000 -1; //46875;
	  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	  /* Prescaler configuration */
	  TIM_PrescalerConfig(TIM2, PrescalerValue, TIM_PSCReloadMode_Immediate);
	   
	  /* TIM Interrupts enable */
	  TIM_ITConfig(TIM2, TIM_IT_CC1 , ENABLE);
	
	  /* TIM3 enable counter */
	  TIM_Cmd(TIM2, ENABLE);
		
	return;
  /* TIM3 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  /* GPIOC clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  
  /* GPIOC Configuration: TIM3 CH1 (PC6), TIM3 CH2 (PC7), TIM3 CH3 (PC8) and TIM3 CH4 (PC9) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOE, &GPIO_InitStructure); 

  /* Connect TIM3 pins to AF2 */  
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
	
	PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 20000000) - 1;

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period =1000000;
  TIM_TimeBaseStructure.TIM_Prescaler = 1000;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_Pulse = 45;
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM1, ENABLE);

  CCR3_Val = 100;
  /* TIM3 enable counter */
  TIM_Cmd(TIM1, ENABLE);
	
}

void offSegment(int a)
{
	if(a%2==1)
	{
	 GPIOD->BSRRH |= Segment2TableD[8];
	 GPIOG->BSRRH |= Segment2TableG[8];
	}
	a = a/2;
	if(a%2==1)
	{
		GPIOE->BSRRH |= Segment1TableE[8];
	  GPIOB->BSRRH |= Segment1TableB[8];
	}
}

void setNumber(int a)
{
	 GPIOD->BSRRH |= Segment2TableD[8];
	 GPIOG->BSRRH |= Segment2TableG[8];
	 GPIOD->BSRRL |= Segment2TableD[a%10];
	 GPIOG->BSRRL |= Segment2TableG[a%10];	
	
	 a = a/10;
	
	 GPIOE->BSRRH |= Segment1TableE[8];
	 GPIOB->BSRRH |= Segment1TableB[8];
	 GPIOE->BSRRL |= Segment1TableE[a%10];
	 GPIOB->BSRRL |= Segment1TableB[a%10];	

	 
}

void beep(int delay)
{
		GPIOE->BSRRL = GPIO_Pin_13;
		delay_ms(delay);
		GPIOE->BSRRH = GPIO_Pin_13;
}

void setLed(int i)
{
	if(i)
	{
		GPIOG->BSRRL = GPIO_Pin_13;
	}
	else
	{
		GPIOG->BSRRH = GPIO_Pin_13;
	}
}

void initLED()
{
		 GPIO_InitTypeDef  GPIO_InitStructure; 
     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_10 ;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
     GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
     GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(GPIOG, &GPIO_InitStructure);
	
     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_7 | GPIO_Pin_6 ;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
     GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
     GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(GPIOB, &GPIO_InitStructure);
			 
     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_3 | GPIO_Pin_1 | GPIO_Pin_0 |GPIO_Pin_13 ;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
     GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
     GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(GPIOE, &GPIO_InitStructure);
		 
		 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
     GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
     GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(GPIOD, &GPIO_InitStructure);
		 		 
}


void init_NRF1_IO()
{
	   GPIO_InitTypeDef  GPIO_InitStructure; 
     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
     GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
     GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(GPIOE, &GPIO_InitStructure);
	
		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
     GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
     GPIO_Init(GPIOE, &GPIO_InitStructure);
	
		 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
     GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
     GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
}

void init_NRF2_IO()
{
	   GPIO_InitTypeDef  GPIO_InitStructure; 
     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_15;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
     GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
     GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
     GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
     GPIO_Init(GPIOD, &GPIO_InitStructure);		
	
}

USART_InitTypeDef USART_InitStructure;

void USART2_Init()
{	
		GPIO_InitTypeDef GPIO_InitStructure;

		/* Enable GPIO clock */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

		/* Connect PXx to USARTx_Tx*/
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);

		/* Connect PXx to USARTx_Rx*/
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);

		/* Configure USART Tx as alternate function  */
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOD, &GPIO_InitStructure);

		/* Configure USART Rx as alternate function  */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		
		USART_InitStructure.USART_BaudRate = 38400;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART2, &USART_InitStructure);
		USART_Cmd(USART2, ENABLE);
		
}
SPI_InitTypeDef  SPI_InitStructure;

void USART3_Init()
{
		GPIO_InitTypeDef GPIO_InitStructure;

		/* Enable GPIO clock */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

		/* Connect PXx to USARTx_Tx*/
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);

		/* Connect PXx to USARTx_Rx*/
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);

		/* Configure USART Tx as alternate function  */
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOD, &GPIO_InitStructure);

		/* Configure USART Rx as alternate function  */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		
		USART_InitStructure.USART_BaudRate = 38400;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART3, &USART_InitStructure);
		USART_Cmd(USART3, ENABLE);
}


void SPI3_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the SPI clock */
  SPIy_CLK_INIT(SPIy_CLK, ENABLE);

  /* Enable GPIO clocks */
  RCC_AHB1PeriphClockCmd(SPIy_SCK_GPIO_CLK | SPIy_MISO_GPIO_CLK | SPIy_MOSI_GPIO_CLK, ENABLE);

  /* Connect SPI pins to AF5 */  
  GPIO_PinAFConfig(SPIy_SCK_GPIO_PORT, SPIy_SCK_SOURCE, SPIy_SCK_AF);
  GPIO_PinAFConfig(SPIy_MOSI_GPIO_PORT, SPIy_MOSI_SOURCE, SPIy_MOSI_AF);
	GPIO_PinAFConfig(SPIy_MISO_GPIO_PORT, SPIy_MISO_SOURCE, SPIy_MOSI_AF);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

  /* SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = SPIy_SCK_PIN;
  GPIO_Init(SPIy_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  SPIy_MOSI_PIN;
  GPIO_Init(SPIy_MOSI_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  SPIy_MISO_PIN;
  GPIO_Init(SPIy_MISO_GPIO_PORT, &GPIO_InitStructure);
 
  /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(SPIy);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 0;
	SPI_Init(SPIy, &SPI_InitStructure);
  
  /* Configure the Priority Group to 1 bit */                
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* Configure the SPI interrupt priority */
  NVIC_InitStructure.NVIC_IRQChannel = SPI3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

int checkFirstoo;
int firstPacketRecieved = 0;

void demo()
{
	uint8_t cc[33] = {0};
	unsigned char address[5]={110,110,8,110,110};
	
	cc[0] = 25;
	cc[1] = 1;
	
				 if(checkFirstoo !=0)
				 {
						while(!nrf24l01_irq_pin_active());
						//beep();
						nrf24l01_irq_clear_all();
				 }
				 address[2] = cc[0];
				 nrf24l01_set_tx_addr(address , 5);
				 nrf24l01_write_tx_payload(cc + 1 , 32 , true);				 
				 
				  if(checkFirstoo<2)
						checkFirstoo++;
					
		
	
}


/**
  * @brief  Configures the SPI Peripheral.
  * @param  None
  * @retval None
  */

void SPI1_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the SPI clock */
  SPIx_CLK_INIT(SPIx_CLK, ENABLE);

  /* Enable GPIO clocks */
  RCC_AHB1PeriphClockCmd(SPIx_SCK_GPIO_CLK | SPIx_MISO_GPIO_CLK | SPIx_MOSI_GPIO_CLK, ENABLE);

  /* Connect SPI pins to AF5 */  
  GPIO_PinAFConfig(SPIx_SCK_GPIO_PORT, SPIx_SCK_SOURCE, SPIx_SCK_AF);
  GPIO_PinAFConfig(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_SOURCE, SPIx_MOSI_AF);
	GPIO_PinAFConfig(SPIx_MISO_GPIO_PORT, SPIx_MISO_SOURCE, SPIx_MOSI_AF);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

  /* SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = SPIx_SCK_PIN;
  GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  SPIx_MOSI_PIN;
  GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  SPIx_MISO_PIN;
  GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStructure);
 
  /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(SPIx);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 0;
	SPI_Init(SPIx, &SPI_InitStructure);
  
  /* Configure the Priority Group to 1 bit */                
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* Configure the SPI interrupt priority */
  NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

uint16_t data[1000]={0};
unsigned char str [40];
uint16_t dat = 11;
int tx_channel = 110;
int rx_channel = 80;

extern void process_incoming_rf(void);

int main(void)
{
	int i;		
	unsigned char add[5]={110,110,8,110,110};
  /*!< At this stage the microcontroller clock setting is already configured to 
       120 MHz, this is done through SystemInit() function which is called from
       startup file (startup_stm32f2xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f2xx.c file
     */
  
#ifdef SERIAL_DEBUG
  DebugComPort_Init();
#endif
  
	initLED();
	init_Timer();
	//while(1);
	
  /*Initialize LCD and Leds */ 
  //LCD_LED_Init();
	
	delay_ms(500);				
	
	for(i=0;i<100;i++)
	{
		setNumber(i);
		delay_ms(10);				
	}
	beep(40);
	offSegment(3);
	delay_ms(100);
	setNumber(0);
	delay_ms(100);
	beep(40);
	offSegment(3);
	delay_ms(100);
	setNumber(0);
	delay_ms(100);
	beep(40);
	
	//USART2_Init();
	//USART3_Init();
	
	init_NRF1_IO();
	init_NRF2_IO();
	SPI1_Config();
	SPI3_Config();
	SPI_Cmd(SPI1, ENABLE);
	SPI_Cmd(SPI3, ENABLE);
	
  nrf24l01_initialize_debug(false, 32, false);
	nrf24l01_clear_flush();
	add[2]=8;
	nrf24l01_set_tx_addr(add , 5);
	add[2]=30;
	nrf24l01_set_rx_addr(add,5,0);
	nrf24l01_set_rf_ch(tx_channel);


	nrf24l02_initialize_debug(false, 32, false);	
 	nrf24l02_clear_flush();
	add[2]=8;
	nrf24l02_set_tx_addr(add , 5);
	add[2]=30;
	nrf24l02_set_rx_addr(add,5,0);	
	nrf24l02_set_rf_ch(rx_channel);
	nrf24l02_set_as_rx(true);		
	
  /* configure ethernet */ 
  ETH_BSP_Config();
    
  /* Initilaize the LwIP stack */
  LwIP_Init();
  
  /* UDP echoserver */
  udp_echoserver_init();
  
    
  /* Infinite loop */
  while (1)
  {  
    /* check if any packet received */
					
    if (ETH_CheckFrameReceived())
    {
      /* process received ethernet packet */
      LwIP_Pkt_Handle();
    }
    /* handle periodic timers for LwIP */
    LwIP_Periodic_Handle(LocalTime);
		
		process_incoming_rf();
		
		if(firstPacketRecieved==0)
		{
			demo();
			setNumber(22);			
		}
							
  }   
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: number of 10ms periods to wait for.
  * @retval None
  */
void Delay(uint32_t nCount)
{
  /* Capture the current local time */
  timingdelay = LocalTime + nCount;  

  /* wait until the desired delay finish */  
  while(timingdelay > LocalTime)
  {     
  }
}

/**
  * @brief  Updates the system local time
  * @param  None
  * @retval None
  */
void Time_Update(void)
{
  LocalTime += SYSTEMTICK_PERIOD_MS;
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
