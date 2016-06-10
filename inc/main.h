/**
  ******************************************************************************
  * @file    main.h
  * @author  MCD Application Team
  * @version V1.0.2
  * @date    06-June-2011 
  * @brief   This file contains all the functions prototypes for the main.c 
  *          file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx.h"
#include "stm32f2x7_eth_bsp.h"
#include "nrf24l01_2.h"
#include "nrf24l01.h"
	 
//#include "lwip/igmp.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define USE_DHCP       /* enable DHCP, if disabled static address is used */

/* Uncomment SERIAL_DEBUG to enables retarget of printf to  serial port (COM1 on STM32 evalboard) 
   for debug purpose */   
//#define SERIAL_DEBUG 
	 
#define LWIP_IGMP 1

/* MAC ADDRESS: MAC_ADDR0:MAC_ADDR1:MAC_ADDR2:MAC_ADDR3:MAC_ADDR4:MAC_ADDR5 */
#define MAC_ADDR0   2
#define MAC_ADDR1   0
#define MAC_ADDR2   0
#define MAC_ADDR3   0
#define MAC_ADDR4   0
#define MAC_ADDR5   0
 
/*Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   1
#define IP_ADDR3   208
   
/*NETMASK*/
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

/*Gateway Address*/
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   1
#define GW_ADDR3   1  


#define MULTICAST_ADDR0   224
#define MULTICAST_ADDR1   5
#define MULTICAST_ADDR2   92
#define MULTICAST_ADDR3   5 


/* MII and RMII mode selection, for STM322xG-EVAL Board(MB786) RevB ***********/
//#define RMII_MODE  // In this case the System clock frequency is configured
                     // to 100 MHz, for more details refer to system_stm32f2xx.c      
                                     
#define MII_MODE

/* Uncomment the define below to clock the PHY from external 25MHz crystal (only for MII mode) */
#ifdef 	MII_MODE
 //#define PHY_CLOCK_MCO
#endif

/* STM322xG-EVAL jumpers setting
    +==========================================================================================+
    +  Jumper |       MII mode configuration            |      RMII mode configuration         +
    +==========================================================================================+
    +  JP5    | 2-3 provide 25MHz clock by MCO(PA8)     |  2-3 provide 50MHz clock by MCO(PA8) +
    +         | 1-2 provide 25MHz clock by ext. Crystal |                                      +
    + -----------------------------------------------------------------------------------------+
    +  JP6    |          2-3                            |  1-2                                 +
    + -----------------------------------------------------------------------------------------+
    +  JP8    |          Open                           |  Close                               +
    +==========================================================================================+
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */  
void Time_Update(void);
void Delay(uint32_t nCount);


#ifdef __cplusplus
}
#endif



/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

/* Exported typedef -----------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Exported define ------------------------------------------------------------*/
/* Uncomment the line below if you will use the SPI peripheral as a Master */
#define SPI_MASTER 
/* Uncomment the line below if you will use the SPI peripheral as a Slave */
//#define SPI_SLAVE

/* USER_TIMEOUT value for waiting loops. This timeout is just guarantee that the
   application will not remain stuck if the USART communication is corrupted. 
   You may modify this timeout value depending on CPU frequency and application
   conditions (interrupts routines, number of data to transfer, baudrate, CPU
   frequency...). */ 
#define USER_TIMEOUT                    ((uint32_t)0x64) /* Waiting 1s */

/* SPIx Communication boards Interface */
#define SPIx                           SPI1
#define SPIx_CLK                       RCC_APB2Periph_SPI1 
#define SPIx_CLK_INIT                  RCC_APB2PeriphClockCmd
#define SPIx_IRQn                      SPI1_IRQn
#define SPIx_IRQHANDLER                SPI1_IRQHandler

#define SPIx_SCK_PIN                   GPIO_Pin_5
#define SPIx_SCK_GPIO_PORT             GPIOA
#define SPIx_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOA
#define SPIx_SCK_SOURCE                GPIO_PinSource5
#define SPIx_SCK_AF                    GPIO_AF_SPI1

#define SPIx_MISO_PIN                  GPIO_Pin_6
#define SPIx_MISO_GPIO_PORT            GPIOA
#define SPIx_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOA
#define SPIx_MISO_SOURCE               GPIO_PinSource6
#define SPIx_MISO_AF                   GPIO_AF_SPI1

#define SPIx_MOSI_PIN                  GPIO_Pin_5
#define SPIx_MOSI_GPIO_PORT            GPIOB
#define SPIx_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define SPIx_MOSI_SOURCE               GPIO_PinSource5
#define SPIx_MOSI_AF                   GPIO_AF_SPI1

#define SPIy                           SPI3
#define SPIy_CLK                       RCC_APB1Periph_SPI3 
#define SPIy_CLK_INIT                  RCC_APB1PeriphClockCmd
#define SPIy_IRQn                      SPI3_IRQn
#define SPIy_IRQHANDLER                SPI3_IRQHandler

#define SPIy_SCK_PIN                   GPIO_Pin_10
#define SPIy_SCK_GPIO_PORT             GPIOC
#define SPIy_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOC
#define SPIy_SCK_SOURCE                GPIO_PinSource10
#define SPIy_SCK_AF                    GPIO_AF_SPI3

#define SPIy_MISO_PIN                  GPIO_Pin_11
#define SPIy_MISO_GPIO_PORT            GPIOC
#define SPIy_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOC
#define SPIy_MISO_SOURCE               GPIO_PinSource11
#define SPIy_MISO_AF                   GPIO_AF_SPI3

#define SPIy_MOSI_PIN                  GPIO_Pin_12
#define SPIy_MOSI_GPIO_PORT            GPIOC
#define SPIy_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOC
#define SPIy_MOSI_SOURCE               GPIO_PinSource12
#define SPIy_MOSI_AF                   GPIO_AF_SPI3


#define TXBUFFERSIZE   (countof(TxBuffer) - 1)
#define RXBUFFERSIZE   TXBUFFERSIZE

/* Joystick Commands */
#define CMD_RIGHT                        0x55
#define CMD_LEFT                         0xAA
#define CMD_UP                           0x33
#define CMD_DOWN                         0xCC
#define CMD_SEL                          0xFF 

#define CMD_ACK                          0x66 

/* Define numbers of bytes to transmit from TxBuffer */
#define CMD_RIGHT_SIZE                   0x01
#define CMD_LEFT_SIZE                    0x05
#define CMD_UP_SIZE                      0x14
#define CMD_DOWN_SIZE                    0x1E
#define CMD_SEL_SIZE                     TXBUFFERSIZE

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#ifdef SPI_MASTER
 uint8_t GetVar_NbrOfData(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
