#define USE_HSPI_FOR_EPD  
#include <GxEPD2_3C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <RTClib.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

// Include your credentials file  
#include "credentials.h"  // Ensure this file is in the same directory

// Display Configuration  
#define GxEPD2_DISPLAY_CLASS GxEPD2_3C  
#define GxEPD2_DRIVER_CLASS GxEPD2_750c_Z08

GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, 480> display(GxEPD2_DRIVER_CLASS(/*CS=*/ 15, /*DC=*/ 27, /*RST=*/ 26, /*BUSY=*/ 25));
RTC_DS3231 rtc;

#if defined(ESP32) && defined(USE_HSPI_FOR_EPD)
#include <SPI.h> // Ensure SPI library is included  
SPIClass hspi(HSPI);
#endif

void setup() {
    Serial.begin(115200);
    Serial.println("Setup started...");

    // Initialize the RTC  
    if (!rtc.begin()) {
        Serial.println("Error: RTC DS3231 not found.");
        while (1);
    }

    // Initialize Display  
#if defined(ESP32) && defined(USE_HSPI_FOR_EPD)
    hspi.begin(13, 12, 14, 15); // Remap HSPI for EPD (swap pins)
    display.epd2.selectSPI(hspi, SPISettings(400000, MSBFIRST, SPI_MODE0)); // Use SPI_MODE  
#endif

    display.init(115200); // Initialize the display  
    Serial.println("Display initialized.");

    // Clear the display  
    display.fillScreen(GxEPD_WHITE);
    Serial.println("Display cleared.");

    // Fetch current time from RTC  
    DateTime now = rtc.now();
    drawCalendar(now); // Function to draw calendar  
}

void loop() {
    // Nothing to do here  
}

// Function to draw the calendar  
void drawCalendar(const DateTime& dt) {
    display.setTextColor(GxEPD_BLACK);
    display.setFont(&FreeMonoBold9pt7b);
    display.setCursor(10, 50);
    display.print("Current Date:");
    display.setCursor(10, 70);
    display.print(dt.year(), DEC);
    display.print('/');
    display.print(dt.month(), DEC);
    display.print('/');
    display.print(dt.day(), DEC);
    display.setCursor(10, 100);
    display.print("Time: ");
    display.print(dt.hour(), DEC);
    display.print(':');
    display.print(dt.minute(), DEC);
    display.print(':');
    display.print(dt.second(), DEC);
    
    // Update the display  
    display.display(true); // Force a full update  
}

/* Start WiFi and Connect to Network */
uint8_t StartWiFi() {
    Serial.print("\r\nConnecting to: "); Serial.println(ssid);
    WiFi.disconnect();
    WiFi.mode(WIFI_STA); // switch off AP  
    WiFi.setAutoReconnect(true);
    WiFi.begin(ssid, password);
    unsigned long start = millis();
    uint8_t connectionStatus;
    bool AttemptConnection = true;
    while (AttemptConnection) {
        connectionStatus = WiFi.status();
        if (millis() > start + 15000) { // Wait 15-secs maximum  
            AttemptConnection = false;
        }
        if (connectionStatus == WL_CONNECTED || connectionStatus == WL_CONNECT_FAILED) {
            AttemptConnection = false;
        }
        delay(50);
    }
    if (connectionStatus == WL_CONNECTED) {
        Serial.println("WiFi connected at: " + WiFi.localIP().toString());
        return connectionStatus;
    } else {
        Serial.println("WiFi connection *** FAILED ***");
        return connectionStatus;
    }
}

