/*
 * Weather.cpp
 *
 *  Created on: 7 Apr 2024
 *      Author: jondurrant
 */

#include "Weather.h"
#include "Request.h"
#include <cstdio>
#include "tiny-json.h"
#include <spng.h>

#define MAX_TAGS 80
#define BUF_LEN 1024

Weather::Weather(Ili9341 *tft,  char * buf, size_t bufSize) {
	pTft = tft;
	pBuffer = buf;
	xBufSize = bufSize;

	setPos(0.0, 0.0);
}

Weather::~Weather() {
	// TODO Auto-generated destructor stub
}


void Weather::update(){

	Request req(pBuffer, xBufSize);
	bool res;
	char json[BUF_LEN];
	json_t pool[MAX_TAGS];

	char url[] = "https://api.openweathermap.org/data/2.5/weather";

	std::map<std::string, std::string> query;

	query["appid"]=OPENWEATHERMAPKEY;
	query["lat"] = xLat;
	query["lon"] = xLon;

	req.setForceTLS12(true);
	res = req.get(url, &query);
	if ( res ){
		res = (req.getStatusCode() == 200);
	}
	if (res){
		printf("WS: %.*s\n", req.getPayloadLen(), req.getPayload());

		memcpy(json, req.getPayload(), req.getPayloadLen());
		json[req.getPayloadLen()] = 0;
		json_t const* parent = json_create(
				json,
				pool,
				MAX_TAGS);
		if ( parent != NULL ){
			//Location
			json_t const* sys = json_getProperty( parent, "sys" );
			if (sys != NULL){
				json_t const* loc = json_getProperty( parent, "name" );
				if (loc != NULL){
					strcpy(xLoc, json_getValue(loc));
				}
			}

			//Temperatures
			json_t const* main = json_getProperty( parent, "main" );
			if (main != NULL){
				json_t const* temp = json_getProperty( main, "temp" );
				if (temp != NULL){
					if ( json_getType( temp ) == JSON_REAL ){
						xTemp = json_getReal(temp) -273.15;
					}
				}
				json_t const* tempMin = json_getProperty( main, "temp_min" );
				if (tempMin != NULL){
					if ( json_getType( tempMin ) == JSON_REAL ){
						xTempMin = json_getReal(tempMin) -273.15;
					}
				}
				json_t const* tempMax = json_getProperty( main, "temp_max" );
				if (tempMax != NULL){
					if ( json_getType( tempMax ) == JSON_REAL ){
						xTempMax = json_getReal(tempMax) -273.15;
					}
				}
			}

			//Icon
			json_t const* weather = json_getProperty( parent, "weather" );
			if (weather != NULL){
				json_t const* item = json_getChild( weather);
				if (item != NULL){
					json_t const* desc = json_getProperty( item, "description" );
					if (desc != NULL){
						strcpy(xDesc, json_getValue(desc));
					}
					json_t const* icon = json_getProperty( item, "icon" );
					if (icon != NULL){
						strcpy(xIcon, json_getValue(icon));
					}
				}
			}

		}
	} else {
		printf("WS failed %d\n", req.getStatusCode());
	}
}

void Weather::display(){
	pTft->clear();

	sprintf(pBuffer, "%0.1fc", xTemp);
	pTft->putStr(pBuffer, 55, 5);


	int y = 50;
	pTft->putStr("Weather:", 0, y);
	y+= 34;
	pTft->putStr(xLoc, 0, y);
	y+= 34;
	//sprintf(pBuffer, "Current: %0.1fc", xTemp);
	//pTft->putStr(pBuffer, 0, y);
	//y+= 34;
	pTft->putStr(xDesc, 0, y);
	y+= 34;
	sprintf(pBuffer, "Min: %0.1fc Max: %0.1fc", xTempMin, xTempMax);
	pTft->putStr(pBuffer, 0, y);

	/*
	y+= 34;
	sprintf(pBuffer, "Max: %0.1fc", xTempMax);
	pTft->putStr(pBuffer, 0, y);
	y+= 34;
	*/
}


void Weather::displayIcon(){
	char iconURL[120];

	sprintf(iconURL,"https://openweathermap.org/img/w/%s.png", xIcon);
	Request req(pBuffer, xBufSize);
	bool res;

	res = req.get(iconURL);
	if ( res ){
		res = (req.getStatusCode() == 200);
	}
	if (res){
		printf("Icon Len: %d\n", req.getPayloadLen());

		size_t size;
		int length =  req.getPayloadLen();
		uint8_t * pPng = (uint8_t *) req.getPayload();
		struct spng_ihdr ihdr;
		struct spng_alloc spngAlloc;
		spngAlloc.malloc_fn = pvPortMalloc;
		spngAlloc.realloc_fn = pvPortRealloc;
		spngAlloc.calloc_fn = pvPortCalloc;
		 spngAlloc.free_fn = vPortFree;

		spng_ctx *ctx = spng_ctx_new2(&spngAlloc, 0);
		spng_set_png_buffer(ctx, pPng, length);

		spng_decoded_image_size(ctx, SPNG_FMT_RGB8, &size);
		printf("PNG Out Buf size is %d\n", size);
		//out = (uint32_t *)malloc(size);
		uint8_t * out = (uint8_t *) pvPortMalloc(size);

		spng_get_ihdr(ctx, &ihdr);
		uint width = ihdr.width;
		uint height=ihdr.height;
		printf("PNG (%u, %u)\n", width, height);

		int res = spng_decode_image(ctx, out, size, SPNG_FMT_RGB8, 0);

		printf("Decod complete = %d  %s\n", res, spng_strerror(res));

		pTft->drawBitmapRGB8(0,0, width, height, out, true);

		vPortFree(out);
		spng_ctx_free(ctx);

	} else {
		printf("WS failed %d\n", req.getStatusCode());
	}
}


void Weather::setPos(float lat, float lon){
	sprintf(xLat, "%f", lat);
	sprintf(xLon, "%f", lon);
}
