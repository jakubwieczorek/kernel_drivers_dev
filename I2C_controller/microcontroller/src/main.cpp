/**
  ******************************************************************************
  * @file    main.cpp
  * @author  Jakub Wieczorek
  * @version V1.0
  * @date    22-November-2019
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32f1xx.h"
#include <string>
#include <cstring>
#include "DataManager.h"
#include "service.h"
#include "config.h"

UART_HandleTypeDef uart;
ADC_HandleTypeDef adc;
DMA_HandleTypeDef dma;
DMA_HandleTypeDef dma_uart;

/**
 * adc readouts, first one is voltage divider, second built in voltage reference
 * */
uint16_t adc_value[2];

int main(void)
{
	SystemCoreClock = 8000000;
	HAL_Init();

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_ADC1_CLK_ENABLE();
	__HAL_RCC_DMA1_CLK_ENABLE();

	gpio_config();
	uart_config();
	adc_config();
	dma_config();
	nvic_config();

	char msg[300];
	sprintf(msg, "After reset\r\n");
	send_message(msg);

    HAL_ADC_Start_DMA(&adc, (uint32_t*)adc_value, 2);

	while(1)
	{
		send_message(prepareMsg(adc_value[0]));

		HAL_Delay(1000);
	}
}
