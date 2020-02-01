/**
  ******************************************************************************
  * @file    config.h
  * @author  Jakub Wieczorek
  * @version V1.0
  * @date    22-November-2019
  * @brief   Header with config functions.
  ******************************************************************************
*/

#ifndef CONFIG_H_
#define CONFIG_H_

#include "stm32f1xx.h"

/**
 * GPIO configuration, that is USER button, LED, UART, ADC
 * */
void gpio_config();

/**
 * UART configuration. Baud rate 115200, no parity, one stop bit, 8 bits data
 * */
void uart_config();

/**
 * ADC configuration 2 channels. One for test purposes it is built in voltage reference and second for voltage divider
 * */
void adc_config();

/**
 * DMA configuration for UART and ADC, so two channels. Interrupt for Channel7 (UART) and USART2 global Interrupt is necessary for DMA to work.
 * */
void dma_config();

/**
 * NVIC configuration: EXTI, so button, DMA for UART and UART itself
 * */
void nvic_config();

#endif
