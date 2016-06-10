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

/**
  * @brief  Initialize the server application.
  * @param  None
  * @retval None
  */
void udp_echoserver_init(void)
{
   struct udp_pcb *upcb;
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
uint16_t pp;
unsigned char *cc;
char rr;
int toggle=0;
int i;
int rrr;

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
extern int fps;

int checkFirst = 0;
struct ip_addr boz;
struct pbuf *ansBuf;
extern int Channel;
extern int firstPacketRecieved;

int packetLen=0;
char tmp[100];

void Process_recieved_Packet(char * data , int len , struct udp_pcb *upcb)
{
	int nextPacket=0;
	unsigned char address[5]={110,110,8,110,110};	
	unsigned char recData[10];
	int timeOut = 40;
  memcpy(tmp,data,len);	
	
	while(nextPacket < len)
	{
		 packetLen=0;
		 if(data[nextPacket] == 80)
		 {
			   if(data[nextPacket + 1] == data[nextPacket + 7] && Channel != data[nextPacket+1])
				 {
					  nrf24l02_set_rf_ch(data[nextPacket + 1]);
					  nrf24l01_set_rf_ch(data[nextPacket + 1]);
					  Channel = data[nextPacket+1];
					  beep(100);
					  nextPacket+=10;
				 }
		 }
		 else
		 {
			   if(checkFirst !=0)
				 {
						while(!nrf24l02_irq_pin_active());
						//beep();
						nrf24l02_irq_clear_all();
				 }
				 address[2] = data[nextPacket];
				 packetLen = data[nextPacket+1];
				 nrf24l02_set_tx_addr(address , 5);
				 nrf24l02_write_tx_payload(data + nextPacket + 2 , packetLen , true);				 
				 
				  if(checkFirst<2)
						checkFirst++;
					
				 if(data[nextPacket + 2]>127)
				 {		
						 while(!nrf24l02_irq_pin_active());									
						 nrf24l02_irq_clear_tx_ds();
						 nrf24l02_set_as_rx(true);		
						 delay_us(100);
					   timeOut = 400;	
						 while(timeOut > 0 && !nrf24l02_irq_pin_active())
						 {
								 timeOut--;					
								 delay_us(30);
						 }						 
						 if(timeOut>0 && nrf24l02_irq_rx_dr_active())
						 {			
								 
							    nrf24l02_read_rx_payload(recData,10);				
									ansBuf = pbuf_alloc(PBUF_TRANSPORT,10, PBUF_POOL);
								  IP4_ADDR(&boz, 224, 5 , 23, 3);							 																									   
								  pbuf_take(ansBuf, (char*)recData, 10);							 
									udp_sendto( upcb , ansBuf , &boz , UDP_CLIENT_PORT);	
									pbuf_free(ansBuf);							 								 
						 }						
						 checkFirst=0;
						 nrf24l02_irq_clear_rx_dr();
						 nrf24l02_set_as_tx();
						 delay_us(600);
				 }
				 nextPacket+=packetLen+2;
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
