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

void handleNotFound(AsyncWebServerRequest* request) {
    // Redirect all requests to the root ("/") to your desired webpage
    request->redirect("/");
}

void setup() {
    Serial.begin(115200);

    // Initialize LittleFS
    LittleFS.begin();

    // Create an access point
    WiFi.softAP(ssid, password);
    IPAddress apIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(apIP);

    server.on("/", HTTP_GET, handleRoot); // Handle requests to the root ("/") route
    server.onNotFound(handleNotFound); // Handle requests to unknown routes

    server.begin();
    Serial.println("Web server started in AP mode");
}

void loop() {
   
}







