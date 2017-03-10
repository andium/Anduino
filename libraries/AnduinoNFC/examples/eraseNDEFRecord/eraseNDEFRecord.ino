 /******************************************************************* 
  Example sketch utilizing AnduinoNFC to clear an NDEF formatted
  tag by overwritting the contents with an empty record.
  
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
    Serial.println("NDEF Eraser");
    
    NFC.begin();

    Serial.println("\nPlace an NDEF formatted tag on the NFC reader to erase.");

}

void loop(void) {

    if (NFC.packetReady()) {

        bool success = NFC.erase();  //essentially message.addEmptyRecord()

        if (success) 
        {
          Serial.println("Success! Tag erased!");  
          NfcTag tag = NFC.read(); 
          tag.print(); //print the contents to confirm the tag is erased 
        } 
        else 
        {
          Serial.println("Erase failed.");
        }

    }
}
