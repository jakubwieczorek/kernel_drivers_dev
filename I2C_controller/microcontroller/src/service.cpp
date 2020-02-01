/**
  ******************************************************************************
  * @file    service.cpp
  * @author  Jakub Wieczorek
  * @version V1.0
  * @date    22-November-2019
  * @brief   File with services functions.
  ******************************************************************************
*/

#include "stm32f1xx.h"
#include "service.h"

extern UART_HandleTypeDef uart;

void send_message(const char *a_message)
{
	HAL_UART_Transmit_DMA(&uart, (uint8_t*)a_message, strlen(a_message));
}

const char* prepareMsg(const uint16_t &adc_value)
{
	char adc_value_msg[100];

	std::string sMsg;

	sprintf(adc_value_msg, "ADC_CH1 %4d %1.3f Volts \r\n", adc_value, adc_value * 3.3f / 4096.0f);

	sMsg = std::string() + adc_value_msg;

	return sMsg.c_str();
}
