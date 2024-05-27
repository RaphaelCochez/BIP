/*
 - Author: Raphael Cochez
 - Date: 21/05/24
 - Version: V0.1
 - License: Just use it lol

 Super simple webserver with basic handling and routing
*/

#include <ESP8266WiFi.h>
#include <LittleFS.h> // Include the LittleFS library
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

const char* ssid = "BIPPUPPYFEEDER"; // Set your desired AP SSID
const char* password = "123456789"; // Set your desired AP password

AsyncWebServer server(80); // Create an instance of AsyncWebServer




void handleRoot(AsyncWebServerRequest* request) {
    // Read the HTML file from LittleFS
    File htmlFile = LittleFS.open("/index.html", "r");
    if (htmlFile) {
        request->send(htmlFile, "text/html");
        htmlFile.close();
    } else {
        // Custom response when "index.html" is not found
        String customResponse = "<html><body><h1>LittleFS not found</h1></body></html>";
        request->send(200, "text/html", customResponse);
    }
}

void handleNotFound(AsyncWebServerRequest* request) {
    // Redirect all requests to the root ("/") to your desired webpage
    request->redirect("/");
}

void initWiFi(){
    // Set wifi mode to be an acces point

    WiFi.mode(WIFI_AP);
    // Create an access point
    WiFi.softAP(ssid, password);
    IPAddress apIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(apIP);
}

void initRoutes(){
    server.on("/", HTTP_GET, handleRoot); // Handle requests to the root ("/") route
    server.onNotFound(handleNotFound); // Handle requests to unknown routes
}

void printConfigJSON(){
  File file = LittleFS.open("/config.json", "r");
  if (!file) {
    Serial.println("Error opening json config file for reading");
    return;
  }
  Serial.println(file.readString());
  file.close();  
}

void readConfigJSON(){
  File file = LittleFS.open("/config.json", "r");
  // Check if file exists
  if (!file) {
    Serial.println("Error opening json config file for reading");
    return;
  }
  else{
    Serial.println("Succesfully opening json config file for reading");
    return;
  }
  file.close();  
}

void readNetworkCredentials(){
  File file = LittleFS.open("/config.json", "r");
  // set JSON file size
  StaticJsonDocument<300> doc;
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, file);
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  // Print the contents of the json file
  Serial.println(file.readString());

  // Fetch values
  const char* NETWORKSSID = doc["NETWORKSSID"];
  const char* NETWORKPASPORT = doc["NETWORKPASPORT"];
  const int min_portions = doc["min_portions"];
  const int max_portions = doc["max_portions"];
  const int rotations_per_portion = doc["rotations_per_portion"];
  const int steps_per_portion = doc["steps_per_portion"];
  
  // Print the contents of the json file
  Serial.println();

  Serial.println(NETWORKSSID);
  Serial.println(NETWORKPASPORT);
  file.close(); 
 }
void readParameters(){
  File file = LittleFS.open("/config.json", "r");
  // set JSON file size
  StaticJsonDocument<300> doc;
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, file);
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  // Fetch values
  const int min_portions = doc["min_portions"];
  const int max_portions = doc["max_portions"];
  const int rotations_per_portion = doc["rotations_per_portion"];
  const int steps_per_portion = doc["steps_per_portion"];
  
  // Print the contents of the json file
  Serial.println();
  Serial.println(min_portions);
  Serial.println(max_portions);
  Serial.println(rotations_per_portion);
  Serial.println(steps_per_portion);
  file.close(); 

 }

void writeConfigJSON(){
File file = LittleFS.open("/config.json", "w");
 
  if (!file) {
    Serial.println("Error opening json config file for writing");
    return;
  }
 
  int bytesWritten = file.print("TEST SPIFFS");
 
  if (bytesWritten == 0) {
    Serial.println("File write failed");
    return;
  }
  file.close();  
}



void setup() {
    Serial.begin(9600);

    // Initialize LittleFS
    LittleFS.begin();
    initWiFi();
    initRoutes();
    readConfigJSON();
    server.begin();
    Serial.println();
    Serial.println("Web server started in AP mode");
}

void loop() {
   
}