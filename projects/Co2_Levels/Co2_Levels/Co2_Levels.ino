#include <WiFi101.h> 
#include "Adafruit_MQTT.h" 
#include "Adafruit_MQTT_Client.h" 
#include "AnduinoLCD.h" 

// WiFi parameters 
#define WLAN_SSID       "AndiumTraining" 
#define WLAN_PASS       "Curate!!" 

// Adafruit IO 
#define AIO_SERVER      "io.adafruit.com" 
#define AIO_SERVERPORT  1883 
#define AIO_USERNAME    "bcarbon1" 
#define AIO_KEY         "d5fbd1e7e6fb486db2df5c5e25bd2192" 

WiFiClient client; 
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY); 
/****************************** Feeds ***************************************/ 
// Setup feed for co2  
Adafruit_MQTT_Publish carbonDioxide = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/feeds/co2"); 
/*Create an instance of the AnduinoLCD */ 
AnduinoLCD LCD = AnduinoLCD(ST7735_CS_PIN, ST7735_DC_PIN, 13); 

static int co2 = 0; 
static int co2Prev = 0; 

#define CZR_STREAMING   0x01
#define CZR_POLLING     0x02

char buffer[20] = {0};

void setup() { 
 Serial.begin(115200); 
 delay(3000); 
 Serial1.begin(9600);

 //Connect to WiFi & Adafruit.IO 
 connectToWiFi(); 
 connectToAdafruit(); 
 
 //Initialize LCD 
 LCD.begin(); 
 LCDinit(); 

 //CO2 Calibration and initial setup 
 //setOperatingMode(CZR_STREAMING);
 //setOperatingMode(CZR_POLLING);
 //calibrateFreshAir();
 
} 
void loop() { 
   // ping adafruit io a few times to make sure we remain connected 
 if(! mqtt.ping(3)) { 
   // reconnect to adafruit io 
   if(! mqtt.connected()) 
     connect(); 
 } 
   // Grab the current co2 reading
   co2 = Request("Z");
   //convert int temp to char array 
   char b[20]; 
   String str; 
   str=String(co2); 
   for(int i=0; i<str.length(); i++) 
   { 
     b[i]=str.charAt(i); 
   } 
   b[(str.length())+1]=0; 
   
 // Publish data 
 if (!carbonDioxide.publish((char*)b)) { 
   Serial.println(F("Failed to publish co2")); 
 } else { 
   Serial.print(F("co2 published: ")); 
   Serial.println(co2); 
   displayCo2(co2, co2Prev); 
 } 
 
 Serial.print("CO2 : ");Serial.println(co2);
 Serial.println("");
 
 //prev val stored for LCD 
 co2Prev = co2; 
 //repeat every 1min 
 delay(1000); 
 
} 
// connect to adafruit io via MQTT 
void connect() { 
 Serial.print(F("Connecting to Adafruit IO... ")); 
 int8_t ret; 
 while ((ret = mqtt.connect()) != 0) { 
   switch (ret) { 
     case 1: Serial.println(F("Wrong protocol")); break; 
     case 2: Serial.println(F("ID rejected")); break; 
     case 3: Serial.println(F("Server unavail")); break; 
     case 4: Serial.println(F("Bad user/pass")); break; 
     case 5: Serial.println(F("Not authed")); break; 
     case 6: Serial.println(F("Failed to subscribe")); break; 
     default: Serial.println(F("Connection failed")); break; 
   } 
   if(ret >= 0) 
     mqtt.disconnect(); 
   Serial.println(F("Retrying connection...")); 
   delay(5000); 
 } 
 Serial.println(F("Adafruit IO Connected!")); 
} 
void displayCo2(int co2, int co2Prev) 
{ 
   //clear the stale value 
   LCD.setTextColor(ST7735_BLACK);  
   LCD.setTextSize(2); 
   LCD.setTextWrap(true); 
   LCD.setCursor(40,60); 
   LCD.setTextSize(3); 
   LCD.print(co2Prev); 
   LCD.setTextSize(1); 
   LCD.print("ppm");  
   //Print new value 
   LCD.setTextColor(ST7735_WHITE);  
   LCD.setTextSize(2); 
   LCD.setTextWrap(true); 
   LCD.setCursor(40,60); 
   LCD.setTextSize(3); 
   LCD.print(co2); 
   LCD.setTextSize(1); 
   LCD.print("ppm");  
 } 
void connectToWiFi() 
{ 
 // Connect to WiFi access point. 
 delay(10); 
 Serial.print(F("Connecting to ")); 
 Serial.println(WLAN_SSID); 
 WiFi.begin(WLAN_SSID, WLAN_PASS); 
 while (WiFi.status() != WL_CONNECTED) { 
   delay(500); 
   Serial.print(F(".")); 
 } 
 Serial.println(F("WiFi connected!")); 
} 
void connectToAdafruit() 
{ 
   // connect to adafruit io 
 connect(); 
} 
void LCDinit() 
{ 
 LCD.setBacklight(ON); 
 LCD.fillScreen(ST7735_BLACK);  //clear the screen    
 LCD.showBanner();              //load Andium Banner 
 LCD.setTextColor(ST7735_WHITE);  
 LCD.setTextSize(2); 
 LCD.setTextWrap(true); 
 LCD.setCursor(0,40); 
 LCD.print("CO2: "); 
} 

uint16_t Request(char* s)
{
  buffer[0] = '\0'; 
  int idx = 0;

  //send command request 'Z' for CO2
  Command(s);

  delay(250);  
 
  while(Serial1.available())
  {
    buffer[idx++] = Serial1.read();
  }
  buffer[idx] = '\0';
  uint16_t rv = 0;
  
  rv = atoi(&buffer[2]);
    
  return rv;
}

void Command(char* s)
{
  Serial1.print(s);
  Serial1.print("\r\n");
}

uint16_t calibrateFreshAir()
{
  return Request("G");
}

void setOperatingMode(uint8_t mode)
{
  sprintf(buffer, "K %u", mode);
  Command(buffer);
}
