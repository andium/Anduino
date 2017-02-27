/*******************************************************************
 Check out the included Arduino sketch examples and Anduino Wiki to
 get started!
 https://github.com/andium/Anduino/wiki
 
 This is a modified version of PN532_SPI only renamed to reinforce
 that the PN532_SPI interface has been hardcoded for the Andium
 Anduino shield.
 
 Modified by Brian Carbonette Copyright © 2016 Andium
 Original source contributed by xiongyihui as well as SeedStudio 
 and Adafruit can be found here https://github.com/Seeed-Studio/PN532 
 
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
/*Anduino PN532_SPI*/

#ifndef __AnduinoInterface_H
#define __AnduinoInterface_H

#include "Arduino.h"
#include "AnduinoPins.h"

#include "PN532/PN532.h"
#include "AnduinoNFC.h"

#include <SPI.h>
#include "PN532/PN532Interface.h"


class AnduinoInterface : public PN532Interface {
public:
    AnduinoInterface(SPIClass &spi, uint8_t ss);
    void begin();
    void wakeup();
    int8_t writeCommand(const uint8_t *header, uint8_t hlen, const uint8_t *body = 0, uint8_t blen = 0);
    int16_t readResponse(uint8_t buf[], uint8_t len, uint16_t timeout);
    
private:
    SPIClass* _spi;
    uint8_t   _ss;
    uint8_t command;
    
    boolean isReady();
    void writeFrame(const uint8_t *header, uint8_t hlen, const uint8_t *body = 0, uint8_t blen = 0);
    int8_t readAckFrame();
    
    inline void write(uint8_t data) {

        _spi->transfer(data);

    };

    inline uint8_t read() {  

    int8_t x;
        x = 0;
        x = _spi->transfer(0);  

    return x;

    }; 


};

#endif /* __AnduinoInterface_H */
