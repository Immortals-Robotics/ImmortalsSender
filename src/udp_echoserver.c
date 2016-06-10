/**
  ******************************************************************************
  * @file    udp_echoserver.c
  * @author  MCD Application Team
  * @version V1.0.2
  * @date    06-June-2011
  * @brief   UDP echo server
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
#include "main.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include <string.h>
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define UDP_SERVER_PORT    60005   /* define the UDP local connection port */
#define UDP_CLIENT_PORT    60006   /* define the UDP remote connection port */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void udp_echoserver_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port);

/* Private functions ---------------------------------------------------------*/

struct udp_pcb *upcb;

/**
  * @brief  Initialize the server application.
  * @param  None
  * @retval None
  */
void udp_echoserver_init(void)
{
   
   err_t err;
   
   /* Create a new UDP control block  */
   upcb = udp_new();
   
   if (upcb)
   {
     /* Bind the upcb to the UDP_PORT port */
     /* Using IP_ADDR_ANY allow the upcb to be used by any local interface */
      err = udp_bind(upcb, IP_ADDR_ANY, UDP_SERVER_PORT);
      
      if(err == ERR_OK)
      {
        /* Set a receive callback for the upcb */
        udp_recv(upcb, udp_echoserver_receive_callback, NULL);
      }
      else
      {
        printf("can not bind pcb");
      }
   }
   else
   {
     printf("can not create pcb");
   } 
}

/**
  * @brief This function is called when an UDP datagrm has been received on the port UDP_PORT.
  * @param arg user supplied argument (udp_pcb.recv_arg)
  * @param pcb the udp_pcb which received data
  * @param p the packet buffer that was received
  * @param addr the remote IP address from which the packet was received
  * @param port the remote port from which the packet was received
  * @retval None
  */

void delay_ms(int ms)
{
	int i , j;
	for(j=0;j<ms;j++)
	{
		for(i=0;i<30000;i++)
		{
		}
	}
}

void delay_us(int ms)
{
	int i , j;
	for(j=0;j<ms;j++)
	{
		for(i=0;i<30;i++)
		{
		}
	}
}
extern void setNumber(int);
extern void beep(int delay);
extern int fps;

struct ip_addr boz;
struct pbuf *ansBuf;
extern int rx_channel;
extern int tx_channel;
extern int firstPacketRecieved;

void process_incoming_rf(void)
{
	if(nrf24l02_irq_pin_active())
	{		
		unsigned char recData[32];
		nrf24l02_read_rx_payload(recData,32);				
		struct pbuf* ansBuf = pbuf_alloc(PBUF_TRANSPORT,32, PBUF_POOL);
		struct ip_addr boz;
		IP4_ADDR(&boz, 224, 5 , 23, 3);							 																									   
		pbuf_take(ansBuf, (unsigned char*)recData, 32);
		udp_sendto( upcb , ansBuf , &boz , UDP_CLIENT_PORT);
		pbuf_free(ansBuf);							 								 
		 
		 nrf24l02_irq_clear_rx_dr();
	}
}

void Process_recieved_Packet(unsigned char * data , int len , struct udp_pcb *upcb)
{
	int nextPacket=0;
	unsigned char address[5]={110,110,8,110,110};	
	int timeOut = 40;
	while(nextPacket < len)
	{
		 if(data[nextPacket] == 80)
		 {
			   if(data[nextPacket + 1] == data[nextPacket + 7] && rx_channel != data[nextPacket+1])
				 {
					  nrf24l01_set_rf_ch(data[nextPacket + 1]);
					  rx_channel = data[nextPacket+1];
					  beep(100);
					  nextPacket+=10;
				 }
		 }
		 else if(data[nextPacket] == 81)
		 {
			   if(data[nextPacket + 1] == data[nextPacket + 7] && tx_channel != data[nextPacket+1])
				 {
					  nrf24l02_set_rf_ch(data[nextPacket + 1]);
					  tx_channel = data[nextPacket+1];
					  beep(100);
					  nextPacket+=10;
				 }
		 }
		 else
		 {
				 address[2] = data[nextPacket];
				 nrf24l01_set_tx_addr(address , 5);
				 nrf24l01_write_tx_payload(data + nextPacket + 1 , 32 , true);				 
				 
				 while(!nrf24l01_irq_pin_active());
				 nrf24l01_irq_clear_all();
			 
				 nextPacket+=33;
		 }
	}
}

void udp_echoserver_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{
	unsigned char * cc = p->payload;
	int len = p->len;
	
	fps++;
	firstPacketRecieved=1;
	Process_recieved_Packet(cc,len,upcb);
	pbuf_free(p);
	return;   
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
