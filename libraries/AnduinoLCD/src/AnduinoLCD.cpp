/******************************************************************* 
  This library extends Adafruit's ST7735 library for the Andium 
  Anduino Shield.
  
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


#include "AnduinoLCD.h"

extern gimpImage andiumBanner;
extern gimpImage andiumLogoFull;


AnduinoLCD::AnduinoLCD(uint8_t cs, uint8_t rs, uint8_t rst) : Adafruit_ST7735(cs, rs, rst)
{
}


void AnduinoLCD::begin(void)
{
    #ifdef DEBUG_AnduinoLCD
        Serial.print("Starting AnduinoLCD...");
    #endif

    /* Turn backlight off */
    pinMode(ST7735_BKLIT_PIN, OUTPUT);
    setBacklight(OFF);

    // initialize a ST7735S chip, black tab
    initR(INITR_BLACKTAB);   
    setRotation(1);
    fillScreen(ST7735_BLACK);


    #ifdef DEBUG_AnduinoLCD
        Serial.println("Ready!");
    #endif
}



Status AnduinoLCD::setBacklight(BACKLIGHT_STATE state)
{
    switch(state)
    {
        case ON:
            digitalWrite(ST7735_BKLIT_PIN, LOW);
            break;

        case OFF:
            digitalWrite(ST7735_BKLIT_PIN, HIGH);
            break;
    }

    return STATUS_OK;
}

Status AnduinoLCD::showGimpImage(gimpImage* image)
{
    /* format check*/
    if(image->bytes_per_pixel != 2)
    {
        #ifdef DEBUG_AnduinoLCD
            Serial.println("Gimp image format not supported (16-bit RGB)");
        #endif

        return STATUS_IMAGE_FORMAT_NOT_SUPPORTED;
    }

    uint16_t pixel, index, block;

    #ifdef DEBUG_AnduinoLCD
        uint16_t time = millis();
    #endif


    for(int y =0 ; y<image->height; y++)
    {
        block = y*image->width;
        for(int x=0; x<image->width; x++)
        {
            index = (block + x) *2;
            pixel = image->pixel_data[index];
            pixel |= image->pixel_data[index+1] << 8;

            drawPixel(x, y, pixel);
        }
    }


    #ifdef DEBUG_AnduinoLCD
        time = millis() - time;
        Serial.print("Draw GIMP image took: ");Serial.print(time, DEC);Serial.println("ms");
    #endif


    return STATUS_OK;
}

void AnduinoLCD::screenPrint(char* message, int x, int y)
{
  if(logoFullscreen)
  {
    showBanner();
  }
  
  setCursor(x, y);
  println(message);
}


void AnduinoLCD::showFullscreenLogo(void)
{
  showGimpImage(&andiumLogoFull);
  logoFullscreen = true;
}

void AnduinoLCD::showBanner(void)
{
  fillScreen(ST7735_BLACK);
  showGimpImage(&andiumBanner);
  
  setTextColor(ST7735_GREEN, ST7735_BLACK);
  setTextSize(1);

  logoFullscreen = false;
}

void AnduinoLCD::waveform(uint8_t analogPin, uint8_t px)
{
  int sensor = analogRead(analogPin);                     //analog input
  int drawHeight = map(sensor, 0, 1023, 0, height()-60);  //map range for sensor trend
  int xPos = px;                                          //pixel position counter

  //     (r ,  g,  b)
  stroke(250, 180, 10);
  line(xPos, height() - drawHeight, xPos, height()-drawHeight+5);

  //If the line has reached the end of the screen
  //refresh the screen less the banner
  if (xPos >= 160) {
    stroke(1,1,1);
    fill(1,1,1);
    rect(0,40,width(),100); //screen size less banner
  }

  delay(15);  //sample rate as a function of loop iterations
}

//prints digital analog pin reading to screen under banner
void AnduinoLCD::digitalGauge(uint8_t analogPin)
{
  char sensorPrintout[4];

  // Read the value of the sensor on analogPin
  String sensorVal = String(analogRead(analogPin));

  // convert the reading to a char array
  sensorVal.toCharArray(sensorPrintout, 4);

  stroke(255, 255, 255);
  text(sensorPrintout, 0, 60);
  delay(250); //refresh reate
  //refresh screen by blacking out previous reading
  stroke(0, 0, 0);
  text(sensorPrintout, 0, 60);
}


//brief splashSccreen example to be used on startup draws Andium logo
void AnduinoLCD::splashScreen(void){

  //draw diamond of logo
  fillTriangle( 68,  80,  110,  40,  152,  80,  0x063E);
  fillTriangle( 68,  80,  110,  120,  152,  80,  0x063E);
  delay(100);
  //draw growing circle of logo encompasing diamond
    for(int i = 0; i<40; i+=2){
        fillCircle(60,  80,  i,  0x063F);
        delay(25);
    }
  //fill triangle again and draw circle outline to create overlay
  fillTriangle( 68,  80,  110,  40,  152,  80,  0x063E);
  fillTriangle( 68,  80,  110,  120,  152,  80,  0x063E);
  drawCircle( 60, 80, 39, 0x063F); 
  delay(100);
  //drop in the banner
  showGimpImage(&andiumBanner);
  delay(500); //stare at it for half a second
}