/* Http Request */
bool httpsRequest(WiFiClient& client, int req) {
    // close connection before sending a new request  
    client.stop();
    HTTPClient http;
    http.setTimeout(60000); // the local RPi Zero is very slow, so a long time out is needed  
    if (req == 1) {
        String fullURL = String("http://") + calserver + "/?token=" + calkey;
        http.begin(fullURL);
    }
    if (req == 2) {
        String uri = String("/data/2.5/weather?lat=") + LAT + "&lon=" + LON + "&appid=" + apikey + "&units=" + units;
        http.begin(client, server, 443, uri);
    }
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
        if (req == 1 && !parseIcalJSON(http.getString())) return false;
        if (req == 2 && !parseWeathermapJSON(http.getStream())) return false;
        client.stop();
        http.end();
        return true;
    } else {
        Serial.printf("connection failed, error: %s", http.errorToString(httpCode).c_str());
        Serial.println(httpCode);
        client.stop();
        http.end();
        return false;
    }
}

/* Parse the calendar JSON */
bool parseIcalJSON(String json) {
    Serial.print(F("\nCreating object... Calendar"));

    // Allocate a smaller JsonDocument  
    DynamicJsonDocument doc(1024); // Adjust the size as needed
  
    // Deserialize the JSON document  
    DeserializationError error = deserializeJson(doc, json.c_str());
    // Test if parsing succeeds.
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return false;
    }

    Serial.println(doc.memoryUsage());

    if (doc.containsKey("events")) {
        if (doc["events"].size() > ) {
            for (byte r = ; r < 10; r++) {
                if (doc["events"][r].containsKey("gmtoffset")) {
                    Events[r].starttime = doc["events"][r]["start"].as<time_t>() + doc["events"][r]["gmtoffset"].as<int>() * 60;
                    Events[r].endtime = doc["events"][r]["end"].as<time_t>() + doc["events"][r]["gmtoffset"].as<int>() * 60;
                } else {
                    Events[r].starttime = doc["events"][r]["start"].as<time_t>() + doc["gmtoffset"].as<int>() * 60;
                    Events[r].endtime = doc["events"][r]["end"].as<time_t>() + doc["gmtoffset"].as<int>() * 60;
                }

                Events[r].description = doc["events"][r]["summary"].as<const char*>();
                if (doc["events"][r]["location"] == nullptr) {
                    Events[r].location = "";
                } else {
                    Events[r].location = doc["events"][r]["location"].as<const char*>();
                }

                if (doc["events"][r]["categories"] == nullptr) {
                    Events[r].categories = "";
                } else {
                    Events[r].categories = doc["events"][r]["categories"].as<const char*>();
                }
                Events[r].alarm = false;
                Events[r].allday = doc["events"][r]["allday"];
            }
        }
    }

    doc.clear();
    return true;
}

/* Parse the weather JSON */
bool parseWeathermapJSON(WiFiClient& json) {
    Serial.print(F("Parsing Weather"));

    // Allocate the JsonDocument  
    DynamicJsonDocument doc(3 * 1024);
  
    // Deserialize the JSON document  
    DeserializationError error = deserializeJson(doc, json);
    // Test if parsing succeeds.
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return false;
    }

    Serial.println(doc.memoryUsage());

    if (doc.containsKey("weather")) {
        if (doc["weather"].size() > ) {
            Weather[].summary = doc["weather"][]["description"].as<const char*>();
            Weather[].icon = doc["weather"][]["icon"].as<const char*>();
        }
    }
    if (doc.containsKey("main")) {
        Weather[].temperature = doc["main"]["temp"].as<float>();
    }

    if (doc.containsKey("timezone")) {
        gmtOffset_sec = doc["timezone"].as<int>();
    }

    doc.clear();
    return true;
}

/* Show Date over Serial */
void showDate(const DateTime& dt) {
    Serial.print(dt.year(), DEC);
    Serial.print('/');
    Serial.print(dt.month(), DEC);
    Serial.print('/');
    Serial.print(dt.day(), DEC);
    Serial.print(' ');
    Serial.print(dt.hour(), DEC);
    Serial.print(':');
    Serial.print(dt.minute(), DEC);
    Serial.print(':');
    Serial.print(dt.second(), DEC);
    Serial.println();
}
