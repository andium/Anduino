 /******************************************************************* 
  Example sketch utilizing AnduinoEEPROM library to write a byte, 
  read a byte, and display the data on the AnduinoLCD or in terminal.
  
  Check out the included Arduino sketch examples and Anduino Wiki to 
  get started!
  https://github.com/andium/Anduino/wiki

  Written by Brian Carbonette Copyright © 2016 Andium 
  
  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at
 
  http://www.apache.org/licenses/LICENSE-2.0
 
  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 *******************************************************************/
 
/* Include the AnduinoLCD and AnduinoEEPROM specific libraries */
#include "AnduinoEEPROM.h"
#include "AnduinoLCD.h"

AnduinoEEPROM EEPROM = AnduinoEEPROM();
AnduinoLCD LCD = AnduinoLCD(ST7735_CS_PIN, ST7735_DC_PIN, PERIPH_RST_PIN);

void setup() {

  /*-----------------------------------------------------   
    Set up the EEPROM, serial comm, and LCD screen
    -----------------------------------------------------*/
  Serial.begin(115200);
  
  EEPROM.begin();
  
  LCD.begin();
  LCD.fillScreen(ST7735_BLACK);  //clear the screen
  LCD.setBacklight(ON);
  LCD.splashScreen();
  delay(500);                    //gaze upon it
  LCD.fillScreen(ST7735_BLACK);  
  LCD.showBanner();
  LCD.setCursor(1, 40);
  LCD.setTextSize(1);
  LCD.setTextWrap(ON);


  /*-----------------------------------------------------
    Where the magic happens
    -----------------------------------------------------*/
  byte eebyte = 0x06;                 //what to write
  int eeaddress = 0;                  //where to write it

  //Write to eebyte to EEPROM at eeaddress
  EEPROM.write(eeaddress, eebyte);
  LCD.print("Write:0x");
  LCD.print(eebyte, HEX);
  LCD.println();

  //Read back what we just wrote
  Serial.print(EEPROM.read(eeaddress)); //on the serial terminal
  LCD.print("EEPROM[");
  LCD.print(eeaddress);                 //also to the LCD 
  LCD.print("]:0x");
  LCD.print(EEPROM.read(eeaddress), HEX);


}

void loop() 
{
  //Save some write cycles on the EEPROM! We'll just run this example once.
}


 
