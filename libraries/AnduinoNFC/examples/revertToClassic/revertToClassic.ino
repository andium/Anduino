 /******************************************************************* 
  Example sketch utilizing AnduinoNFC to revert an NDEF formatted
  Mifare Classic RFID tag back to its original configuration. Mifare
  Ultralight cards will remain NDEF and be cleared not reformatted.
  
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
 
/* Include the AnduinoNFC library */
#include <AnduinoNFC.h>

AnduinoNFC NFC = AnduinoNFC();

void setup(void) {
    Serial.begin(115200);
    Serial.println("NDEF --> Mifare Classic");
    
    NFC.begin();

    Serial.println("\nPlace an NDEF formatted tag on the NFC reader to revert back to Mifare Classic.");

}

void loop(void) {

    /* If(there is an RFID card in range and we haven't just read from it)
     * This utilizes the IRQ pin on the PN532 which is tied to a hardware
     * interrupt on the Anduino shield.
     */
    if (NFC.packetReady()) {

        bool success = NFC.clean();  //reformat the RFID tag
        
        if (success) 
        {
            Serial.println("\nSuccess, tag restored to factory state.");
        } 
        else 
        {
            Serial.println("\nError, unable to clean tag.");
        }

    }
}
