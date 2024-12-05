/*
 * Settings for the Calendar Display
 *
 * Timezone: 
 * https://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html
 */

/* WiFi credentials */
const char* ssid            = "";                                      // WiFi SSID
const char* password        = "";                         // WiFi password

/* Calendar */
String calkey               = "";                                   // Key to access calendar data
const char calserver[]      = "SERVER-URL:3000";                             // Calendar Server URL

/* Open Weathermap Account */
String apikey               = "";                // Get the developer key for 1000 request/day
const char server[]         = "api.openweathermap.org";                          // URL (don't change)

/* API Parameters Open Weathermap */
String LAT              = "52.5";                         // Home location Latitude
String LON              = "-2.08";                        // Home location Longitude
String City             = "Dudley";                       // Your home city See: http://bulk.openweathermap.org/sample/
String Country          = "GB";                           // Your _ISO-3166-1_two-letter_country_code country code, on OWM find your nearest city and the country code is displayed
String Language         = "EN";                           // NOTE: Only the weather description is translated by OWM
String Hemisphere       = "north"; 
String units            = "metric";  

/* Time and NTP Server */
const char* ntpServer       = "0.uk.pool.ntp.org";                          // URL address
const char* timezoneStr     = "GMT0BST,M3.5.0/1,M10.5.0";              // Timezone Definition (https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv)
int gmtOffset_sec           = 0;                                  // Timezone offset of your location in seconds -18000;

/* Sleep Time in Minutes */
const long SleepDuration    = 30;                                      // Display update interval in minutes
const int PowerSaveStart    = 22;                                      // Start power save where display is not updated (24 hour format)
const int PowerSaveEnd      =  5;                                      // End power save where display is not updated (24 hour format)

/* Notification sounds */
const bool playAlarm        = false;                                   // Play Calendar Alarms
const bool playBday         = false;                                   // Play Birthday Notifications (true/false)
const int playBdayTime      =  10;                                     // Play Birthday Notification at full hour (24 hour format)
