 /******************************************************************* 
  Example sketch utilizing AnduinoWiFi library to create an end user
  WiFi registration process. This sketch creates a local SSID, serves 
  a webpage to recieve WiFi credentials, and then connects the Anduino
  to the specified WiFi.
  
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

#include "AnduinoWiFi.h"

AnduinoWiFi newWifi = AnduinoWiFi();//create an AnduinoWiFi instance
char testSite[] = "andium.com";     //we'll connect and GET / to test our WiFi connection

WiFiClient webCli;  //used for connecting to testSite


void setup() {
  //open serial comm for debugging--------------
 Serial.begin(115200);
  delay(3000);
  //--------------------------------------------

  //--------------------------------------------
  //create temp network Andium Demo for WiFi Reg
  newWifi.begin("Anduino WiFi");
  WiFiClient regClient;

  //While we're in AP mode serve-------------- 
  //reg webpage and parse POST response-------
   while(newWifi.isAPmode())
  {
    regClient = newWifi.getClient();

    //if we receive a client request to the server
    if(regClient)
    {
      newWifi.parseRequest(regClient);
    }  

  }
  //--------------------------------------------
 
  //WiFi Connected------------------------------
  Serial.println("WiFi connected");
  //--------------------------------------------

  //Test Wifi by connecting to www.andium.com---
  //--------------------------------------------
  Serial.println("\nStarting connection to server...");

  if (webCli.connect(testSite, 80)) {
    Serial.println("connected to server");
    // Make an HTTP request:
    webCli.println("GET / HTTP/1.1");
    webCli.println("Host: andium.com");
    webCli.println("Connection: close");
    webCli.println();
  }
  
     
}

void loop()
{
   // if there are incoming bytes available
  // from the server, read them and print them:
  while (webCli.available()) {
    char c = webCli.read();
    Serial.write(c);
  }

  //when the server has disconnected, stop the client:
  if (!webCli.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    webCli.stop();

    //we're done!
    while (true);
  }

}




