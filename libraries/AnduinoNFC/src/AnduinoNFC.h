/*******************************************************************
 Check out the included Arduino sketch examples and Anduino Wiki to
 get started!
 https://github.com/andium/Anduino/wiki
 
 This is a modified version of Don Coleman's NfcAdapter to include
 hardware interrupt support for the PN532 as well as a hardcoded
 SPI interface for the Andium Anduino shield.
 
 Modified by Brian Carbonette Copyright © 2016 Andium
 Original source contributed by Don Coleman https://github.com/don/NDEF
 
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

#ifndef AnduinoNFC_h
#define AnduinoNFC_h

#include <PN532/AnduinoInterface.h>
#include <PN532/PN532Interface.h>
#include <PN532/PN532.h>
#include <NFC/NfcTag.h>
#include <NDEF/Ndef.h>

// Drivers
#include <Mifare/MifareClassic.h>
#include <Mifare/MifareUltralight.h>
#include <AnduinoPins.h>


#define TAG_TYPE_MIFARE_CLASSIC (0)
#define TAG_TYPE_1 (1)
#define TAG_TYPE_2 (2)
#define TAG_TYPE_3 (3)
#define TAG_TYPE_4 (4)
#define TAG_TYPE_UNKNOWN (99)

#define SPI_HAS_TRANSACTION 1


class AnduinoNFC {
    public:
        AnduinoNFC();

       // bool isIsrTaken(void);
        bool packetReady(void);

        ~AnduinoNFC(void);
        void begin(boolean verbose=true);
        boolean tagPresent(unsigned long timeout=0); // tagAvailable
        NfcTag read();
        boolean write(NdefMessage& ndefMessage);
        // erase tag by writing an empty NDEF record
        boolean erase();
        // format a tag as NDEF
        boolean format();
        // reset tag back to factory state
        boolean clean();
    private:
        PN532* shield;
        byte uid[7];  // Buffer to store the returned UID
        unsigned int uidLength; // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
        unsigned int guessTagType();


};

#endif /*AnduinoNFC_h*/
