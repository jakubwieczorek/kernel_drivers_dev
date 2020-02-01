/**
  ******************************************************************************
  * @file    service.h
  * @author  Jakub Wieczorek
  * @version V1.0
  * @date    22-November-2019
  * @brief   File with services functions.
  ******************************************************************************
*/

#ifndef SERVICE_H_
#define SERVICE_H_

#include "stm32f1xx.h"
#include <string>
#include <cstring>

/**
 * Sends a_message through UART
 *
 * @param a_message const char* message to be sent
 * */

void send_message(const char *a_message);

/**
 * Function prepares message, by wrapping adc_value and average according to a protocol,
 * which is
 * NAME VALUE CONVERT UNIT with carriage return and new line, for example
 * ADC_CH1 1906 1.536 Volts
 * ADC_CH1_MEAN 2220 1.789 Volts
 *
 * @param adc_value value from ADC, to which voltage divider is connected with needles and a set of resistors
 * @param average avarage of 20 ADC readouts
 *
 * @return prepared message in const char*
 * */
const char* prepareMsg(const uint16_t &adc_value);

#endif
