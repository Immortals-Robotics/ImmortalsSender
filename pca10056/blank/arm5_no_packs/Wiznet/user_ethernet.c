#include "user_ethernet.h"
#include "user_spi.h"
#include "socket.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>


///////////////////////////////////
// Default Network Configuration //
///////////////////////////////////
wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc,0x00, 0xab, 0xcd},
                            .ip = {192, 168, 1, 126}, 
                            .sn = {255,255,255,0},  // Mask 
                            .gw = {192, 168, 1, 1}, 
                            .dns = {8,8,8,8},
                            .dhcp = NETINFO_DHCP };

void wizchip_select(){
	nrf_gpio_pin_clear(SPI_SS_PIN);
}

void wizchip_deselect(){
	nrf_gpio_pin_set(SPI_SS_PIN);
}


uint8_t wizchip_read(){
	uint8_t recv_data;

	spi_master_rx(SPI0,1,&recv_data);

	return recv_data;
}

void wizchip_write(uint8_t wb){
	spi_master_tx(SPI0, 1, &wb);
}


void user_ethernet_init(){
	  uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};
		wiz_NetTimeout timeout_info;

    reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
    reg_wizchip_spi_cbfunc(wizchip_read, wizchip_write);

    /* WIZCHIP SOCKET Buffer initialize */
	
    SEGGER_RTT_WriteString(0, "W5100 memory init\r\n");

    if(ctlwizchip(CW_INIT_WIZCHIP,(void*)memsize) == -1){
			SEGGER_RTT_WriteString(0, "WIZCHIP Initialized fail.\r\n");
      while(1);
    }

    timeout_info.retry_cnt = 1;
    timeout_info.time_100us = 0x3E8;	// timeout value = 10ms

    wizchip_settimeout(&timeout_info);
		
    /* Network initialization */
    network_init();
}

void network_init(){
  uint8_t tmpstr[6];
	ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);
	ctlnetwork(CN_GET_NETINFO, (void*)&gWIZNETINFO);

	// Display Network Information
	ctlwizchip(CW_GET_ID,(void*)tmpstr);
}
