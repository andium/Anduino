/*******************************************************************
 Check out the included Arduino sketch examples and Anduino Wiki to
 get started!
 https://github.com/andium/Anduino/wiki
 
 This is a modified version of PN532_SPI only renamed to reinforce
 that the PN532_SPI interface has been hardcoded for the Andium
 Anduino shield.
 
 Modified by Brian Carbonette Copyright © 2016 Andium
 Original source contributed by xiongyihui as well as SeedStudio 
 and Adafruit can be found here https://github.com/Seeed-Studio/PN532 
 
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
 
/*Anduino PN532_SPI*/
#include "AnduinoInterface.h"
#include "PN532/PN532_debug.h"
#include "Arduino.h"

#define STATUS_READ     2
#define DATA_WRITE      1
#define DATA_READ       3



AnduinoInterface::AnduinoInterface(SPIClass &spi, uint8_t ss)
{
    command = 0;
    _spi = &spi;
    _ss  = ss;

}

void AnduinoInterface::begin()
{
    pinMode(_ss, OUTPUT);

    _spi->begin();

    _spi->setDataMode(SPI_MODE0);  // PN532 only supports mode0
    _spi->setBitOrder(LSBFIRST);

#if defined __SAM3X8E__
    /** DUE spi library does not support SPI_CLOCK_DIV8 macro */
    _spi->setClockDivider(6, 21);             // set clock 2MHz(max: 5MHz)
#elif defined __SAMD21G18A__
    /** M0 spi library does not support SPI_CLOCK_DIV8 macro */
    _spi->setClockDivider(16);             // set clock 2MHz(max: 5MHz)
#else 
    _spi->setClockDivider(SPI_CLOCK_DIV8); // set clock 2MHz(max: 5MHz)
#endif

}

void AnduinoInterface::wakeup()
{
    digitalWrite(_ss, LOW);
    delay(2);
    digitalWrite(_ss, HIGH);
}

int8_t AnduinoInterface::writeCommand(const uint8_t *header, uint8_t hlen, const uint8_t *body, uint8_t blen)
{
    command = header[0];
    writeFrame(header, hlen, body, blen);
    
    uint8_t timeout = PN532_ACK_WAIT_TIME;
    while (!isReady()) {
        delay(1);
        timeout--;
        if (0 == timeout) {
            DMSG("Time out when waiting for ACK\n");
            return -2;
        }
    }
    if (readAckFrame()) {
        DMSG("Invalid ACK\n");
        return PN532_INVALID_ACK;
    }
    return 0;
}

int16_t AnduinoInterface::readResponse(uint8_t buf[], uint8_t len, uint16_t timeout)
{
    uint16_t time = 0;
    while (!isReady()) {
        delay(1);
        time++;
        if (timeout > 0 && time > timeout) {
            return PN532_TIMEOUT;
        }
    }

    digitalWrite(_ss, LOW);
    delay(1);

    int16_t result;
    do {
        write(DATA_READ);

        if (0x00 != read()      ||       // PREAMBLE
                0x00 != read()  ||       // STARTCODE1
                0xFF != read()           // STARTCODE2
           ) {

            result = PN532_INVALID_FRAME;
            break;
        }

        uint8_t length = read();
        if (0 != (uint8_t)(length + read())) {   // checksum of length
            result = PN532_INVALID_FRAME;
            break;
        }

        uint8_t cmd = command + 1;               // response command
        if (PN532_PN532TOHOST != read() || (cmd) != read()) {
            result = PN532_INVALID_FRAME;
            break;
        }

        DMSG("read:  ");
        DMSG_HEX(cmd);

        length -= 2;
        if (length > len) {
            for (uint8_t i = 0; i < length; i++) {
                DMSG_HEX(read());                 // dump message
            }
            DMSG("\nNot enough space\n");
            read();
            read();
            result = PN532_NO_SPACE;  // not enough space
            break;
        }

        uint8_t sum = PN532_PN532TOHOST + cmd;
        for (uint8_t i = 0; i < length; i++) {
            buf[i] = read();
            sum += buf[i];

            DMSG_HEX(buf[i]);
        }
        DMSG('\n');

        uint8_t checksum = read();
        if (0 != (uint8_t)(sum + checksum)) {
            DMSG("checksum is not ok\n");
            result = PN532_INVALID_FRAME;
            break;
        }
        read();         // POSTAMBLE

        result = length;
    } while (0);

    digitalWrite(_ss, HIGH);

    return result;
}

boolean AnduinoInterface::isReady()
{
    digitalWrite(_ss, LOW);

    write(STATUS_READ);
    uint8_t status = read() & 1;
    digitalWrite(_ss, HIGH);
    return status;
}

void AnduinoInterface::writeFrame(const uint8_t *header, uint8_t hlen, const uint8_t *body, uint8_t blen)
{
    digitalWrite(_ss, LOW);
    delay(2);               // wake up PN532

    write(DATA_WRITE);
    write(PN532_PREAMBLE);
    write(PN532_STARTCODE1);
    write(PN532_STARTCODE2);

    uint8_t length = hlen + blen + 1;   // length of data field: TFI + DATA
    write(length);
    write(~length + 1);         // checksum of length

    write(PN532_HOSTTOPN532);
    uint8_t sum = PN532_HOSTTOPN532;    // sum of TFI + DATA

    DMSG("write: ");

    for (uint8_t i = 0; i < hlen; i++) {
        write(header[i]);
        sum += header[i];

        DMSG_HEX(header[i]);
    }
    for (uint8_t i = 0; i < blen; i++) {
        write(body[i]);
        sum += body[i];

        DMSG_HEX(body[i]);
    }

    uint8_t checksum = ~sum + 1;        // checksum of TFI + DATA
    write(checksum);
    write(PN532_POSTAMBLE);

    digitalWrite(_ss, HIGH);

    DMSG('\n');
}

int8_t AnduinoInterface::readAckFrame()
{
    const uint8_t PN532_ACK[] = {0, 0, 0xFF, 0, 0xFF, 0};

    uint8_t ackBuf[sizeof(PN532_ACK)];

    digitalWrite(_ss, LOW);
    delay(1);
    write(DATA_READ);

    for (uint8_t i = 0; i < sizeof(PN532_ACK); i++) {
        ackBuf[i] = read();
    }

    digitalWrite(_ss, HIGH);

    return memcmp(ackBuf, PN532_ACK, sizeof(PN532_ACK));
}





