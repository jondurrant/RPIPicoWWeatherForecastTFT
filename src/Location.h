/*
 * Location.h
 *
 * Manage IPGeolocation and MAP of location area
 *
 *  Created on: 21 May 2024
 *      Author: jondurrant
 */

#ifndef SRC_LOCATION_H_
#define SRC_LOCATION_H_

#include "FreeRTOS.h"
#include "Ili9341.h"
#include "pico/stdlib.h"
#include <spng.h>

class Location {
public:
	Location(Ili9341 *tft, char * buf, size_t bufSize);
	virtual ~Location();

	/***
	 * Update location data
	 */
	void update();

	/***
	 * Display a local map of the loxation
	 */
	void display();

	/***
	 * Get returned latitude
	 * @return
	 */
	float getLat();

	/***
	 * Get returned longitude
	 * @return
	 */
	float getLon();

private:
	Ili9341 *pTft = NULL;

	float xLon = 0.0;
	float xLat = 0.0;

	char * pBuffer;
	size_t xBufSize;

	const void *pPng = NULL;
	uint16_t xPngLen = 0;
	uint16_t xPngPos =0;


};

#endif /* SRC_LOCATION_H_ */
