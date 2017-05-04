#include "AnduinoWiFi.h"


int APstatus = WL_IDLE_STATUS;

WiFiServer APserver(80);
WiFiClient AnduinoWiFiclient;


String SSIDval, password, eof = "";
bool mode = true;
char SSIDvalchar[40] = "";
char passwordchar[40] = "";


AnduinoWiFi::AnduinoWiFi()
{

}

int AnduinoWiFi::begin(char APssid[])
{
    //create WiFi AP
    APstatus = WiFi.beginAP(APssid);
    if (APstatus != WL_AP_LISTENING) {
      Serial.println("Creating access point failed");
      // don't continue
      while (true);
    }

    // wait a few seconds:
    delay(3000);

    // start the web server on port 80
    APserver.begin();

    // you're connected now, so print out the status
    printAPStatus();
    return APstatus;
}

bool AnduinoWiFi::disconnectAP()
{
  //disconnect from AP mode
    WiFi.disconnect();
    WiFi.end(); 
    delay(200);

  return false;
}

uint8_t AnduinoWiFi::connectToWiFi(char* newSsid, char* newPass)
{
   
    // attempt to connect to Wifi network:
    while (WiFi.status() != WL_CONNECTED)
    {
    Serial.println();  
    Serial.println();
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(newSsid);
    Serial.print("With password: ");
    Serial.println(newPass);
 
    APstatus = WiFi.begin(newSsid, newPass);

    //Don't try again too soon
    delay(3000);
    }

    printWiFiStatus();

    return WiFi.status();
  
}

void AnduinoWiFi::printAPStatus() 
{
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    IPAddress ip = WiFi.localIP();
    Serial.print("Device IP Address: ");
    Serial.println(ip);

    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
    Serial.print("Registration server Running at http://");
    Serial.print(ip);
    Serial.println("/register");
}

void AnduinoWiFi::printWiFiStatus() 
{
    Serial.println("Success! Connected to...");
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    IPAddress ip = WiFi.localIP();
    Serial.print("Device IP Address: ");
    Serial.println(ip);

    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}

void AnduinoWiFi::getRegistrationPage(WiFiClient client)
{
    //Header
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();

    //Body
    client.print("<style>body {background-color: #00cbf5; color: white;}</style>");
    client.print("<pre>"); 
    client.print("                         `-`                                                                                              ");
    client.print("<br>");   
    client.print("       `-://++//:-`    `:+++:`                                                  :/:  :/:                                  ");      
    client.print("<br>");   
    client.print("     -/++++++++++++/-`:+++++++:`                                                /++  :/:                                  ");  
    client.print("<br>");   
    client.print("   `/++++++++++++++/.`/+++++++++:`             `.-::-.`     `.-:::-`     `.-::-./++  ...  ...     ...   .-:::-` .-:::-`   ");   
    client.print("<br>");
    client.print("  `/+++++++++++++/.    /++++++++++:.         `:++/::/++:   -++/::/++/`  :++/::/++++  +++  +++     ++/  /++/::++/+/::/++:  ");   
    client.print("<br>");
    client.print("  -++++++++++++:.      .++++++++++++:.       :++-    .++:  +++`   .++- :++-    .+++  +++  +++     ++/ .++:   `+++`   /++  ");   
    client.print("<br>");
    client.print("  :++++++++++++.       .+++++++++++++:       /++`    `++/  ++/    `++: :++.     ++/  +++  +++    `++/ .++-    ++/    :++` ");   
    client.print("<br>");
    client.print("  .+++++++++++++/.     :+++++++++++:`        .++/-..-/+++  ++/    `++: `/++-..-/++.  +++  -++:.../++- .++-    ++/    :++` ");   
    client.print("<br>");
    client.print("   -++++++++++++++/.  .++++++++++:`           `-//+//://:  //:    `//-  `-://///-`   ://   .://+//:.  .//.    //:    -//  ");   
    client.print("<br>");
    client.print("   ./++++++++++++++:.+++++++++:`                 ``                        ``                ``                           ");  
    client.print("<br>");
    client.print("      .:++++++++++:.` `:+++++:`                                                                                           ");   
    client.print("<br>");
    client.print("         `..--..`       `:+:`                                                                                             ");
    client.print("<br>");
    client.print("</pre>");
    client.print("<div style=\"display: inline-block; text-align: center; color: white; align: center; width: 100%; font-family: Lucida Sans Unicode\">");
    client.print("<h3>Please enter your WiFi Credentials!</h3>");
    client.print("<form method=\"POST\" action=\"/enterCredentials\">Enter SSID:<br><input type=\"text\" name=\"SSID\"><br>Password:<br><input type=\"password\" name=\"password\" defaultvalue=\"blank\"><br><input type=\"hidden\" name=\"eof\" value=\"#EOF\"><br><input type=\"submit\" value=\"Submit\"></form>");
    client.print("</div>");
    client.println();

    client.stop();
}

