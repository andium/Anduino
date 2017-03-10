 /******************************************************************* 
  Example sketch utilizing AnduinoNFC Library to read a passive 
  RFID tag.
  
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
 
/* Include the AnduinoNFC PN532 specific library */
#include <AnduinoNFC.h>

AnduinoNFC NFC = AnduinoNFC();


void setup() {
    Serial.begin(115200);
    Serial.println("RFID Tag Reader");
    
    NFC.begin();

    Serial.println("\nScan an NFC tag\n");

}

void loop() {

     /* If(there is an RFID card in range and we haven't just read from it)
     * This utilizes the IRQ pin on the PN532 which is tied to a hardware
     * interrupt on the Anduino shield.
     */
    if (NFC.packetReady())
    {
        /* NfcTag's contain highlevel properties like UID and tag type,
         * check out the keycardAcess sketch to see UIDs in action.
         */
        NfcTag tag = NFC.read();  //attempt to read the RFID tag 
        tag.print();              //and print the results to the terminal
    }
}
