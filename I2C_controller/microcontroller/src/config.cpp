/**
  ******************************************************************************
  * @file    config.cpp
  * @author  Jakub Wieczorek
  * @version V1.0
  * @date    22-November-2019
  * @brief   Source file with config functions.
  ******************************************************************************
*/

#include "config.h"

extern UART_HandleTypeDef uart;
extern ADC_HandleTypeDef adc;
extern DMA_HandleTypeDef dma;
extern DMA_HandleTypeDef dma_uart;

void gpio_config()
{
	GPIO_InitTypeDef gpio;

	// button
	gpio.Pin = GPIO_PIN_13;
	gpio.Mode = GPIO_MODE_IT_RISING_FALLING;
	gpio.Pull = GPIO_PULLUP;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &gpio);

	//led
	gpio.Pin = GPIO_PIN_5;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &gpio);

	// UART output
	gpio.Pin = GPIO_PIN_2;
	gpio.Mode = GPIO_MODE_AF_PP;
	HAL_GPIO_Init(GPIOA, &gpio);

	// UART input
	gpio.Pin = GPIO_PIN_3;
	gpio.Mode = GPIO_MODE_AF_INPUT;
	HAL_GPIO_Init(GPIOA, &gpio);

	// ADC
	gpio.Pin = GPIO_PIN_0;
	gpio.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init(GPIOA, &gpio);
}

void uart_config()
{
	uart.Instance = USART2;
	uart.Init.BaudRate = 115200;
	uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart.Init.Mode = UART_MODE_TX_RX;
	uart.Init.OverSampling = UART_OVERSAMPLING_16;
	uart.Init.Parity = UART_PARITY_NONE;
	uart.Init.StopBits = UART_STOPBITS_1;
	uart.Init.WordLength = UART_WORDLENGTH_8B;
	HAL_UART_Init(&uart);
}

void adc_config()
{
    RCC_PeriphCLKInitTypeDef adc_clk;
    adc_clk.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    adc_clk.AdcClockSelection = RCC_ADCPCLK2_DIV2;
    HAL_RCCEx_PeriphCLKConfig(&adc_clk);

    adc.Instance = ADC1;
	adc.Init.ContinuousConvMode = ENABLE;
	adc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	adc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	adc.Init.ScanConvMode = ADC_SCAN_ENABLE;
	adc.Init.NbrOfConversion = 2;
	adc.Init.DiscontinuousConvMode = DISABLE;
	adc.Init.NbrOfDiscConversion = 1;
	HAL_ADC_Init(&adc);

	ADC_ChannelConfTypeDef adc_ch;
	adc_ch.Channel = ADC_CHANNEL_0;
	adc_ch.Rank = ADC_REGULAR_RANK_1;
	adc_ch.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
	HAL_ADC_ConfigChannel(&adc, &adc_ch);

	adc_ch.Channel = ADC_CHANNEL_VREFINT;
	adc_ch.Rank = ADC_REGULAR_RANK_2;
	HAL_ADC_ConfigChannel(&adc, &adc_ch);
	HAL_ADCEx_Calibration_Start(&adc);
}

void dma_config()
{
	dma.Instance = DMA1_Channel1;
	dma.Init.Direction = DMA_PERIPH_TO_MEMORY;
	dma.Init.PeriphInc = DMA_PINC_DISABLE;
	dma.Init.MemInc = DMA_MINC_ENABLE;
	dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	dma.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	dma.Init.Mode = DMA_CIRCULAR;
	dma.Init.Priority = DMA_PRIORITY_HIGH;
	HAL_DMA_Init(&dma);
	__HAL_LINKDMA(&adc, DMA_Handle, dma);

	dma_uart.Instance = DMA1_Channel7;
	dma_uart.Init.Direction = DMA_MEMORY_TO_PERIPH;
	dma_uart.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	dma_uart.Init.MemInc = DMA_MINC_ENABLE;
	dma_uart.Init.Mode = DMA_NORMAL;
	dma_uart.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	dma_uart.Init.PeriphInc = DMA_PINC_DISABLE;
	dma_uart.Init.Priority = DMA_PRIORITY_HIGH;
	HAL_DMA_Init(&dma_uart);
	__HAL_LINKDMA(&uart, hdmatx, dma_uart);
}

void nvic_config()
{
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);

	HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
}
