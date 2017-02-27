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

#include <AnduinoNFC.h>

static volatile uint8_t _isrTaken = false;  //ISR semaphore for PN532 IRQ
unsigned long _lastReceiveTime = 0;         //used to debounce reads

/*The PN532 on the Anduino shield can only be accessed via SPI
  to utilize this library with a different shield just repalce the 
  AnduinoInterface with one of xiongyihui's interface files found
  here https://github.com/Seeed-Studio/PN532 */
AnduinoInterface PN532_SPI = AnduinoInterface(SPI, PN532_CS_PIN);


static void PN532ISR(void)
{
    _isrTaken = true;
}

AnduinoNFC::AnduinoNFC()
{
    shield = new PN532(PN532_SPI);
}

AnduinoNFC::~AnduinoNFC(void)
{
    delete shield;
}

void AnduinoNFC::begin(boolean verbose)
{
    shield->begin();

    uint32_t versiondata = shield->getFirmwareVersion();

    if (! versiondata)
    {
        Serial.print(F("Didn't find PN53x board"));
        while (1); // halt
    }

    if (verbose)
    {
        Serial.print(F("Found chip PN5")); Serial.println((versiondata>>24) & 0xFF, HEX);
        Serial.print(F("Firmware ver. ")); Serial.print((versiondata>>16) & 0xFF, DEC);
        Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
    }
    // configure board to read RFID tags
    shield->SAMConfig();

     //attach the interrupt for PN532 IRQ
    pinMode(PN532_IRQ_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(PN532_IRQ_PIN), PN532ISR, FALLING);
}

/*bool AnduinoNFC::isIsrTaken(void)
{
    return _isrTaken;
}*/

boolean AnduinoNFC::tagPresent(unsigned long timeout)
{
    uint8_t success;
    uidLength = 0;

    if (timeout == 0)
    {
        success = shield->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, (uint8_t*)&uidLength);
    }
    else
    {
        success = shield->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, (uint8_t*)&uidLength, timeout);
    }
    return success;
}

boolean AnduinoNFC::erase()
{
    boolean success;
    NdefMessage message = NdefMessage();
    message.addEmptyRecord();
    return write(message);
}

boolean AnduinoNFC::format()
{
    boolean success;
    if (uidLength == 4)
    {
        MifareClassic mifareClassic = MifareClassic(*shield);
        success = mifareClassic.formatNDEF(uid, uidLength);
    }
    else
    {
        Serial.print(F("Unsupported Tag."));
        success = false;
    }
    return success;
}

boolean AnduinoNFC::clean()
{
    uint8_t type = guessTagType();

    if (type == TAG_TYPE_MIFARE_CLASSIC)
    {
        #ifdef NDEF_DEBUG
        Serial.println(F("Cleaning Mifare Classic"));
        #endif
        MifareClassic mifareClassic = MifareClassic(*shield);
        return mifareClassic.formatMifare(uid, uidLength);
    }
    else if (type == TAG_TYPE_2)
    {
        #ifdef NDEF_DEBUG
        Serial.println(F("Cleaning Mifare Ultralight"));
        #endif
        MifareUltralight ultralight = MifareUltralight(*shield);
        return ultralight.clean();
    }
    else
    {
        Serial.print(F("No driver for card type "));Serial.println(type);
        return false;
    }

}


NfcTag AnduinoNFC::read()
{
    uint8_t type = guessTagType();

    if (type == TAG_TYPE_MIFARE_CLASSIC)
    {
        #ifdef NDEF_DEBUG
        Serial.println(F("Reading Mifare Classic"));
        #endif
        MifareClassic mifareClassic = MifareClassic(*shield);
        return mifareClassic.read(uid, uidLength);
    }
    else if (type == TAG_TYPE_2)
    {
        #ifdef NDEF_DEBUG
        Serial.println(F("Reading Mifare Ultralight"));
        #endif
        MifareUltralight ultralight = MifareUltralight(*shield);
        return ultralight.read(uid, uidLength);
    }
    else if (type == TAG_TYPE_UNKNOWN)
    {
        Serial.print(F("Can not determine tag type"));
        return NfcTag(uid, uidLength);
    }
    else
    {
        Serial.print(F("No driver for card type "));Serial.println(type);
        // TODO should set type here
        return NfcTag(uid, uidLength);
    }

}

boolean AnduinoNFC::write(NdefMessage& ndefMessage)
{
    boolean success;
    uint8_t type = guessTagType();

    if (type == TAG_TYPE_MIFARE_CLASSIC)
    {
        #ifdef NDEF_DEBUG
        Serial.println(F("Writing Mifare Classic"));
        #endif
        MifareClassic mifareClassic = MifareClassic(*shield);
        success = mifareClassic.write(ndefMessage, uid, uidLength);
    }
    else if (type == TAG_TYPE_2)
    {
        #ifdef NDEF_DEBUG
        Serial.println(F("Writing Mifare Ultralight"));
        #endif
        MifareUltralight mifareUltralight = MifareUltralight(*shield);
        success = mifareUltralight.write(ndefMessage, uid, uidLength);
    }
    else if (type == TAG_TYPE_UNKNOWN)
    {
        Serial.print(F("Can not determine tag type"));
        success = false;
    }
    else
    {
        Serial.print(F("No driver for card type "));Serial.println(type);
        success = false;
    }

    return success;
}

// TODO this should return a Driver MifareClassic, MifareUltralight, Type 4, Unknown
// Guess Tag Type by looking at the ATQA and SAK values
// Need to follow spec for Card Identification. Maybe AN1303, AN1305 and ???
unsigned int AnduinoNFC::guessTagType()
{

    // 4 byte id - Mifare Classic
    //  - ATQA 0x4 && SAK 0x8
    // 7 byte id
    //  - ATQA 0x44 && SAK 0x8 - Mifare Classic
    //  - ATQA 0x44 && SAK 0x0 - Mifare Ultralight NFC Forum Type 2
    //  - ATQA 0x344 && SAK 0x20 - NFC Forum Type 4

    if (uidLength == 4)
    {
        return TAG_TYPE_MIFARE_CLASSIC;
    }
    else
    {
        return TAG_TYPE_2;
    }
}

/* Checks PN532 IRQ interrupt and whether or not an NFC tag is present
before returning true that a tag/packet is ready to be processed, otherwise
it returns false*/
bool AnduinoNFC::packetReady()
{

    #ifdef __SAM3X8E__ 

    if((_lastReceiveTime > 0) && (millis() - _lastReceiveTime) < 2000)
    {
        return false;
    }
    else if(_isrTaken && tagPresent())
    {
        _lastReceiveTime = millis();        
        return true;
    } 
    else
    {
        //shouldn't get here
        return false;
    }

    #else //DIO2 external interrupt not supported on __SAMD21G18A__

    if((_lastReceiveTime > 0) && (millis() - _lastReceiveTime) < 2000)
    {
        return false;
    }
    else if(_isrTaken && tagPresent())
    {
        _lastReceiveTime = millis();        
        return true;
    } 
    else
    {
        //shouldn't get here
        return false;
    }

    #endif

         
}
