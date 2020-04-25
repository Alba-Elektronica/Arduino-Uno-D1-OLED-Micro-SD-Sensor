# Arduino-Uno-D1-OLED-Micro-SD-Sensor
Arduino Uno D1 in a project where  a sensor generates data written on a Micro SD card

 SD card read/write + OLED + Photosensitive sensor on an Arduino Uno board

  This example shows how to read from the SD card file on OLED / write on the SD card the data from a sensor 
  
  The circuit:
   --> SD module attached to SPI bus as follows:
   
 ** GND - GND 
 ** VCC - +5V 
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

  --> Initialize the OLED display 

 ** GND - GND 
 ** VCC - +3.3V 
 ** D3 -> SDA
 ** D5 -> SCL

  --> Initialize the KY sensor

** S - A5  
** + - +5V
** - - GND
  
