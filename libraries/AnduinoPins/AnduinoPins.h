/*******************************************************************
 Pin assignments for CC1200 Radio (SPI), PN532 NFC (SPI),
 ST7735 TFT (SPI), ATWINC1500 Radio (SPI) and EEPROM (I2C) on 
 Anduino shield.
 
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

#define AnduinoWiFi
//#define AnduinoSubG

#ifdef AnduinoWiFi
/* Anduino pin definitions */
#define ARDUINO_RST_PIN     A0       /* Reset for Arduino itself */
#define PERIPH_RST_PIN      5        /* Reset signal for CC1200 radio, PN532 NFC and ST7735 TFT */

#define ST7735_CS_PIN       9        /* ST7735 chip select pin */
#define ST7735_DC_PIN       8        /* ST7735 DC (data / command) pin */
#define ST7735_BKLIT_PIN    4        /* ST7735 backlight pin */

#define PN532_CS_PIN        6        /* PN532 chip select pin */
#define PN532_IRQ_PIN       2        /* PN532 IRQ pin */

#elif defined AnduinoSubG

/* Anduino pin definitions */
#define ARDUINO_RST_PIN     A0       /* Reset for Arduino itself */
#define PERIPH_RST_PIN      14        /* Reset signal for CC1200 radio, PN532 NFC and ST7735 TFT */

#define CC1200_CS_PIN 	    9        /* CC1200 chip select pin */
#define CC1200_IRQ1_PIN     3        /* CC1200 IRQ pin (GPIO2) */
#define CC1200_IRQ2_PIN     10       /* CC1200 IRQ pin (GPIO3) */

#define ST7735_CS_PIN       7        /* ST7735 chip select pin */
#define ST7735_DC_PIN       8        /* ST7735 DC (data / command) pin */
#define ST7735_BKLIT_PIN    5        /* ST7735 backlight pin */

#define PN532_CS_PIN        6        /* PN532 chip select pin */
#define PN532_IRQ_PIN       2        /* PN532 IRQ pin */

#endif


