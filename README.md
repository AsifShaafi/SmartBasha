# SmartBasha
This is a project for the MSD (Microcontroller Based System Design) Lab, which is taken in AUST in 3rd year 2nd semester. We are building a system whrer anyone can control their home light/fan with their android smart phone from anywhere around the wrold. We have also implemented PIR sensor, which detects movement and automatically turn the light on.

### Video demo: [YouTube](https://www.youtube.com/watch?v=KjK3I9iN3jo)

## Tools and Languages:
  1. Arduino
  2. Android studio
  3. NodeMCU esp8266 WiFi module
  4. Java, PHP, MySql

### Requirements
  1. Arduino Mega/uno (In the project arduino mega was used, and the code is for that, but any arduino will do)
  2. NodeMCU esp8266 wifi module (you can buy it from [TechshopBD](https://www.techshopbd.com/product-categories/wifi/2869/esp8266-nodemcu-lua-wifi-with-cp2102-techshop-bangladesh#!prettyPhoto[pp_gal]/0/))
  3. Android phone for android app
  4. A relay to control the A\C current with the arduino and wifi module. (In the project 4 relay was used)

### Working procedure

#### Connecting nodeMCU wifi module with arduino software

1. Go to preference in arduino software and add "http://arduino.esp8266.com/stable/package_esp8266com_index.json" this link in the 'Additional Boards Manager URLs' box. (without ")
2. Then goto Tools>Board>Boards Manager and search for 'nodemcu esp8266'. Install the driver named 'esp8266 by ESP8266 Community'. Try to download the latest version. By the time of this writting it was 2.4.2 .
3. Now to select the board goto Tools>Board and scroll down a little bit and select 'NodeMCU 1.0(ESP-12E Module)'. There is another one named 'NodeMCU 0.9' but we will use the 1.0 12E version.
4. Change the 'ssid' and 'password' variables with your own wifi name and password.
5. A library name 'ArduinoJson' is required for running the code. So if you already have it, then you are good to go, otherwise install it by following the [instructions on 'arduinojson' website](https://arduinojson.org/v5/doc/installation/)

That's it, you are ready to run code in the wifi module.

The details description can be found [here](https://www.instructables.com/id/Programming-ESP8266-ESP-12E-NodeMCU-Using-Arduino-/)

