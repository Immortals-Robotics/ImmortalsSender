#include "mylog.h"
#include <stdio.h>
#include <string.h>
#define LOG_NRF_TX "TX Payload"
#define LOG_NRF_RX "RX Payload"
#define LOG_ETHERNET "Ethernet"
#define LOG_NETWORK "Network"
#define LOG_ERROR "ERROR"
#define LOG_BUFFER_SIZE 256
char logBuffer[LOG_BUFFER_SIZE];


char* MYLOG(char* key, char* msg){
	sprintf(logBuffer, "[%s] %s\r\n", key, msg);
	return logBuffer;
}