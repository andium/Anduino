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

#ifndef __AnduinoEEPROM_H
#define __AnduinoEEPROM_H

#include "Arduino.h"
#include "Wire.h"  //I2C utilizing 'Wire1' for second interface SDA2 + SCL2


#include "GimpImageFormat.h"
#define EEPROM_ADDR 0x50  //slaveAddr on I2C bus sda2/scl2
#define EEPROM_SIZE 32768 //bytes
#define PAGE_SIZE 64      //bytes

/*In order to utilize a PAGE_SIZE of 64bytes BUFFER_LENGTH in the Wire.h must be edited
to 66 in order to store the buffer and two address bytes
#define BUFFER_LENGTH 66 edited in /wire.h
otherwise a PAGE_SIZE of 32 can safely be used */

/*#define DEBUG_AnduinoEEPROM*/




class AnduinoEEPROM 
{
	
	public:
		/*Constructor*/
		AnduinoEEPROM();

		void begin(void);
		void storeImg(unsigned int eeaddress);
  		void write(unsigned int eeaddress, byte data);
        byte read(unsigned int eeaddress);
        int writePage(unsigned int eeaddress, byte* buffer, int pageSize);
        void readPage(unsigned int eeaddress, byte buffer[], int pageSize);
        void printToTerm(unsigned int eeaddress, int numPages);
        void eraseContents(unsigned int eeaddress, int numPages);




	private:
		int storeGimpImage(gimpImage* image, unsigned int eeaddress);

};

#endif /*__AnduinoEEPROM_H */
