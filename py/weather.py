import requests
import os

key = os.environ["OPENWEATHERMAPKEY"]
lat = 50.822529
lon = -0.137163

url = ('https://api.openweathermap.org/data/2.5/weather?lat=%f&lon=%f&appid=%s'%(
    lat,
    lon,
    key
    ))

x = requests.get(url)

print(x.text)