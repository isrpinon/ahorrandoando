

from network import WLAN
from mqtt import MQTTClient
import machine
import time

wlan = WLAN(mode=WLAN.STA)
wlan.connect('mxhacks_esquina3', auth=(WLAN.WPA2, '5805hack'), timeout=5000)

print('Waiting to get connected to Wifi…\n')
while not wlan.isconnected():
     machine.idle()
print('Connected to Wifi!\n')

client = MQTTClient('d:rxe24d:esp32_iot:9C0E1BA4AE30','rxe24d .messaging.internetofthings.ibmcloud.com',user='use-token-auth', password='(+S_C*1j_Gt2R5u?+(', port=1883)

client.connect()
print(“Connected to Watson IoT!\n”)

while True:
     print('Sending ON')
     client.publish(topic='iot-2/evt/status/fmt/json', msg='{\'d\':\'ON\'}')
     time.sleep(10)
     print('Sending OFF')
     client.publish(topic='iot-2/evt/status/fmt/json', msg='{\'d\':\'OFF\'}')
     time.sleep(10) 
