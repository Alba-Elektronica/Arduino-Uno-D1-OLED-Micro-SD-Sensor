/*
 *   
 *   The MIT License (MIT)
 *
 * Copyright (c) 2020 by Alba Elektronica
 *

 * 
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
  

  contributors: David A. Mellis, Tom Igoe, (2010 - MicroSd adapter)
  Daniel Eichhorn, Fabrice Weinberg (ThingPulse), (2018 - SSD1306 Drawing Demo) 
  David Cuartielles, Tom Igoe (2011 - Analog input example) 
  Madalina Drugan (Alba Elektronica) (2020 - current file)

  This example code is in the public domain.

*/

#include <SPI.h>
#include <SD.h>

#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`

//////////////////////////////
// Initialize the OLED display using Wire library
SSD1306Wire  display(0x3c, D3, D5);

// Adapted from Adafruit_SSD1306
void drawLines() {
  for (int16_t i=0; i<display.getWidth(); i+=4) {
    display.drawLine(0, 0, i, display.getHeight()-1);
    display.display();
    delay(10);
  }
  for (int16_t i=0; i<display.getHeight(); i+=4) {
    display.drawLine(0, 0, display.getWidth()-1, i);
    display.display();
    delay(10);
  }
  delay(250);

  display.clear();
  for (int16_t i=0; i<display.getWidth(); i+=4) {
    display.drawLine(0, display.getHeight()-1, i, 0);
    display.display();
    delay(10);
  }
  for (int16_t i=display.getHeight()-1; i>=0; i-=4) {
    display.drawLine(0, display.getHeight()-1, display.getWidth()-1, i);
    display.display();
    delay(10);
  }
  delay(250);

  display.clear();
  for (int16_t i=display.getWidth()-1; i>=0; i-=4) {
    display.drawLine(display.getWidth()-1, display.getHeight()-1, i, 0);
    display.display();
    delay(10);
  }
  for (int16_t i=display.getHeight()-1; i>=0; i-=4) {
    display.drawLine(display.getWidth()-1, display.getHeight()-1, 0, i);
    display.display();
    delay(10);
  }
  delay(250);
  display.clear();
  for (int16_t i=0; i<display.getHeight(); i+=4) {
    display.drawLine(display.getWidth()-1, 0, 0, i);
    display.display();
    delay(10);
  }
  for (int16_t i=0; i<display.getWidth(); i+=4) {
    display.drawLine(display.getWidth()-1, 0, i, display.getHeight()-1);
    display.display();
    delay(10);
  }
  delay(250);
}

// Adapted from Adafruit_SSD1306
void drawRect(void) {
  for (int16_t i=0; i<display.getHeight()/2; i+=2) {
    display.drawRect(i, i, display.getWidth()-2*i, display.getHeight()-2*i);
    display.display();
    delay(10);
  }
}

// Adapted from Adafruit_SSD1306
void fillRect(void) {
  uint8_t color = 1;
  for (int16_t i=0; i<display.getHeight()/2; i+=3) {
    display.setColor((color % 2 == 0) ? BLACK : WHITE); // alternate colors
    display.fillRect(i, i, display.getWidth() - i*2, display.getHeight() - i*2);
    display.display();
    delay(10);
    color++;
  }
  // Reset back to WHITE
  display.setColor(WHITE);
}

// Adapted from Adafruit_SSD1306
void drawCircle(void) {
  for (int16_t i=0; i<display.getHeight(); i+=2) {
    display.drawCircle(display.getWidth()/2, display.getHeight()/2, i);
    display.display();
    delay(10);
  }
  delay(1000);
  display.clear();

  // This will draw the part of the circel in quadrant 1
  // Quadrants are numberd like this:
  //   0010 | 0001
  //  ------|-----
  //   0100 | 1000
  //
  display.drawCircleQuads(display.getWidth()/2, display.getHeight()/2, display.getHeight()/4, 0b00000001);
  display.display();
  delay(200);
  display.drawCircleQuads(display.getWidth()/2, display.getHeight()/2, display.getHeight()/4, 0b00000011);
  display.display();
  delay(200);
  display.drawCircleQuads(display.getWidth()/2, display.getHeight()/2, display.getHeight()/4, 0b00000111);
  display.display();
  delay(200);
  display.drawCircleQuads(display.getWidth()/2, display.getHeight()/2, display.getHeight()/4, 0b00001111);
  display.display();
}

void printBuffer(void) {
  // Initialize the log buffer
  // allocate memory to store 8 lines of text and 30 chars per line.
  display.setLogBuffer(5, 30);

  // Some test data
  const char* test[] = {
      "Hello",
      "World" ,
      "----",
      "Show off",
      "how",
      "the log buffer",
      "is",
      "working.",
      "Even",
      "scrolling is",
      "working"
  };

  for (uint8_t i = 0; i < 11; i++) {
    display.clear();
    // Print to the screen
    display.println(test[i]);
    // Draw it to the internal screen buffer
    display.drawLogBuffer(0, 0);
    // Display it on the screen
    display.display();
    delay(500);
  }
}

void setup_OLED() {
  display.init();

  // display.flipScreenVertically();

  display.setContrast(255);

  drawLines();
  delay(1000);
  display.clear();

  drawRect();
  delay(1000);
  display.clear();

  fillRect();
  delay(1000);
  display.clear();

  drawCircle();
  delay(1000);
  display.clear();

  printBuffer();
  delay(1000);
  display.clear();
}

/////////////////////
// Initialize mini TF
File myFile;

void setup_MiniTF() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
}

/////////////////
//Initialize KY sensor
int sensorPin = A5;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor

void setup_KY(){
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
}

void setup(){
  setup_OLED();
  setup_MiniTF();
  setup_KY();
}

void loop() {
  // read and write data periodically on both screen and Micro SD

  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
   // turn the ledPin on
  digitalWrite(ledPin, HIGH);
 
  //write the value on the screen
  display.println("Sensor =" + sensorValue);
  // extra information may be added here
  display.println("");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("Sensor =" + sensorValue);
    
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  
  // stop the program for 100 milliseconds:
  delay(100);
  // turn the ledPin off:
  digitalWrite(ledPin, LOW);
  // stop the program for 1000 milliseconds before the next read
  delay(1000);

}
