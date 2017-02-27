 /******************************************************************* 
  Example sketch utilizing AnduinoEEPROM Library to read contents of 
  EEPROM in 64byte pages to terminal display.
  
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

/* Include the AnduinoEEPROM specific library */
#include "AnduinoEEPROM.h"

AnduinoEEPROM EEPROM = AnduinoEEPROM();

void setup() {

  Serial.begin(115200);
  EEPROM.begin();

  int eeaddress = 0;   //eeaddress start address (best to do this in page sized increments, ex: 0, 64, 128...) 
  int numPages = 3;    //(512) * 64byte wide pages in 32Kb EEPROM prints entire contents
  byte bufferPage[PAGE_SIZE] = { 0 };
  int next = 0;
  
  //initialize write buffer with test data
  for(int i=0; i<PAGE_SIZE; i++)
      {
          bufferPage[i]=i; 
      }

  Serial.println("Write test pages of data");
  //write each page of test data to EEPROM    
  for(int j=numPages; j>0; j--)
  {            
    EEPROM.writePage((eeaddress+next), (uint8_t *)bufferPage, PAGE_SIZE);
    next += 64; //increment to the next page
    delay(50);
  }
      
  //Print the page/s we just wrote (numPages = 512 to print entire EEPROM contents)
  EEPROM.printToTerm(eeaddress, numPages);

  //Erase the page/s we just wrote
  EEPROM.eraseContents(eeaddress, numPages);
  Serial.println("Erased test data!");

  //Print the EEPROM again
  EEPROM.printToTerm(eeaddress, numPages);
        
}

void loop() 
{
    //Save some write cycles on the EEPROM! We'll just run this example once.
}
