/**
  ******************************************************************************
  * @file    DataManager.cpp
  * @author  Jakub Wieczorek
  * @version V1.0
  * @date    22-November-2019
  * @brief   Source file for DataManager class.
  ******************************************************************************
*/

#include"DataManager.h"

void DataManager::add(uint16_t value)
{
	this->data[this->index++] = value;

	if(this->index == this->size)
	{
		this->index = 0;
	}
}

uint16_t DataManager::calculateAverage()
{
	uint16_t average = 0;

	for(int i = 0; i < this->size; i++)
	{
		average += (this->data[i] / this->size);
	}

	return average;
}
