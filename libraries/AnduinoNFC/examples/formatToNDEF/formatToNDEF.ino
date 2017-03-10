 /******************************************************************* 
  Example sketch utilizing AnduinoNFC to format a Mifare Classic
  tag as an NDEF tag. The revertToClassic sketch will reverse this
  process. Mifare Ultralight tags may also be formatted via this 
  sketch but can not be reverted!
  
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
 
/* Include the AnduinoNFC library */
#include <AnduinoNFC.h>

AnduinoNFC NFC = AnduinoNFC();

void setup(void) {
    Serial.begin(115200);
    Serial.println("Mifare Classic/Ultralight --> NDEF");
    
    NFC.begin();        
    
    Serial.println("\nPlace an unformatted Mifare Classic tag on the reader.");

}

void loop(void) {

    /* If(there is an RFID card in range and we haven't just read from it)
     * This utilizes the IRQ pin on the PN532 which is tied to a hardware
     * interrupt on the Anduino shield.
     */
    if (NFC.packetReady()) {

        bool success = NFC.format(); //formats a Mifare Classic card to support NDEF 
        
        if (success) 
        {
          Serial.println("\nSuccess, tag formatted for NDEF.");
        } 
        else 
        {
          Serial.println("\nFormat failed.");
        }

    }
}
