/*
 * Settings for the Calendar Display
 */

/* WiFi credentials */
// Wi-Fi Credentials  
const char* ssid = "";                                      // WiFi SSID  
const char* password = "";                   // WiFi password

// Calendar Server  
const char* calkey = "";                                 // Key to access calendar data  
const char calserver[] = "";                       // Locally hosted Calendar Server URL

// OpenWeatherMap API  
const char* apikey = "";              // API key  
const char server[] = "api.openweathermap.org";             // URL (don't change)

/* API Parameters Open Weathermap */
const char* LAT             = "52.5";                                  // Home location Latitude
const char* LON             = "-2.08";                                 // Home location Longitude
const char* City            = "Dudley";                                // Your home city
const char* Country         = "GB";                                    // Your country code
const char* Language        = "EN";                                    // Language for weather description
const char* Hemisphere      = "north"; 
const char* units           = "metric";                                // Units (imperial or metric)

/* Time and NTP Server */
const char* ntpServer = ".uk.pool.ntp.org";                         // NTP Server URL  
const char* timezoneStr = "GMTBST,M3.5./1,M10.5.";                // Timezone Definition

/* Sleep Time in Minutes */
const long SleepDuration    = 30;                                      // Display update interval in minutes
const int PowerSaveStart    = 22;                                      // Start power save mode
const int PowerSaveEnd      = 5;                                       // End power save mode

/* Notification sounds */
const bool playAlarm        = false;                                   // Play Calendar Alarms
const bool playBday         = false;                                   // Play Birthday Notifications
const int playBdayTime      = 10;                                      // Time for birthday notifications
