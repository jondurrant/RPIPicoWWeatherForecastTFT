# RPIPicoWWeatherForecastTFT
Display Weather Forecast on a TFT Screen via Raspberry Pi Pico W

This is a demonstration of using a MSP2401  2.4 TFT screen driven by ILI9341 from Raspberry PI Pico

The Repo goes along with a video on my [YouTube channel](https://youtu.be/@drjonea)

## Display

The display I used is MSP2401 or a generic version of. Purchased from [Kunkune](https://kunkune.co.uk/shop/kunkune-accessories/lcd-2-4-inch-240x320-spi-tft-lcd-serial-port-module-5v3-3v-pcb-adapter-micro-sd-card/)

## Connectivity
		-	12,		//SDO(MISO)
		-	9, 		//CS
		-	10,		//SCK
		-	11,		//SDI(MOSI)
		-	15,  		//RESET
		-	8,			//DC/RS
		-	13, 		//LED
		
		
## Web Services Used
Location information and a local map is access through Google Web Services.  A  Google API Key should be obtained as details in the Google Maps Platform: https://developers.google.com/maps/documentation/geolocation/overview

the Google API Key should be defined in the environment variable GOOGLEKEY before running cmake.

Weather forecast data is pulled from OpenWeatherMaps. An OpenWeatherMaps API Key is required. This can be obtained from https://openweathermap.org/.

The Open Weatehr Maps API Key should defined in an environment variable called OPENWEATHERMAPKEY

Wifi credentials should be defined in the environment variables:
+ WIFI_SSID
+ WIFI_PASSWORD
