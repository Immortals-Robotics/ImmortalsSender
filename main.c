//  NRF 52840
//  http://gitlab.com/Phoenix_flame
#include "nrf_esb.h"
#include "SEGGER_RTT.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "sdk_common.h"
#include "nrf.h"
#include "nrf_error.h"
#include "nrf_esb_error_codes.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "app_util.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "mylog.h"
#include "myNRF.h"
#include <time.h>

//#define DEBUG

///////////////////// Ethernet /////////////////////////
#include "user_spi.h"
#include "user_ethernet.h"
#include "socket.h"

#define SOCK_TCPC 0





time_t raw_time;
uint8_t UDP_packet[BUFFER_SIZE];
uint8_t targetIP[4] = {192, 168, 1, 255};   // It's OK !!!  // Forth byte must be compliment of Mask addr (Mask -> 255.255.255.0  Ip -> (192.168.1)(not important).255(0(mask)+255(ip) ))
uint8_t sourceIP[4] = {0, 0, 0, 0};  
uint16_t senderPort = 30008;
uint16_t targetPort = 30010;
uint16_t *curPort;


int c = 0;
void transmit(char* msg, bool demo){
	#ifdef DEBUG
		for(int i = 0; i < size(msg); i++){
			SEGGER_RTT_printf(0, "Bytes: %x\r\n", msg[i]);
		}
	}
	#endif

	// Preproccessing
	int len = (int)msg[1]; // Packet Length
	int id = (int)msg[0]; // RobotID
	
	
	if (demo){
		nrf_esb_stop_rx();
		nrf_delay_us(2000);
		esb_init(TX_MODE, -1);
		tx_payload.length = 1;
		tx_payload.noack = true;
		nrf_delay_us(2000);
		while(1){
			tx_payload.data[0] = (char)msg[0];
			tx_payload.data[8] = c;
			if(nrf_esb_write_payload(&tx_payload) == NRF_SUCCESS){
				c ++;
				break;
			}
		}
		nrf_delay_us(2000);
		esb_init(RX_MODE, 30);
		nrf_delay_us(2000);	
		nrf_esb_start_rx();	
	}
	// Normal packet transmission
	else{
		nrf_esb_stop_rx();
		nrf_delay_us(2000);
		esb_init(TX_MODE, id);
		tx_payload.length = 32;
		tx_payload.noack = true;
		nrf_delay_us(2000);
		
		#ifdef DEBUG
		for(int i = 0 ; i < len; i++){
			tx_payload.data[i] = (char)msg[i];
			SEGGER_RTT_printf(0, "Bytes: %x\r\n", msg[i]);
		}
		SEGGER_RTT_printf(0, "Length: %d\r\n", (int)msg[0]);
		#endif
		
		while(1){
			for(int i = 0 ; i < len; i++){
				tx_payload.data[i] = (char)msg[i];
			}
			tx_payload.data[20] = c;
			if(nrf_esb_write_payload(&tx_payload) == NRF_SUCCESS){
				c ++;
				break;
			}
		}
		nrf_delay_us(2000);
		esb_init(RX_MODE, 30);   // RX address for all robots
		nrf_delay_us(2000);	
		nrf_esb_start_rx();	
	}
	
}


int main(void){
		// Init packet counter
		counter = 0;
		nrf_delay_ms(250);
		gpio_init();
		spi0_master_init();
		nrf_delay_ms(250);
		wizchip_select();
		nrf_delay_ms(250);
		wizchip_deselect();
		nrf_delay_ms(250);	
		user_ethernet_init();
		welcome();
		SEGGER_RTT_WriteString(0, "Immortals\r\n");
		SEGGER_RTT_WriteString(0, "\r\n***Immortals Robotic team***\r\n");
		
		SEGGER_RTT_WriteString(0, "\r\nInitialize SPI\r\n");



		
		//nrf_delay_ms(10);  // Don't use delay here :) 
		SEGGER_RTT_WriteString(0, "\r\nInitialize Ethernet\r\n");
		int32_t ret; // return value for SOCK_ERRORs
		ret = socket(SOCK_TCPC, Sn_MR_UDP, senderPort, 0x00);
		if(ret != SOCK_TCPC){ 
				SEGGER_RTT_WriteString(0, "%d:Socket Open Fail\r\n"); 
		}
		
		ret = sendto(SOCK_TCPC, (void *)"Connected\r\n", 11, targetIP, targetPort);
		
    uint32_t err_code;
    err_code = logging_init();
    
    clocks_start();
		esb_init(RX_MODE, 30);  // Mode, Address
		
		// Notify (Demo mode)
		//////////////////////////////////////////
		sprintf(demoBuffer, NOTIFY);
		//////////////////////////////////////////
		int timer = 0;
		int new_packet = 0;
		
		
    while (true){
			new_packet = getSn_RX_RSR(0);  // Ethernet Listener

			if(new_packet == 0 && timer == 0){
					for(int i = 0 ; i < 100; i++){
						transmit(demoBuffer, true);
					}
			}
			if (timer != 0 && new_packet == 0){
					timer --;
			}
			if(new_packet != 0){
				#ifdef DEBUG
					SEGGER_RTT_printf(0, "********AI*********\r\n");
				#endif
				new_packet = 0;
				timer = 100000;
				int ret = recvfrom(SOCK_TCPC, UDP_packet, BUFFER_SIZE, sourceIP, curPort);
				transmit(UDP_packet, false);
				#ifdef DEBUG
					SEGGER_RTT_printf(0, "********AI*********\r\n");
				#endif
			}
			if(received){
				received = false;
				sprintf(tempBuffer, "%s\r\n", rx_payload.data);
				#ifdef DEBUG
					SEGGER_RTT_printf(0, "%s\r\n", tempBuffer);
				#endif
				sendto(SOCK_TCPC, (tempBuffer), size(tempBuffer), targetIP, targetPort);
			}
    }
}