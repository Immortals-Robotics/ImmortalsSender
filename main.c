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


///////////////////// Ethernet /////////////////////////
#include "user_spi.h"
#include "user_ethernet.h"
#include "socket.h"

#define SOCK_TCPC 0
uint8_t UDP_packet[BUFFER_SIZE];
uint8_t targetIP[4] = {192, 168, 1, 255};   // Test !!!  // Forth byte must be compliment of Mask addr (Mask -> 255.255.255.0  Ip -> (192.168.1)(not important).255(0(mask)+255(ip) ))
uint8_t sourceIP[4] = {0, 0, 0, 0};  
uint16_t senderPort = 30008;
uint16_t targetPort = 30010;
uint16_t *curPort;
////////////////////////////////////////////////////////

int c = 0;
void transmit(char* msg){
	for(int i = 0; i < size(msg); i++){
		SEGGER_RTT_printf(0, "Bytes: %x\r\n", msg[i]);
	}
	nrf_esb_stop_rx();
	nrf_delay_us(2000);
	esb_init(TX_MODE);
	tx_payload.length = 3;
	tx_payload.noack = true;
	nrf_delay_us(2000);
	
	while(1){
		tx_payload.data[0] = (char)msg[0];
		tx_payload.data[1] = (char)msg[1];
		tx_payload.data[2] = c;
		if(nrf_esb_write_payload(&tx_payload) == NRF_SUCCESS){
				c ++;
				break;
		}
	}
	nrf_delay_us(2000);
	esb_init(RX_MODE);
	nrf_delay_us(2000);	
  nrf_esb_start_rx();
}


int main(void){
		// Init packet counter
		counter = 0;
		SEGGER_RTT_WriteString(0, "Immortals\r\n");
		
		gpio_init();
		welcome();
		
		SEGGER_RTT_WriteString(0, "\r\n***Immortals Robotic team***\r\n");
		
		SEGGER_RTT_WriteString(0, "\r\nInitialize SPI\r\n");



	spi0_master_init();
		//nrf_delay_ms(10);  // Don't use delay here :) 
		SEGGER_RTT_WriteString(0, "\r\nInitialize Ethernet\r\n");
		user_ethernet_init();
	
		int32_t ret; // return value for SOCK_ERRORs
		ret = socket(SOCK_TCPC, Sn_MR_UDP, senderPort, 0x00);
		if(ret != SOCK_TCPC){ 
				SEGGER_RTT_WriteString(0, "%d:Socket Open Fail\r\n"); 
		}
		
		for(int k = 0 ; k < 2 ; k ++){
			ret = sendto(SOCK_TCPC, (void *)"Connected\r\n", 11, targetIP, targetPort);
		}
		
    uint32_t err_code;
    err_code = logging_init();
    APP_ERROR_CHECK(err_code);
    
    clocks_start();
		esb_init(RX_MODE);
		//////////////////////////////////////////

		//////////////////////////////////////////
		
		int new_packet = 0;
    while (true){
			new_packet = getSn_RX_RSR(0);  // Ethernet Listener
			if(new_packet != 0){
				int ret = recvfrom(SOCK_TCPC, UDP_packet, BUFFER_SIZE, sourceIP, curPort);
				SEGGER_RTT_printf(0, "Packet: %d %x\r\n", size(UDP_packet), UDP_packet);
				SEGGER_RTT_printf(0, "IP: %d %hu\r\n", size(sourceIP), (uint16_t)*curPort);
				transmit(UDP_packet);
			}
			if(received){
				received = false;
				sprintf(tempBuffer, "%s\r\n", rx_payload.data);
				sendto(SOCK_TCPC, (tempBuffer), size(tempBuffer), targetIP, targetPort);
			}
    }
}