# GH03

## ENC28J60 on a Nano (ATmega328P) as webserver
Control it with a browser on your computer, tablet or mobil or maybe with the timer example on Node-RED.
Or use the four manual-override buttons to control the four relays.
Nothing fancy with the simple code and Node-RED example that needs to cleaned up, made beter or even shorter.
But for now, it works.

![Diagram](https://github.com/Allday3D/GH03/blob/master/gh03_enc28j60_07.jpg)

## Used Hardware
The project is based on the
- ENC28J60,

placed on a

- Nano (ATmega328P).
- 4 way relayboard.
- MP1584EN module.
- Four switches (4 buttons and 10K resistors).
- 12 volt powersupply.

### Used Libraries
This project uses libraries and code by different authors:

For Arduino IDE:

https://github.com/njh/EtherCard EtherCard is a driver for the Microchip ENC28J60 chip

https://github.com/lexus2k/ssd1306 SSD1306 driver is Arduino style C/C++ library

https://github.com/PaulStoffregen/Time the Arduino Time Library

For Node-RED:

https://flows.nodered.org/node/node-red-contrib-schedex

## To Do

What about using MDNS!?

Webpage!? Or at least a bit more info.

![Web view](https://github.com/Allday3D/GH03/blob/master/web_view.jpg)

That time thing. NTP request at midnight.



