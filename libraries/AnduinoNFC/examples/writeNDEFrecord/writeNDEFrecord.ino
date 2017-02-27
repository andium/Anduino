 /******************************************************************* 
  Example sketch utilizing AnduinoNFC Library to write an NDEF 
  formatted message onto a passive RFID tag.
  
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
 
/* Include the AnduinoNFC PN532 specific library */
#include <AnduinoNFC.h>

AnduinoNFC NFC = AnduinoNFC();

void setup() {
      Serial.begin(115200);
      Serial.println("NDEF Writer");
      
      NFC.begin();

      Serial.println("\nPlace an NDEF formatted Mifare Classic or Ultralight NFC tag on the reader.");
}

void loop() {

    /* If(there is an RFID card in range and we haven't just read from it)
     * This utilizes the IRQ pin on the PN532 which is tied to a hardware
     * interrupt on the Anduino shield.
     */
    if (NFC.packetReady()) {

        /* Create an NDEF message
         */
        NdefMessage message = NdefMessage();
        /* In this instance we'll add a uri, uncomment a message type
         * below to give a different type a try, keep in mind space 
         * is usually pretty limited on RFID tags so we probably 
         * can't fit all 5 records.
         */
        message.addUriRecord("https://github.com/andium/Anduino/wiki");
        
        //message.addMimeMediaRecord("text/html", "<li>anotha one</li>");
        //message.addTextRecord("I'm a generic text record in english");
        //message.addTextRecord("Ciao mondo", "it"); //include an encoding 
        //message.addEmptyRecord(); //boring, but use this to erase! or NFC.erase();
        
       bool success = NFC.write(message); //write this to our tag
      
        if (success) 
        {
          Serial.println("Success! Tag written!");  
          NfcTag tag = NFC.read(); 
          tag.print(); //print what we've just written. 
        } 
        else 
        {
          Serial.println("Write failed.");
        }
    }
    
}
