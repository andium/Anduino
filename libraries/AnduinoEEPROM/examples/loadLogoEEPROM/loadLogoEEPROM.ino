 /******************************************************************* 
  Example sketch utilizing AnduinoEEPROM library to load a GIMP image
  into EEPROM and then display the image read directly from EEPROM
  pixel by pixel. 
  
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

  Serial.begin(115200);
  EEPROM.begin();
  LCD.begin();
  LCD.fillScreen(ST7735_BLACK);  //clear the screen
  LCD.setBacklight(ON);

  int eeaddress = 0; //address of first byte of GIMP in EEPROM

  //If img hasn't been stored in EEPROM uncomment below
  //max GIMP size of 32kb
  //EEPROM.storeImg(eeaddress); //use storeImg to save a GIMP style image contiguously in EEPROM

  drawLogofromEEPROM(eeaddress);

}

void loop() 
{
    //Save some write cycles on the EEPROM! We'll just run this example once.
}

void drawLogofromEEPROM(int eeaddress){
  
  uint16_t pixel;
  //eeaddress address of first byte in GIMP file stored in EEPROM

    for(int y=0; y<128; y++)                      //repeat for each row y    
    {
        for(int x=0; x<160; x++)                  //each column x, 20480px display
        {
            pixel = EEPROM.read(eeaddress+1);     //concatenate 2bytes from EEPROM 
            pixel |= EEPROM.read(eeaddress) << 8; //into one 16bit pixel
            LCD.drawPixel(x, y, pixel);
            eeaddress+=2;                         //inc to the next pixel
        }
    }
}
 
