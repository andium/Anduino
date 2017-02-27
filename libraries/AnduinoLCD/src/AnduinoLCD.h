/*******************************************************************
 This library extends Adafruit's ST7735 library for the Andium
 Anduino Shield.
 
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

#ifndef __AnduinoLCD_H
#define __AnduinoLCD_H

#include "Arduino.h"
#include "Adafruit_ST7735.h" // Hardware-specific library
#include "Adafruit_GFX.h"  // Adafruit Graphics Library

#include "AnduinoPins.h"
#include "AnduinoErrorCodes.h"
#include "GimpImageFormat.h"

/*#define DEBUG_AnduinoLCD*/



typedef enum
{
    ON,
    OFF
} BACKLIGHT_STATE;

/*
 *
 */
class AnduinoLCD : public Adafruit_ST7735
{   
    public:

        /* Constructor */
        AnduinoLCD(uint8_t cs, uint8_t rs, uint8_t rst);

        void begin(void);

        Status setBacklight(BACKLIGHT_STATE state);

        Status showGimpImage(gimpImage* image);

    void screenPrint(char* message, int x, int y);
    void showFullscreenLogo(void);
    void showBanner(void);
    void waveform(uint8_t analogPin, uint8_t px);
    void digitalGauge(uint8_t analogPin);
    void splashScreen(void);



    private:
    bool logoFullscreen = true;
       

};

#endif /* __AnduinoLCD_H */
