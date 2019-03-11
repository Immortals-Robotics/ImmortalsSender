/////////////////////////NRF_RADIO//////////////////////////////


#define TX_MODE  1
#define RX_MODE  2
#define LED_ON          0
#define LED_OFF         1
#define LED_YELLOW      LED_3
#define LED_GREEN       LED_4
#define BUFFER_SIZE	 256

uint8_t tempBuffer[BUFFER_SIZE];


uint32_t esb_init(int state);
static nrf_esb_payload_t tx_payload;
static nrf_esb_payload_t rx_payload;
int counter = 0;
int report = 0;
bool received = false;
char buf[10];



void welcome(){
	for(int i = 0 ; i < 3; i ++){
		nrf_gpio_pin_write(LED_YELLOW, LED_ON);
		nrf_delay_ms(500);
		nrf_gpio_pin_write(LED_YELLOW, LED_OFF);
		nrf_delay_ms(500);
	}
}



void compile_udp_packet(char* msg){
	sprintf(tempBuffer, "%d\r\n", sizeof(msg));
	SEGGER_RTT_WriteString(0, tempBuffer);
	sprintf(tempBuffer, "%s\r\n", msg);
	SEGGER_RTT_WriteString(0, msg);
	for(int i = 0 ; i < sizeof(msg); i++){
		sprintf(tempBuffer, "%s\r\n", msg[i]);
		SEGGER_RTT_WriteString(0, tempBuffer);
	}
}


int size(char *ptr){
    
    int offset = 0;
    int count = 0;
    while (*(ptr + offset) != '\0'){
        ++count;
        ++offset;
    }
    return count;
}



void nrf_esb_event_handler(nrf_esb_evt_t const * p_event){
    switch (p_event->evt_id){
        case NRF_ESB_EVENT_TX_SUCCESS:
						SEGGER_RTT_printf(0, (const char*)MYLOG("TX Payload", "Success."));
            break;
        case NRF_ESB_EVENT_TX_FAILED:
						SEGGER_RTT_WriteString(0, (const char*)MYLOG("TX Payload", "Failed."));
            (void) nrf_esb_flush_tx();
            break;
        case NRF_ESB_EVENT_RX_RECEIVED:
            while (nrf_esb_read_rx_payload(&rx_payload) == NRF_SUCCESS) ;
						sprintf(buf, "[# of packets] %d\r\n", rx_payload.data[0]);
						SEGGER_RTT_WriteString(0, buf);
						sprintf(buf, "[Report] %d\r\n", rx_payload.data[3]);
						SEGGER_RTT_WriteString(0, buf);
						nrf_gpio_pin_toggle(LED_YELLOW);
						received = true;
            break;
    }
}


uint32_t esb_init(int state){
    uint32_t err_code;
    uint8_t base_addr_0[4] = {0xE7, 0xE7, 0xE7, 0xE7};
    uint8_t base_addr_1[4] = {0xC2, 0xC2, 0xC2, 0xC2};
    uint8_t addr_prefix[8] = {0xE7, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8 };
		nrf_esb_config_t nrf_esb_config         = NRF_ESB_DEFAULT_CONFIG;
    nrf_esb_config.payload_length           = 30;
    nrf_esb_config.bitrate                  = NRF_ESB_BITRATE_2MBPS;
		nrf_esb_config.protocol                 = NRF_ESB_PROTOCOL_ESB_DPL;
		switch(state){
			case TX_MODE:
				nrf_esb_config.mode                 = NRF_ESB_MODE_PTX;
				nrf_esb_config.retransmit_delay     = 600;
				nrf_esb_config.tx_mode              = NRF_ESB_TXMODE_AUTO;
				break;
			case RX_MODE:
				nrf_esb_config.mode                 = NRF_ESB_MODE_PRX;
				break;
		}
		nrf_esb_config.selective_auto_ack       = false;
    nrf_esb_config.event_handler            = nrf_esb_event_handler;

    err_code = nrf_esb_init(&nrf_esb_config);
    VERIFY_SUCCESS(err_code);

    err_code = nrf_esb_set_base_address_0(base_addr_0);
    VERIFY_SUCCESS(err_code);

    err_code = nrf_esb_set_base_address_1(base_addr_1);
    VERIFY_SUCCESS(err_code);

    err_code = nrf_esb_set_prefixes(addr_prefix, 8);
    VERIFY_SUCCESS(err_code);

		nrf_esb_set_rf_channel(1);

    return NRF_SUCCESS;
}



uint32_t logging_init( void ){
    uint32_t err_code;
    err_code = NRF_LOG_INIT(NULL);

    NRF_LOG_DEFAULT_BACKENDS_INIT();

    return err_code;
}

void clocks_start( void ){
    // Start HFCLK and wait for it to start.
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART = 1;
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);
}


void gpio_init( void ){
    nrf_gpio_cfg_input(BUTTON_1, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(BUTTON_2, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(BUTTON_3, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(BUTTON_4, NRF_GPIO_PIN_PULLUP);
    nrf_delay_ms(1);
    bsp_board_init(BSP_INIT_LEDS);
}