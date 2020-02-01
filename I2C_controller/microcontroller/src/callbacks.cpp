/**
  ******************************************************************************
  * @file    callbacks.cpp
  * @author  Jakub Wieczorek
  * @version V1.0
  * @date    22-November-2019
  * @brief   Callbacks implementation.
  ******************************************************************************
*/

#include "stm32f1xx.h"

/**
 * Callback for user button on Nucleo board for test purposes.
 * Just toggle built in LED.
 * Function is invoked by HAL_GPIO_EXTI_IRQHandler,
 * from low level EXTI15_10_IRQHandler.
 *
 * @param GPIO_Pin GPIO pin, for user button - is shorted with GPIO_PIN_13
 * */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_13)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}
}
