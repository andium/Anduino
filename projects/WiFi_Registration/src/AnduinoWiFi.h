#ifndef __AnduinoWiFi_H
#define __AnduinoWiFi_H

#include "Arduino.h"
#include <WiFi101.h>

/*#define DEBUG_WIFIREG*/


class AnduinoWiFi
{
	
	public:
		AnduinoWiFi();
		int begin(char ssid[]);
	    void printDetails();
	    bool disconnectAP();
	    bool isAPmode();
	    String getSSID();
	    String getPassword();
	    void parseRequest(WiFiClient client);
	    WiFiClient getClient();



	private:
	    void getRegistrationPage(WiFiClient client);
	    void postRegistrationPage(WiFiClient client);
	    uint8_t connectToWiFi(char* newSsid, char* newPass);
	    uint8_t recursiveBodyParser(String reqBody);    
	    String decodeUri(String buff);
	    void printAPStatus();
	    void printWiFiStatus();
		
};

#endif /*__AnduinoWiFi_H */
