 /******************************************************************* 
  Example sketch utilizing AnduinoNFC and AnduinoLCD library to 
  create a simple RFID card access terminal. This sketch only prints
  information to the screen, but you could extend this to actuate a 
  lock with a relay and the right locking mechanism! The sketch 
  compares the UID of the scanned card to the pre programmed ID's 
  defined at the top of the sketch.
  
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

/* Replace these UIDs with the UIDs on your RFID tags. If 
 *  you're unsure what they are run the readRFIDtag sketch
 *  and check the tag info that's printed to the terminal.
 *  Depending on the card type the length will will vary.
 */
#define BRIAN_ID "BA 61 20 00"  //UID from Mifare KeyCard
#define DOOLEY_ID "12 AA 1A 00" //UID from Mifare KeyCard

AnduinoNFC NFC = AnduinoNFC();
AnduinoLCD LCD = AnduinoLCD(ST7735_CS_PIN, ST7735_DC_PIN, PERIPH_RST_PIN);
          
void setup() {
    //Serial.begin(115200);
    
    /* AnduinoLCD setup
     */
    LCD.begin();
    LCD.setBacklight(ON);
    LCD.fillScreen(ST7735_BLACK);  //clear the screen   
    LCD.showBanner();              //load Andium Banner

    //print scan instructions to screen
    scanInst();

    //Start up NFC
    NFC.begin();
}


void loop() 
{
    keyCardScan();
}

void keyCardScan(void)
{    
    /* If(there is an RFID card in range and we haven't just read from it)
     * This utilizes the IRQ pin on the PN532 which is tied to a hardware
     * interrupt on the Anduino shield.
     */
    if (NFC.packetReady())
    {     
        /* Grab a tag object of the card we're reading
         * Mifare Classic and Ultra-light supported
         * and read the tag
         */
        NfcTag tag = NFC.read();

        //grab the "fairly" unique UID 
        String UID = tag.getUidString();  

        if(UID == BRIAN_ID)
        {
          //Open the lock!
          //clear screen
          clearScreen();
          
          //Serial.println("ACCESS GRANTED - Welcome Brian");
          LCD.setTextSize(2);
          LCD.stroke(255,255,255);
          LCD.setTextWrap(true);
          LCD.screenPrint("Welcome Brian Carbonette", 0, 40);
          delay(2000);
          

          //clear screen 
          clearScreen();
          delay(500);
          //put back instructions
          scanInst();
          
        }
        else if(UID == DOOLEY_ID)
        {
          //Open the lock!
          //clear screen
          clearScreen();
          
          //Serial.println("ACCESS GRANTED - Welcome Dooley");
          LCD.setTextSize(2);
          LCD.stroke(255,255,255);
          LCD.setTextWrap(true);
          LCD.screenPrint("Welcome James Dooley", 0, 40);
          delay(2000);

           //clear screen 
          clearScreen();
          delay(500);
          
          //put back instructions
          scanInst();
        }
        else
        {
          //keep that door locked!
          //Serial.println("ACCESS DENIED");
          accessDenied(); //flashes access denied to the LCD
        }
       
        /* TODO maybe modify SPI library to map BitOrder to specific SS like
         *  bus speed, for now this is necessary to put bus back in NFC mode
         *  after LCD switches to MSBFIRST
         */
        SPI.setBitOrder(LSBFIRST);
    }
    else
    {
      //no card present to be read
    }
}

//takes care of setting up the scren to display "Access Denied"
void accessDenied(void)
{
  //clear screen
  clearScreen();
  
  int i=5;
  while(i>0)
  { LCD.setTextSize(2);
    LCD.stroke(229,24,24);
    LCD.text("Access Denied", 0, 40);
    delay(500);
    clearScreen();
    i--;
  }
    //clear screen
    clearScreen();
     //put back instructions
    scanInst();
  
}


void clearScreen(void)
{
   LCD.stroke(1,1,1);
   LCD.fill(1,1,1);
   LCD.rect(0,40,LCD.width(),100);
}

//default screen image
void scanInst(void)
  {
    LCD.stroke(255, 255, 255); //white
    LCD.setTextSize(2);
    LCD.setTextWrap(true);
    LCD.screenPrint("Scan keycard to enter!", 0, 40); 
}

