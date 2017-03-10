 /******************************************************************* 
  Example sketch utilizing AnduinoLCD Library w/ analog input representing 
  the data as a waveform on the LCD.
  
  Check out the included Arduino sketch examples and Anduino Wiki to 
  get started!
  https://github.com/andium/Anduino/wiki

  Written by Brian Carbonette Copyright © 2017 Andium 
  
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

/* Include the AnduinoLCD ST7735 specific library */
#include "AnduinoLCD.h"

AnduinoLCD screen = AnduinoLCD(ST7735_CS_PIN, ST7735_DC_PIN, PERIPH_RST_PIN);

uint8_t pxInc = 0;       //pixel counter to drive x-axis draw(ST7735 is 160x128)
uint8_t sensorPin = A1;  //analog input

void setup() {
  
  // initialize the display
  screen.begin();
  screen.setBacklight(ON);
  screen.fillScreen(ST7735_BLACK);  //clear the screen 
  screen.showBanner();              //load Andium Banner
 
}


void loop() {

//Prints waveform representing Analog Input A0 to LCD
screen.waveform(sensorPin, pxInc);

//Pixel incrementer for waveform 160 is max right index
//for other display sizes adjust 160 to extreme border pixelcount
  if(pxInc == 160){
    pxInc=0;
  }else{
    pxInc++;
  }

delay(250); //delay can be used as a pseudo-sample rate adjustment 


}

