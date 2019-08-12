## How Is working

On motion detection the AM312 pir will send a short negative pulse on ESP-M2 reset pin in order to wake him up from deep sleep and send a http request to hue emulator bridge with mac address and light level measured by the photoresistor. The logic state of this sensor will remain triggered until the sensor will not send anymore other request in 30 seconds interval.

## Components

 - ESP-M2 module
 - MCP1700T 3.3V regulator
 - 30mm x 40mm battery (803040 in my case)
 - AM312 PIR module with lense
 - 1 x NPN transitor (MMBT4401 in my case)
 - 3 x 100k 0805 resistor
 - 1 x 10k 0805 resistor
 - 1 x 470ohm 0805 resistor
 - 1 x 100nF 0603 capacitor
 - 1 x 4,7uF 0603 capacitor
 - 1 x 10uF 0805 capacitor
 - 1 x SMD led

## Circuit diagram

![Circuit Diagram](https://github.com/diyhue/Devices/raw/master/HueMotionSensor/images/Schematic.png)

## Upgrade

Arduino OTA is integrated but it need to be activated by connecting GPIO4 to ground for short period during the boot process. The led will blink 4 times to confirm Arduino OTA is enabled. You will need to isolate the PIR as any motion detection will reset the device during firmware upload. Ensure the device will not remain in OTA mode enabled as this will consume the battery very fast (manual reset or motion detection reset are enough).

## Led codes:

 - only short reset flash - the request was successfully sent to bridge emulator
 - 4 flashes - Arduino OTA is enabled
 - 6 flashes - http error
 - 8 flashes - bridge emulator unreachable
 - 10 flashes - unable to connect to wifi network in 10 seconds.

## Images

![Case](https://github.com/diyhue/Devices/raw/master/HueMotionSensor/images/case.jpg)

![Case-front](https://github.com/diyhue/Devices/raw/master/HueMotionSensor/images/case-front.jpg)

![Battery](https://github.com/diyhue/Devices/raw/master/HueMotionSensor/images/battery.jpg)

