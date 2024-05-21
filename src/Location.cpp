/*
 * Location.cpp
 *
 *  Created on: 21 May 2024
 *      Author: jondurrant
 */

#include "Location.h"
#include "Request.h"
#include <cstdio>
#include "tiny-json.h"


#define MAX_TAGS 10
#define BUF_LEN 256

Location::Location(Ili9341 *tft,  char * buf, size_t bufSize) {
	pTft = tft;
	pBuffer = buf;
	xBufSize = bufSize;
}

Location::~Location() {
	// TODO Auto-generated destructor stub
}


void Location::update(){

	Request req(pBuffer, xBufSize);
	bool res;
	char json[BUF_LEN];
	json_t pool[MAX_TAGS];

	char url[] = "https://www.googleapis.com/geolocation/v1/geolocate?key="
			GOOGLEKEY;

	std::map<std::string, std::string> query;

	query["key"]=GOOGLEKEY;

	res = req.post(url, &query);
	if ( res ){
		res = (req.getStatusCode() == 200);
	}
	if (res){
		printf("Got Location\n");
		printf("WS: %.*s\n", req.getPayloadLen(), req.getPayload());

		memcpy(json, req.getPayload(), req.getPayloadLen());
		json[req.getPayloadLen()] = 0;
		json_t const* parent = json_create(
				json,
				pool,
				MAX_TAGS);
		if ( parent != NULL ){
			//Location
			json_t const* loc = json_getProperty( parent, "location" );
			if (loc != NULL){
				json_t const* lat = json_getProperty( loc, "lat" );
				if (lat != NULL){
					if ( json_getType( lat ) == JSON_REAL ){
						xLat = json_getReal(lat);
					}
				}

				json_t const* lon = json_getProperty( loc, "lng" );
				if (lon){
					if ( json_getType( lon ) == JSON_REAL ){
						xLon = json_getReal(lon);
					}
				}
			}
		}

	} else {
		printf("Location Failed %d %s\n",
				req.getStatusCode(),
				req.getUriChar());
	}

}

void Location::display(){

	Request req(pBuffer, xBufSize);
	bool res;
	bool result = true;
	struct spng_ihdr ihdr;
	size_t size;
	char pos[80];
	uint32_t out[320];
	struct spng_row_info row_info;

	char url[] = "https://maps.googleapis.com/maps/api/staticmap";

	std::map<std::string, std::string> query;

	query["key"]=GOOGLEKEY;
	query["size"] = "320x240";
	query["zoom"] = "10";
	sprintf(pos,"%f,%f", xLat, xLon);
	query["center"]= pos;

	res = req.get(url, &query);
	if ( res ){
		res = (req.getStatusCode() == 200);
	}
	if (res){
		printf("Got Map\n");
		printf("WS: len %d\n", req.getPayloadLen());

		pPng = req.getPayload();
		xPngLen = req.getPayloadLen();

		struct spng_alloc spngAlloc;
		spngAlloc.malloc_fn = pvPortMalloc;
		spngAlloc.realloc_fn = pvPortRealloc;
		spngAlloc.calloc_fn = pvPortCalloc;
		 spngAlloc.free_fn = vPortFree;

		spng_ctx *ctx = spng_ctx_new2(&spngAlloc, 0);
		spng_set_png_buffer(ctx, pPng, xPngLen);

		spng_decoded_image_size(ctx, SPNG_FMT_RGB8, &size);
		printf("PNG Out Buf size is %d\n", size);

		res = spng_get_ihdr(ctx, &ihdr);
		if (ihdr.width > pTft->getWidth()){
			printf("Image is too wide %u > 240\n", ihdr.width);
			result = false;
		}
		if (ihdr.height > pTft->getHeight()){
			printf("Image is too high %u > 320\n", ihdr.height);
					result = false;
		}
		printf("(%d)PNG (%u, %u)\n", res, ihdr.width, ihdr.height);

		if (result){
			spng_decode_image(ctx, out, size, SPNG_FMT_RGB8, SPNG_DECODE_PROGRESSIVE);
			//spng_decode_image(ctx, out, size, SPNG_FMT_RGBA8, SPNG_DECODE_PROGRESSIVE);

			uint line = 0;
			for(;;){
				res = spng_decode_row( ctx,  out,  ihdr.width*3);

				if (res == SPNG_EOI){
					printf("EOF Reached\n");
					break;
				}
				if (res != 0){
					printf("Read decode failed\n");
					result = false;
					break;
				}

				res = spng_get_row_info( ctx,  &row_info);
				//printf("(%d)Row Info %d \n", res, row_info.row_num);
				if (res == 0){
					line = (row_info.row_num  -1)% ihdr.width;

					pTft->drawBitmapRGB8(
							0,
							line,
							 ihdr.width,
							 1,
							(uint8_t *)out,
							false
							);

				} else {
					result = false;
					break;
				}
			}
		}

		spng_ctx_free(ctx);


	} else {
		printf("Map Failed %d %s\n",
						req.getStatusCode(),
						req.getUriChar());
	}

}

float Location::getLat(){
	return xLat;
}
float Location::getLon(){
	return xLon;
}

