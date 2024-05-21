import requests
import os

key = os.environ["GOOGLEKEY"]
lat = 50.822529
lon = -0.137163

url = ('https://www.googleapis.com/geolocation/v1/geolocate?key=%s'%(
    key
    ))

x = requests.post(url)

print(x.text)

mapUrl = ('https://maps.googleapis.com/maps/api/staticmap?center=%f,%f&size=320x240&zoom=10&key=%s'%(
    lat,
    lon,
    key))

map = requests.get(url)

