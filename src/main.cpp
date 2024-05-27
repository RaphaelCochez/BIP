/*
 - Author: Raphael Cochez
 - Date: 27/05/24
 - Version: V0.2
 - License: Just use it lol

  webserver with basic handling and routing
  reads the config.json file as its main parameters
  has the ability to change a parameter
  serves html, css and a svg logo to a root (/) location.4
  this code needs a data/config.json, a data/index.html, a data/index.js and a data/logo.svg file uploaded in the SPDIFFS in order to work

*/

#include <ESP8266WiFi.h>
#include <LittleFS.h> // Include the LittleFS library
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>


class Configuration {
public:
    Configuration(const char* filename) : filename(filename) {}

    bool readParameters() {
        File file = LittleFS.open(filename, "r");
        if (!file) {
            Serial.println(F("Error opening config file for reading"));
            return false;
        }

        StaticJsonDocument<300> doc; // Use JsonDocument instead of StaticJsonDocument
        DeserializationError error = deserializeJson(doc, file);
        file.close();

        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return false;
        }

        JsonObject wifiConfig = doc["WiFi"];
        NETWORK_SSID = wifiConfig["KEY_NETWORK_SSID"].as<const char*>(); // Update the pointer
        NETWORK_PASSWORD = wifiConfig["KEY_NETWORK_PASSWORD"].as<const char*>(); // Update the pointer

        JsonObject paramConfig = doc["parameters"];
        min_portions = paramConfig["min_portions"];
        max_portions = paramConfig["max_portions"];
        rotations_per_portion = paramConfig["rotations_per_portion"];
        steps_per_portion = paramConfig["steps_per_portion"];

        return true;
    }

    const char* getNetworkSSID() const {
        return NETWORK_SSID;
    }

    const char* getNetworkPassword() const {
        return NETWORK_PASSWORD;
    }

    int getMinPortions() const {
        return min_portions;
    }

    int getMaxPortions() const {
        return max_portions;
    }

    int getRotationsPerPortion() const {
        return rotations_per_portion;
    }

    int getStepsPerPortion() const {
        return steps_per_portion;
    }

    void updateMinPortions(const int newValue) {
        File file = LittleFS.open(filename, "w"); // Open in write mode
        if (!file) {
            Serial.println(F("Error opening config file for writing"));
            return;
        }

        DynamicJsonDocument doc(512); // Adjust the size as needed
        DeserializationError error = deserializeJson(doc, file);
        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            file.close();
            return;
        }

        doc["parameters"]["min_portions"] = newValue;

        file.seek(0); // Move to the beginning of the file
        serializeJson(doc, file); // Write the updated JSON back to the file
        file.close();
    }

private:
    const char* filename;
    const char* NETWORK_SSID = nullptr;
    const char* NETWORK_PASSWORD = nullptr;
    int min_portions = 0;
    int max_portions = 0;
    int rotations_per_portion = 0;
    int steps_per_portion = 0;
};

Configuration config("/config.json");
const char* ssid = "BIPPUPPYFEEDING";
const char* password = "123456789";

AsyncWebServer server(80); // Create an instance of AsyncWebServer
void handleRoot(AsyncWebServerRequest* request) {
    // Serve the html file
    File htmlFile = LittleFS.open("/index.html", "r");
    if (htmlFile) {
        request->send(200, "text/html", htmlFile.readString());
        htmlFile.close();
    } else {
        // Custom response when "index.html" is not found
        String customResponse = "<html><body><h1>LittleFS not found</h1></body></html>";
        request->send(200, "text/html", customResponse);
    }
}



void handleCSS(AsyncWebServerRequest* request) {
    // Serve the CSS file
    File cssFile = LittleFS.open("/style.css", "r");
    if (cssFile) {
        request->send(200, "text/css", cssFile.readString());
        cssFile.close();
    } 
}


void handleJS(AsyncWebServerRequest* request) {
    // Serve the javascript
    File jsFile = LittleFS.open("/index.js", "r");
    if (jsFile) {
        request->send(200, "application/javascript", jsFile.readString());
        jsFile.close();
    }
}

void handleLogo(AsyncWebServerRequest* request) {
    // Serve the SVG logo
    File logoFile = LittleFS.open("/logo.svg", "r");
    if (logoFile) {
        request->send(200, "image/svg+xml", logoFile.readString());
        logoFile.close();
    } 
}


void handleNotFound(AsyncWebServerRequest* request) {
    // Redirect all requests to the root ("/") to your desired webpage
    request->redirect("/");
}

void setup() {
    Serial.begin(9600);

    // Initialize LittleFS
    LittleFS.begin();

    // Set WiFi mode to be an access point and create it
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);

    // create a ip adres for a client device
    IPAddress apIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(apIP);

    // Handle requests to the root ("/") route
    server.on("/", HTTP_GET, handleRoot);
    server.on("/style.css", HTTP_GET, handleCSS);
    server.on("/index.js", HTTP_GET, handleJS);
    server.on("/logo.svg", HTTP_GET, handleLogo);

    // Handle form input


    // Handle requests to unknown routes
    server.onNotFound(handleNotFound);

    // start server
    server.begin();
    Serial.println();
    Serial.println("Web server started in AP mode");
}

void loop() {
}
