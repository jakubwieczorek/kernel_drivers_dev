/**
  ******************************************************************************
  * @file    DataManager.h
  * @author  Jakub Wieczorek
  * @version V1.0
  * @date    22-November-2019
  * @brief   Header file for DataManager class.
  ******************************************************************************
*/


#ifndef DATAMANAGER_H_
#define DATAMANAGER_H_

#include"stm32f1xx.h"

/**
 *  DataManager class, which calculates an average of queue implemented within a class.
 */
class DataManager
{
private:
		uint16_t data[20] {};
		uint8_t index {};
		uint8_t size = 20;
public:
		/**
		 * Add element to data queue
		 *
		 * @param value to be inserted into data queue
		 * */
		void add(uint16_t value);

		/**
		 * calculate an average of data
		 *
		 * @ return average
		 * */
		uint16_t calculateAverage();
};

#endif /* DATAMANAGER_H_ */
