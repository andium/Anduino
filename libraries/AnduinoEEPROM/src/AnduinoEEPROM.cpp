/******************************************************************* 
  This library provides basic functionality around the M24256 32Kb 
  I2C connected EEPROM available in our Anduino shield fo Arduino.
  
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

#include "AnduinoEEPROM.h"


extern gimpImage andiumLogoFullEEPROM;


AnduinoEEPROM::AnduinoEEPROM()
{

}


void AnduinoEEPROM::begin()
{
    #ifdef DEBUG_AnduinoEEPROM
    Serial.print("EEPROM Ready! \n");
    #endif

    WIRE.begin();           //begin I2C
    WIRE.setClock(1000000); //set clock freq to 1Mhz

}

/*parse GIMP img pixel by pixel in sequence so drawLogofromEEPROM can assume
eeaddress = 0 is pixel coord 0,0)*/
int AnduinoEEPROM::storeGimpImage(gimpImage* image, unsigned int eeaddress)
{
    uint16_t index, block;
    byte MSB, LSB;

    for(int y=0; y<image->height; y++)
    {
      Serial.println(y);
        block = y*image->width;
        for(int x=0; x<image->width; x++)
        {

            index = (block + x) *2;
            MSB = image->pixel_data[index+1]; //MSB
            //write byte to address i
            write(eeaddress, MSB);

            LSB = image->pixel_data[index]; //LSB
            //write byte to address i+1
            write(eeaddress+1, LSB);

            eeaddress += 2;  //increment to the next byte of pixel data
          

            if(eeaddress > EEPROM_SIZE-1)
            {
                Serial.print("Image too large to fully store in EEPROM");
                return 1;

            }

        }
    }
    return 0;
}

//pass example GIMP img to store function
void AnduinoEEPROM::storeImg(unsigned int eeaddress)
{
  storeGimpImage(&andiumLogoFullEEPROM, eeaddress);
}

//Write a byte to EEPROM
void AnduinoEEPROM::write(unsigned int eeaddress, byte data ) 
{

  WIRE.beginTransmission(EEPROM_ADDR);
  WIRE.write((int)(eeaddress >> 8));   // MSB
  WIRE.write((int)(eeaddress & 0xFF)); // LSB
  WIRE.write(data);
  WIRE.endTransmission();
 
  delay(5);
}

//Read a byte from EEPROM
byte AnduinoEEPROM::read(unsigned int eeaddress ) 
{
  byte rdata = 0xFF;
 
  WIRE.beginTransmission(EEPROM_ADDR);
  WIRE.write((int)(eeaddress >> 8));   // MSB
  WIRE.write((int)(eeaddress & 0xFF)); // LSB
  WIRE.endTransmission();
 
  WIRE.requestFrom(EEPROM_ADDR,1);
 
  return rdata = WIRE.read();
 
}

/*read a page sized chunk of EEPROM starting at eeaddress and store in buffer*/
void AnduinoEEPROM::readPage(unsigned int eeaddress, byte buffer[], int pageSize)
{
 WIRE.beginTransmission(EEPROM_ADDR);
 WIRE.write((int)(eeaddress >> 8)); // MSB
 WIRE.write((int)(eeaddress & 0xFF)); // LSB
 WIRE.endTransmission();
 WIRE.requestFrom(EEPROM_ADDR, pageSize);
 
 //Read one PAGE_SIZE page from EEPROM and store in buffer
 for (int i=0; i<pageSize; i++){
   if (WIRE.available())
   {
     buffer[i] = WIRE.read();
   }
 }
}

/*passes byte buffer to I2C txBuffer to be sent to EEPROM in one transmission
reduces overhead of transmitting two address bytes for each byte to be stored*/
int AnduinoEEPROM::writePage(unsigned int eeaddress, byte* buffer, int pageSize)
{
    //TODO - remove pageSize param and utilize buffer.length 
    if(pageSize>PAGE_SIZE) 
    {
        Serial.print("pageSize larger than configured PAGE_SIZE ");
        return 1;
    }

    WIRE.beginTransmission(EEPROM_ADDR);
    WIRE.write((int)(eeaddress >> 8)); // MSB
    WIRE.write((int)(eeaddress & 0xFF)); // LSB
   
    WIRE.write(buffer, pageSize); 
    
    WIRE.endTransmission();

    delay(5);  //give EEPROM time to complete write before utilizing data

    return 0;

}

//print n pages of EEPROM to terminal (512page max 512*64=32768) starting at eeaddress
void AnduinoEEPROM::printToTerm(unsigned int eeaddress, int numPages){
 
    byte bufferPage[PAGE_SIZE];  //buffer to store one page of mem

       if(eeaddress < PAGE_SIZE) //corner case
       {

          for(int i=eeaddress; i<numPages*PAGE_SIZE; i+=PAGE_SIZE)
          {   
              {
                  Serial.print("0x");
                  Serial.print(i, HEX);
                  Serial.print("-");
                  Serial.print("0x");
                  Serial.print(i+(PAGE_SIZE-1), HEX);
                  Serial.print(":  ");   
                  readPage(i, bufferPage, PAGE_SIZE); //read each page and inc to next page in mem


                 for(int i=0; i<PAGE_SIZE; i++)  //loop to print each page of mem
                {
                     Serial.print("0x");
                     Serial.print(bufferPage[i], HEX);
                     Serial.print("  ");
                }
                
                Serial.println();
              }
          
          }

       } else
       {
        for(int i=eeaddress; i<=numPages*PAGE_SIZE; i+=PAGE_SIZE)
        {   
            {
                Serial.print("0x");
                Serial.print(i, HEX);
                Serial.print("-");
                Serial.print("0x");
                Serial.print(i+(PAGE_SIZE-1), HEX);
                Serial.print(":  ");   
                readPage(i, bufferPage, PAGE_SIZE); //read each page and inc to next page in mem


               for(int i=0; i<PAGE_SIZE; i++)  //loop to print each page of mem
              {
                   Serial.print("0x");
                   Serial.print(bufferPage[i], HEX);
                   Serial.print("  ");
              }
              
              Serial.println();
            }
        
        }

       }
      
}

//write 0x00 to all bytes one page at a time starting at eeaddress for consecutive numPages
void AnduinoEEPROM::eraseContents(unsigned int eeaddress, int numPages)
{
    byte deleteBuffer[PAGE_SIZE] = { 0 };  //hardcoded page sized buffer initialized to 0

    //starting at supplied address delete each successive page until N(numPages) pages are cleared
    for(eeaddress; eeaddress<numPages*PAGE_SIZE; eeaddress+=PAGE_SIZE)
    {  
        writePage(eeaddress, deleteBuffer, PAGE_SIZE);
    }

}










