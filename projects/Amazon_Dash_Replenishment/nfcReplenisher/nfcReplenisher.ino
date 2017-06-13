#include "AmazonDRS.h"
#include "AnduinoNFC.h"

AmazonDRS DRS = AmazonDRS();
AnduinoNFC NFC = AnduinoNFC();

//WiFi creds -----------------------------------------------------------------
char ssid[] = ""; //  your network SSID (name)
char pass[] = "";    // your network password (use for WPA, or use as key for WEP)
//----------------------------------------------------------------------------

void setup() {
 Serial.begin(115200);
 while (!Serial) {
     ; // wait for serial port to connect. Needed for native USB port only
  }

 //Setup NFC
 NFC.begin();

 //Start up DRS
 DRS.begin(ssid,pass);

 //initialize slots
 DRS.retrieveSubscriptionInfo();  //check slot statuses
}

void loop() {
 //scan nfc tag containing slotId
 //if scanned id matches a valid slot and the slot is available
 //request replenishment for the supplied slot

  if (NFC.packetReady())
   {
       NfcTag tag = NFC.read();  //attempt to read the RFID tag 
       tag.print();              //and print the results to the terminal

       NdefMessage message = tag.getNdefMessage();
       NdefRecord record = message.getRecord(0); //grab the bits that contain the DRS Slot ID

       int payloadLength = record.getPayloadLength();
       byte payloadBytes[payloadLength];
       record.getPayload(payloadBytes);
       String payloadString = "";  //store the RFID msg bits in a String for comparison
      
       for(int i=3; i<payloadLength; i++)
       {
         payloadString += (char)payloadBytes[i]; //load up the cmp string with payload less the encoding 
       }
      
       if(slotId0 == payloadString)    //eventually if(slotId[i] has a match and slotStatus[i] is available 
       {
           //we have a match! replenish the products associated with that slot!            
           DRS.requestReplenishmentForSlot(payloadString);
       }
       else
       {
         Serial.print("Sorry, slot ");
         Serial.print(payloadString);
         Serial.println(" is not available at this time");
       }
   }
}