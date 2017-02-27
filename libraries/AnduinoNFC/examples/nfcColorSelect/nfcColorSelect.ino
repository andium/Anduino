 /******************************************************************* 
  Example sketch utilizing AnduinoNFC and AnduinoLCD library to 
  illustrate how a mime media record stored on an RFID tag
  could be used to configure an environment in realtime. When a tag 
  is scanned the message is then interpreted as an r,g,b color
  value, the text on the display then immediately changes to that 
  color.
  
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
 
/* Include the AnduinoNFC and AnduinoLCD library */
#include <AnduinoLCD.h>
#include <AnduinoNFC.h>

AnduinoNFC NFC = AnduinoNFC();
AnduinoLCD LCD = AnduinoLCD(ST7735_CS_PIN, ST7735_DC_PIN, PERIPH_RST_PIN);   

void setup() {
   
    /* AnduinoLCD setup
     */
    LCD.begin();
    LCD.setBacklight(ON);
    LCD.fillScreen(ST7735_BLACK);  //clear the screen   
    LCD.showBanner();              //load Andium Banner
    LCD.stroke(255, 255, 255);     //white
    LCD.setTextSize(1);
    LCD.text("Tap Anduino w/ an NFC tag", 0, 40); 
    LCD.text("to change the color of the", 0, 50);
    LCD.text("text", 0, 60); 

    //Start up NFC
    NFC.begin(); 
}


void loop() 
{
    nfcColorSelect();
}

void nfcColorSelect(void)
{
  
  	//TODO add verification of TNF/MIME type 'text/pixelColor'
    /* If(there is an RFID card in range and we haven't just read from it)
     * This utilizes the IRQ pin on the PN532 which is tied to a hardware
     * interrupt on the Anduino shield.
     */
    if (NFC.packetReady()) 
    {     
        /* Grab a tag object of the card we're reading
         * Mifare Classic and Ultra-light supported
         */
        NfcTag tag = NFC.read();
        /* To grab the payload we need the NDEF message
         */
        NdefMessage message = tag.getNdefMessage();
        /* We stored the color in the first record
         * when the card was programmed 
         */
        NdefRecord record = message.getRecord(0);
        /* Create local storage based off the size of the 
         *  payload.
         */
        int payloadLength = record.getPayloadLength();
        byte payloadBytes[payloadLength];
        /*Stores the payload in playloadBytes buffer
         */
        record.getPayload(payloadBytes);

        /* Change the text display color sent over in the MIME 'text/pixelColor'
         * w/ 3 byte payload in r,g,b
         */
        LCD.stroke(payloadBytes[0], payloadBytes[1], payloadBytes[2]);
        LCD.text("Tap Anduino w/ an NFC tag", 0, 40); 
        LCD.text("to change the color of the", 0, 50);
        LCD.text("text", 0, 60); 

        /* TODO maybe modify SPI library to map BitOrder to specific SS like
         *  bus speed, for now this is necessary to put bus back in NFC mode
         */
        SPI.setBitOrder(LSBFIRST);
     }
}
