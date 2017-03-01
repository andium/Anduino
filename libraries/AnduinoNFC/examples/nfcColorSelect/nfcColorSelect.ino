 /******************************************************************* 
  Example sketch utilizing AnduinoNFC and AnduinoLCD library to 
  illustrate how an NDEF text record stored on an RFID tag
  could be used to configure an environment in realtime. When a tag 
  is scanned the message is then interpreted as a 16 bit color
  value, the text on the display then immediately changes to that 
  color. If a scanned color is not recognized a random color will
  be generated.
  
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
    
    LCD.setTextSize(1);
    LCD.setCursor(0, 40); 
    LCD.setTextColor(ST7735_WHITE);
    LCD.print("Tap Anduino w/ an NFC tag");
    LCD.setCursor(0, 50);
    LCD.print("to change the color of the");
    LCD.setCursor(0, 60);
    LCD.print("text"); 

  

    //Serial deubgging
    Serial.begin(115200);
    while(!Serial)
    {
      ;
    }

    //Start up NFC
    NFC.begin(); 

}


void loop() 
{
    nfcColorSelect();
}

void nfcColorSelect(void)
{
  
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
        byte temp[payloadLength];
        uint16_t color;
        String rfidColor="";
        /*Stores the payload in playloadBytes buffer
         */
        record.getPayload(payloadBytes);

        /* Change the text display color sent over in the text record
         * w/ 16-bit default color values
         */
        //strip the encoding
        for(int i=3; i<payloadLength; i++)
        {
          temp[i-3] = payloadBytes[i];
        }
        //cast to string
        rfidColor = (const char*)temp;
        
        color = colorPicker(rfidColor);
        
        LCD.setTextColor(color);
        LCD.setCursor(0, 40); 
        LCD.print("Tap Anduino w/ an NFC tag");
        LCD.setCursor(0, 50);
        LCD.print("to change the color of the");
        LCD.setCursor(0, 60);
        LCD.print("text"); 
     }
}


/* Returns the 16-bit val for some pre-set 
 * color definitions or a random number
#define ST7735_BLACK   0x0000
#define ST7735_BLUE    0x001F
#define ST7735_RED     0xF800
#define ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW  0xFFE0
#define ST7735_WHITE   0xFFFF
 */
uint16_t colorPicker(String rfidColor)
{
  if(rfidColor == "white")
  {
      return ST7735_WHITE;
  }
  else if(rfidColor == "red")
  {
      return ST7735_RED;
  }
  else if(rfidColor == "blue")
  {
      return ST7735_BLUE;
  }
  else if(rfidColor == "black")
  {
      return ST7735_BLACK;
  }
   else if(rfidColor == "green")
  {
      return ST7735_GREEN;
  }
   else if(rfidColor == "cyan")
  {
      return ST7735_CYAN;
  }
   else if(rfidColor == "magenta")
  {
      return ST7735_MAGENTA;
  }
   else if(rfidColor == "yellow")
  {
      return ST7735_YELLOW;
  }
   else
   {
    return random(65535);
   }
}
