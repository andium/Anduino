#include <WiFi101.h> 
#include "Adafruit_MQTT.h" 
#include "Adafruit_MQTT_Client.h" 
#include "DHT.h" 
#include "AnduinoLCD.h" 

// WiFi parameters 
#define WLAN_SSID       "YOUR_SSID" 
#define WLAN_PASS       "YOUR_PASSWD" 

// Adafruit IO 
#define AIO_SERVER      "io.adafruit.com" 
#define AIO_SERVERPORT  1883 
#define AIO_USERNAME    "YOUR_AIO_USERNAME" 
#define AIO_KEY         "YOUR_AIO_KEY" 
#define DHTPIN 53       // what digital pin we're connected to 
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321 

DHT dht(DHTPIN, DHTTYPE); 
WiFiClient client; 

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY); 
/****************************** Feeds ***************************************/ 

// Setup feed for temperature 
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/feeds/anduinoTemp"); 

/*Create an instance of the AnduinoLCD */ 

AnduinoLCD LCD = AnduinoLCD(ST7735_CS_PIN, ST7735_DC_PIN, PERIPH_RST_PIN); 

static int temp = 0; 
static int tempPrev = 0; 

void setup() { 
 Serial.begin(115200); 
 delay(3000); 

 //Connect to WiFi & Adafruit.IO 
 connectToWiFi(); 
 connectToAdafruit(); 

 //connect to DHT22 Temp Sensor 
 dht.begin(); 

 //Initialize LCD 
 LCD.begin(); 
 LCDinit(); 
} 

void loop() { 
   // ping adafruit io a few times to make sure we remain connected 
 if(! mqtt.ping(3)) { 
   // reconnect to adafruit io 
   if(! mqtt.connected()) 
     connect(); 
 } 

   // Grab the current state of the sensor 
   temp = dht.readTemperature(true); 

   //convert int temp to char array 
   char b[4]; 
   String str; 
   str=String(temp); 

   for(int i=0; i<str.length(); i++) 
   { 
     b[i]=str.charAt(i); 
   } 

   b[(str.length())+1]=0; 

 // Publish data 

 if (!temperature.publish((char*)b)) { 
   Serial.println(F("Failed to publish temp")); 
 } else { 
   Serial.print(F("Temp published: ")); 
   Serial.println(temp); 
   displayTemp(temp, tempPrev); 
 } 

 //float h = dht.readHumidity(); 
 // Check if any reads failed and exit early (to try again). 
 if (isnan(temp)) { 
   Serial.println("Failed to read from DHT sensor!"); 
   return; 
 } 

 //Serial.print("Humidity: "); 
 //Serial.print(h); 
 //Serial.print(" %\t"); 
 Serial.print("Temperature: "); 
 Serial.print(temp); 
 Serial.print(" *F\t"); 

 //prev temp stored for LCD 
 tempPrev = temp; 

 //repeat every 1min 
 delay(60000); 

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

void displayTemp(int temp, int tempPrev) 
{ 
   //clear the stale value 
   LCD.setTextColor(ST7735_BLACK);  
   LCD.setTextSize(2); 
   LCD.setTextWrap(true); 
   LCD.setCursor(40,60); 
   LCD.setTextSize(5); 
   LCD.print(tempPrev); 
   LCD.setTextSize(1); 
   LCD.print("F");  
   //Print new value 
   LCD.setTextColor(ST7735_WHITE);  
   LCD.setTextSize(2); 
   LCD.setTextWrap(true); 
   LCD.setCursor(40,60); 
   LCD.setTextSize(5); 
   LCD.print(temp); 
   LCD.setTextSize(1); 
   LCD.print("F");  
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
 LCD.print("Temperature: "); 
} 