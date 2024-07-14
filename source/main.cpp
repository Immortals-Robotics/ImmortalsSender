#include "main.h"
#include "udp_echoserver.h"

int fps = 0;

void setNumber(int a)
{

}

void beep(int delay)
{

}

void setLed(int i)
{

}

int checkFirstoo;
int firstPacketRecieved = 0;

void demo()
{
	uint8_t cc[20];
	unsigned char address[5]={110,110,8,110,110};

	cc[0] = 25;
	cc[1] = 1;
	cc[2] = 0;
	cc[3] = 0;
	cc[4] = 0;
	cc[5] = 0;
	cc[6] = 0;
	cc[7] = 0;
	cc[8] = 0;
	cc[9] = 0;
	cc[10] = 0;

				 if(checkFirstoo !=0)
				 {
						while(!nrf24l01_irq_pin_active());
						//beep();
						nrf24l01_irq_clear_all();
				 }
				 address[2] = cc[0];
				 nrf24l01_set_tx_addr(address , 5);
				 nrf24l01_write_tx_payload(cc + 1 , 10 , true);

				  if(checkFirstoo<2)
						checkFirstoo++;

}

uint16_t data[1000]={0};
unsigned char str [40];
uint16_t dat = 11;
int Channel = 110;

int main(void)
{
	int i;
	unsigned char add[5]={110,110,8,110,110};

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

    nrf24l01_initialize_debug(false, 10, false);
	nrf24l01_clear_flush();
	add[2]=8;
	nrf24l01_set_tx_addr(add , 5);
	add[2]=30;
	nrf24l01_set_rx_addr(add,5,0);
	nrf24l01_set_rf_ch(Channel);

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

		if(firstPacketRecieved==0)
		{
			demo();
			setNumber(11);
		}

  }
}
