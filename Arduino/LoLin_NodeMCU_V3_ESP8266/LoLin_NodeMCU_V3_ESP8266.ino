//----------------------------------------Include the NodeMCU ESP8266 Library
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
//----------------------------------------

#include "DHT.h"  //--> Include the DHT Sensor Library 
#define DHTTYPE DHT11 //--> Defines the type of DHT sensor used (DHT11, DHT21, and DHT22), in this project the sensor used is DHT11.

#include "PageIndex.h" //--> Include the contents of the User Interface Web page, stored in the same folder as the .ino file

#define LEDonBoard 2  //--> Defining an On Board LED, used for indicators when the process of connecting to a wifi router

//----------------------------------------SSID and Password of your WiFi router
const char* ssid = "ABB_Gym_IOT";
const char* password = "Welcome2abb";
//----------------------------------------

ESP8266WebServer server(80);  //--> Server on port 80

const int DHTPin = 4; //--> The pin used for the DHT11 sensor is Pin D1=Pin 5
DHT dht(DHTPin, DHTTYPE); //--> Initialize DHT sensor, DHT dht(Pin_used, Type_of_DHT_Sensor);

//----------------------------------------This routine is executed when you open NodeMCU ESP8266 IP Address in browser
void handleRoot() {
 String s = MAIN_page; //--> Read HTML contents
 server.send(200, "text/html", s); //--> Send web page
}
//----------------------------------------
//----------------------------------------Procedure for reading the temperature value of a DHT11 sensor
void handleDHT11Temperature() {
  float t = dht.readTemperature();  //--> Read temperature as Celsius (the default). If you want to read the temperature sensor in Fahrenheit, use this -> float t = dht.readTemperature (true);
  String Temperature_Value = String(t);
 
  server.send(200, "text/plane", Temperature_Value); //--> Send Temperature value only to client ajax request
  
  if (isnan(t)) { //--> Check if any reads failed and exit early (to try again).  
    Serial.println("Failed to read from DHT sensor!");
  }
  else {
    Serial.print("DHT11 || Temperature : ");
    Serial.print(t);
    Serial.print(" || ");
  }
}
//----------------------------------------
//----------------------------------------Procedure for reading humidity values from DHT11 sensors
void handleDHT11Humidity() {
  float h = dht.readHumidity();
  String Humidity_Value = String(h);
 
  server.send(200, "text/plane", Humidity_Value); //--> Send Humidity value only to client ajax request
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor !");
  }
  else {
    Serial.print("Humidity : ");
    Serial.println(h);
  }
}
//----------------------------------------
//----------------------------------------Setup
void setup(void){
  Serial.begin(115200);
  delay(500);
  dht.begin();  //--> Start reading DHT11 sensors
  delay(500);
  
  WiFi.begin(ssid, password); //--> Connect to your WiFi router
  Serial.println("");
    
  pinMode(LEDonBoard,OUTPUT); //--> On Board LED port Direction output
  digitalWrite(LEDonBoard, HIGH); //--> Turn off Led On Board
  
  //----------------------------------------Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    //----------------------------------------Make the On Board Flashing LED on the process of connecting to the wifi router.
    digitalWrite(LEDonBoard, LOW);
    delay(250);
    digitalWrite(LEDonBoard, HIGH);
    delay(250);
    //----------------------------------------
  }
  //----------------------------------------
  digitalWrite(LEDonBoard, HIGH); //--> Turn off the On Board LED when it is connected to the wifi router.
  //----------------------------------------If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //----------------------------------------
  server.on("/", handleRoot); //--> Routine to handle at root location. This is to display web page.
  server.on("/readTemperature", handleDHT11Temperature);  //--> Routine to handle the call procedure handleDHT11Temperature
  server.on("/readHumidity", handleDHT11Humidity);  //--> Routine to handle the call procedure handleDHT11Humidity

  server.begin(); //--> Start server
  Serial.println("HTTP server started");
}
//----------------------------------------
//----------------------------------------Loop
void loop(){
  server.handleClient();  //Handle client requests
}
//----------------------------------------
