/*
 - Author: Raphael Cochez
 - Date: 21/05/24
 - Version: V0.2
 - License: Just use it lol
  
  upload index.html and about.html to LittleFS filesystem

  http://<your_ESP8266_IP>/index.html (for the root page)
  http://<your_ESP8266_IP>/about.html (for the about page)

  to add
  Logic to handle form input of

  Simple form
  5 radio-buttons
  1 text-box 
  1 submit button

  on submit use code to let stepper do X 

  add logic to do something with this function
  step(false, X_DIR, X_STP, 200); // x axis motor rotates CCW for 1 circle, as in 200 steps 

  Libaries + docs
  Code for the ESP12E or ESP12F
  https://github.com/lorol/LITTLEFS
  https://github.com/me-no-dev/ESPAsyncWebServer?tab=readme-ov-file
  https://github.com/me-no-dev/ESPAsyncTCP


*/


#include <ESP8266WiFi.h>
#include <LittleFS.h> // Include the LittleFS library
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino.h>

#define EN        8       // stepper motor enable, low level effective
#define X_DIR     2       //X axis, stepper motor direction control 
#define X_STP     5       //x axis, stepper motor control

void step(boolean dir, byte dirPin, byte stepperPin, int steps)
{
  digitalWrite(dirPin, dir);
  delay(100);
  for (int i = 0; i < steps; i++) {
    digitalWrite(stepperPin, HIGH);
    delayMicroseconds(2000);  
    digitalWrite(stepperPin, LOW);
    delayMicroseconds(2000);  
  }
}

void setupStepper(){
  pinMode(X_DIR, OUTPUT); pinMode(X_STP, OUTPUT);
  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);
}


const char* ssid = "MyESP8266AP"; // Set your desired AP SSID
const char* password = "MyPassword"; // Set your desired AP password

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

void handleAbout(AsyncWebServerRequest* request) {
    // Read the "about.html" file from LittleFS
    File aboutFile = LittleFS.open("/about.html", "r");
    if (aboutFile) {
        request->send(aboutFile, "text/html");
        aboutFile.close();
    } else {
        // Custom response when "about.html" is not found
        String customResponse = "<html><body><h1>About page not found</h1></body></html>";
        request->send(200, "text/html", customResponse);
    }
}



void handleNotFound(AsyncWebServerRequest* request) {
    // Redirect all requests to the root ("/")
    request->redirect("/");
}

void setup() {
    Serial.begin(115200);

    setupStepper();
    // Initialize LittleFS
    LittleFS.begin();

    // Create an access point
    WiFi.softAP(ssid, password);
    IPAddress apIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(apIP);

    server.on("/", HTTP_GET, handleRoot); // Handle requests to the root ("/") route
    server.on("/about.html", HTTP_GET, handleAbout); // Handle requests to the root ("/about") route
    server.onNotFound(handleNotFound); // Handle requests to unknown routes


    server.begin();
    Serial.println("Web server started in AP mode");
}

void loop() {
   
}