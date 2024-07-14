#include <string.h>
#include <stdio.h>

#define UDP_SERVER_PORT    60005   /* define the UDP local connection port */
#define UDP_CLIENT_PORT    60006   /* define the UDP remote connection port */

extern int fps;

int checkFirst = 0;
struct ip_addr boz;
struct pbuf *ansBuf;
extern int Channel;
extern int firstPacketRecieved;

int packetLen=0;

void Process_recieved_Packet(char * data , int len , struct udp_pcb *upcb)
{
    fps++;
	firstPacketRecieved=1;

	int nextPacket=0;
	unsigned char address[5]={110,110,8,110,110};
	unsigned char recData[10];
	int timeOut = 40;

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