void AnduinoWiFi::postRegistrationPage(WiFiClient client)
{
    //Header
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();

    //Body
    client.print("<style>body {background-color: #00cbf5; color: white;}</style>");
    client.print("<pre>"); 
    client.print("                         `-`                                                                                              ");
    client.print("<br>");   
    client.print("       `-://++//:-`    `:+++:`                                                  :/:  :/:                                  ");      
    client.print("<br>");   
    client.print("     -/++++++++++++/-`:+++++++:`                                                /++  :/:                                  ");  
    client.print("<br>");   
    client.print("   `/++++++++++++++/.`/+++++++++:`             `.-::-.`     `.-:::-`     `.-::-./++  ...  ...     ...   .-:::-` .-:::-`   ");   
    client.print("<br>");
    client.print("  `/+++++++++++++/.    /++++++++++:.         `:++/::/++:   -++/::/++/`  :++/::/++++  +++  +++     ++/  /++/::++/+/::/++:  ");   
    client.print("<br>");
    client.print("  -++++++++++++:.      .++++++++++++:.       :++-    .++:  +++`   .++- :++-    .+++  +++  +++     ++/ .++:   `+++`   /++  ");   
    client.print("<br>");
    client.print("  :++++++++++++.       .+++++++++++++:       /++`    `++/  ++/    `++: :++.     ++/  +++  +++    `++/ .++-    ++/    :++` ");   
    client.print("<br>");
    client.print("  .+++++++++++++/.     :+++++++++++:`        .++/-..-/+++  ++/    `++: `/++-..-/++.  +++  -++:.../++- .++-    ++/    :++` ");   
    client.print("<br>");
    client.print("   -++++++++++++++/.  .++++++++++:`           `-//+//://:  //:    `//-  `-://///-`   ://   .://+//:.  .//.    //:    -//  ");   
    client.print("<br>");
    client.print("   ./++++++++++++++:.+++++++++:`                 ``                        ``                ``                           ");  
    client.print("<br>");
    client.print("      .:++++++++++:.` `:+++++:`                                                                                           ");   
    client.print("<br>");
    client.print("         `..--..`       `:+:`                                                                                             ");
    client.print("<br>");
    client.print("</pre>");
    client.print("<div style=\"display: inline-block; text-align: center; color: white; align: center; width: 100%; font-family: Lucida Sans Unicode\">");
    client.print("<h3>Thanks! Disconnecting from 'Andium Demo' and attempting to connect to ");
    client.print(SSIDval);
    client.print(" WiFi!</h3>");
    client.print("</div>");
    client.println();

    client.stop();

}

void AnduinoWiFi::parseRequest(WiFiClient client)
{
    String currentLine = "";
    bool getRegisterPage = false;
    bool getEnterCredentialsPage = false;
    bool endOfReq = false;
    char prevC = ' ';

      while (client.connected() && (!endOfReq)) 
      {            // loop while the client's connected
          if (client.available()) 
          {             // if there's bytes to read from the client,
                  char c = client.read();             // read a byte, then
                  Serial.write(c);                    // print it out the serial monitor
                  currentLine += c;
                  if (currentLine == "GET /register HTTP/1.1") 
                  {
                        getRegisterPage = true;
                  }
                  else if(currentLine == "POST /enterCredentials HTTP/1.1")
                  {
                        getEnterCredentialsPage = true;
                  }
                  else if (c == '\r' && prevC == '\n' && getRegisterPage)
                  {
                        endOfReq = true;
                  }
                  else if (getEnterCredentialsPage && currentLine.endsWith("%23EOF"))
                  {
                        endOfReq = true;
                  }
                  else if(currentLine == "GET /favicon.ico HTTP/1.1")
                  {
                       client.stop();
                  }
                  prevC = c;       
          }
      }
    

    if(getRegisterPage)
    {
        getRegistrationPage(client);
    }
    else if(getEnterCredentialsPage)
    {          
        recursiveBodyParser(currentLine.substring(currentLine.indexOf("SSID",0)));
        postRegistrationPage(client);

        mode = disconnectAP();
        
        SSIDval.toCharArray(SSIDvalchar, 40); 
        password.toCharArray(passwordchar, 40);   
        connectToWiFi(SSIDvalchar, passwordchar);  
    }

}

