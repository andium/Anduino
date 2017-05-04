#include <WiFi101.h> 
#include "Adafruit_MQTT.h" 
#include "Adafruit_MQTT_Client.h" 
#include "AnduinoLCD.h" 

// WiFi parameters 
#define WLAN_SSID       "Andium" 
#define WLAN_PASS       "maybeitsjustpassword" 

// Adafruit IO 
#define AIO_SERVER      "io.adafruit.com" 
#define AIO_SERVERPORT  1883 
#define AIO_USERNAME    "yourusernname" 
#define AIO_KEY         "dosifje0j230fj32lksdfj230ff23" 

WiFiClient client; 

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY); 

/****************************** Feeds ***************************************/ 
// Setup feed for temperature 
Adafruit_MQTT_Publish humidityVal = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/feeds/officePlants"); 

/*Create an instance of the AnduinoLCD */ 
AnduinoLCD LCD = AnduinoLCD(ST7735_CS_PIN, ST7735_DC_PIN, ST7735_RST_PIN); 

int inputPin = A1; 
const int dryVal = 807; 
const int wetVal = 415; 
int humidity = 0;  
int humidityPrev = 0; 
int relHumidity = 0; 

void setup()  
{   
 Serial.begin(115200); 
 delay(3000); 
 //Connect to WiFi & Adafruit.IO 
 connectToWiFi(); 
 connectToAdafruit(); 
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

   humidity = readHumidity(); 

   //convert int temp to char array 
   char b[4]; 
   String str; 
   str=String(humidity); 
   for(int i=0; i<str.length(); i++) 
   { 
     b[i]=str.charAt(i); 
   } 
   b[(str.length())+1]=0; 

 // Publish data 
 if (!humidityVal.publish((char*)b)) 
 { 
   Serial.println(F("Failed to publish temp")); 
 } else { 
   Serial.print(F("Humidity published: ")); 
   Serial.println(humidity); 
   displayHumidity(humidity, humidityPrev); 
 } 
 
 Serial.print("Humidity: "); 
 Serial.print(humidity); 
  
 //prev temp stored for LCD 
 
 humidityPrev = humidity; 
 //repeat every 10min 
 delay(600000); 
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
   delay(1000); 
 } 
 Serial.println(F("Adafruit IO Connected!")); 
} 

void displayHumidity(int humidity, int humidtyPrev) 
{ 
   //clear the stale value 
   LCD.setTextColor(ST7735_BLACK);  
   LCD.setTextSize(2); 
   LCD.setTextWrap(true); 
   LCD.setCursor(40,60); 
   LCD.setTextSize(5); 
   LCD.print(humidityPrev); 
   LCD.setTextSize(2); 
   LCD.print("%");  
   //Print new value 
   LCD.setTextColor(ST7735_WHITE);  
   LCD.setTextSize(2); 
   LCD.setTextWrap(true); 
   LCD.setCursor(40,60); 
   LCD.setTextSize(5); 
   LCD.print(humidity); 
   LCD.setTextSize(2); 
    LCD.print("%");  
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
 LCD.print("Humidity: "); 
} 

int readHumidity() { 
 int sensorVal = analogRead(inputPin); 
      relHumidity = map(sensorVal, dryVal, wetVal, 0, 100); 
      Serial.println("relHumidity"); 
      Serial.print(relHumidity); 
return relHumidity; 
} 