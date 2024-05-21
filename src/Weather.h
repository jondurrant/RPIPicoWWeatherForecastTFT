/*
 * Weather.h
 *
 *  Created on: 7 Apr 2024
 *      Author: jondurrant
 */

#ifndef SEC_10_EXAMPLES_WEATHER_SRC_WEATHER_H_
#define SEC_10_EXAMPLES_WEATHER_SRC_WEATHER_H_

#include "FreeRTOS.h"
#include "Ili9341.h"
#include "pico/stdlib.h"


class Weather {
public:
	Weather(Ili9341 *tft, char * buf, size_t bufSize);
	virtual ~Weather();


	void update();

	void display();

	void displayIcon();

private:
	Ili9341 *pTft = NULL;

	char xDesc[20];
	char xLoc[20];
	char xIcon[5];
	float xTemp;
	float xTempMax;
	float xTempMin;

	char * pBuffer;
	size_t xBufSize;


};

#endif /* SEC_10_EXAMPLES_WEATHER_SRC_WEATHER_H_ */