//ex input:SSID=WiFissid&wifipassword=hmmmm&eof=%23EOF

uint8_t AnduinoWiFi::recursiveBodyParser(String reqBody)
{

  String param = "";
  String value = "";
  bool parsed = false;
  int i = 0;
  
  if(!reqBody.startsWith("eof"))       //base case 
  {                                    //hidden field eof depicts end of input

      while(!parsed)
      {
        #ifdef DEBUG_WIFIREG
        Serial.print(reqBody.charAt(i));
        #endif

          if(reqBody.charAt(i) == '=')
          {
            param = reqBody.substring(0,i);
            
            reqBody.remove(0,i+1);          //remove key name up to =       
          }
          
          if(reqBody.charAt(i) == '&')
          {
            value = reqBody.substring(0,i); //temp store the value
            reqBody.remove(0,i+1);          //remove the saved val
            parsed = true;                  //break the loop
          }
          
          i++;
      }

      value = decodeUri(value);

      /*Store the correct value*/
      if(param == "SSID")
      {                   
        SSIDval = value;
      }
      else if(param == "password")
      { 
       
        if(value == "none")
        {
          password = " ";
        }
        password = value;

        #ifdef DEBUG_WIFIREG
        Serial.print("Passowrd: "); Serial.println(password);
        #endif
      }
      else if(param == "eof")
      {
        //should never get here
        eof = value;
          
        #ifdef DEBUG_WIFIREG
        Serial.print("eof: "); Serial.println(eof);
        #endif
      }
      else
      {
        Serial.println("ERROR form name not recognized!");
      }
      
      #ifdef DEBUG_WIFIREG
      Serial.println();
      #endif
      
      delay(1000);
      recursiveBodyParser(reqBody); //call the function agian with the shortened body
  }
  else
  {
    //We've reached eof entire form is parsed end recursive loop
    return 0;
  }
  
}

String AnduinoWiFi::decodeUri(String buff)
{
  //Decode popular special characters
  buff.replace("+", " ");
  
  buff.replace("%20", " "); 
  buff.replace("%21", "!");
  buff.replace("%22", "\"");
  buff.replace("%23", "#");
  buff.replace("%24", "$");
  buff.replace("%25", "%");
  buff.replace("%26", "&");
  buff.replace("%27", "`");
  buff.replace("%28", "(");
  buff.replace("%29", ")");
  buff.replace("%2A", "*");
  buff.replace("%2B", "+");
  buff.replace("%2C", ",");
  buff.replace("%2D", "-");
  buff.replace("%2E", ".");
  buff.replace("%2F", "/");
  buff.replace("%3A", ":");
  buff.replace("%3B", ";");
  buff.replace("%3C", "<");
  buff.replace("%3D", "=");
  buff.replace("%3E", ">");
  buff.replace("%3F", "?");
  buff.replace("%40", "@");
  buff.replace("%5B", "[");
  buff.replace("%5C", "\\");
  buff.replace("%5D", "]");
  buff.replace("%5E", "^");
  buff.replace("%5F", "_");
  buff.replace("%60", "`");
  buff.replace("%7B", "{");
  buff.replace("%7C", "|");
  buff.replace("%7D", "}");
  buff.replace("%7E", "~");
  buff.replace("%7F", " ");
  
  return buff;

}

WiFiClient AnduinoWiFi::getClient()
{
    return AnduinoWiFiclient = APserver.available();
}

String AnduinoWiFi::getSSID()
{
  return SSIDval;
}

String AnduinoWiFi::getPassword()
{
  return password;
}

bool AnduinoWiFi::isAPmode()
{
  return mode;
}

