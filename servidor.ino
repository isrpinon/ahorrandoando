 /**
 * IBM IoT Foundation managed Device
 * Author: Alejandro Pizano
 * 
 **/ 
  
// Included libraries:
#include <WiFi.h>
#include <PubSubClient.h>         
#include <SPI.h>     

// Wifi configuration
const char* ssid = "mxhacks_esquina3";
const char* password = "5805hack";

// IBM IoT settings. Get them from Bluemix
#define ORG "maxaow"
#define DEVICE_TYPE "esp32"
#define DEVICE_ID "esp32 "
#define TOKEN "-yYXg1M8pRj(htAoEm"
int led = 2;

// Static configuration
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";   // Bluemix service server
char authMethod[] = "use-token-auth";                             // Auth by token
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;          
void callback(char* topic, byte* payload, unsigned int length);  

WiFiClient wifiClient;
PubSubClient client(server, 1883, callback, wifiClient);

int publishInterval = 30000; // This is the publish interval in milliseconds
long lastPublishMillis; 
// Channels
const char publishTopic[] = "iot-2/evt/status/fmt/json";

void setup() {
 Serial.begin(115200); Serial.println();    // Start the serial communication with pc, if running headless we can remove all the serial commands.
 pinMode(led, OUTPUT);              // We setup the LED as an output pinMode(integer,OUTPUT). LED_BUILTIN is a builtin function that returns the pin number where internal LED is connected.
 wifiConnect();                             // Start Wifi
 mqttConnect();                             // Connect to IBM IoT server

}

void loop() {
 digitalWrite(led,HIGH);
 if (millis() - lastPublishMillis > publishInterval) {                 // this routine publish the data every publish interval
  publishData(); 
  lastPublishMillis = millis();
 }
 if (!client.loop()) {                                                //    If we were disconnected from server, Connect again
  mqttConnect();
 }

}


// Subtroutine for publishing
void publishData() {
 String payload = "{\"data\":{\"Temperature\":";
 payload += 80 + random(3);
 payload += "}}";
 
 Serial.print("Sending payload: "); Serial.println(payload);
 digitalWrite(led,LOW);
 
 if (client.publish(publishTopic, (char*) payload.c_str())) {
 Serial.println("Publish OK");
  } else {
 Serial.println("Publish FAILED");
 }
}


// Connect to wifi. 
void wifiConnect() {
 Serial.print("Connecting to "); Serial.print(ssid); // Print the name of the SSID we are connecting to
 WiFi.begin(ssid, password);                         // Start coneection
 while (WiFi.status() != WL_CONNECTED) {             // Wait until we are connected
  delay(500);
  Serial.print(".");                                 
 } 
 Serial.print("\nWiFi connected, IP address: "); Serial.println(WiFi.localIP()); // Print the assigned ip
}

// Connect to IoT service
void mqttConnect() {
 if (!!!client.connected()) {
  Serial.print("Reconnecting MQTT client to "); Serial.println(server);
  while (!!!client.connect(clientId, authMethod, token)) {
    Serial.print(".");
    delay(500);
  }
 Serial.println();
 }
}


// Subroutine that runs when a MQQT message arrive to our device

void callback(char* topic, byte* payload, unsigned int payloadLength) {
 Serial.print("callback invoked for topic: "); Serial.println(topic);     //Print the topic we received
  // nothing to see here (yet) move along :)
   payload[payloadLength] = '\0';               // Add a line ending to the payload. Needed to parse correctly.
   String payload1 = String((char *)payload);   // Convert the payload to a string.  As it is received as char. 
   Serial.print("Payload is: ");
   Serial.println(payload1);                    // print the payload for debugging 
}
