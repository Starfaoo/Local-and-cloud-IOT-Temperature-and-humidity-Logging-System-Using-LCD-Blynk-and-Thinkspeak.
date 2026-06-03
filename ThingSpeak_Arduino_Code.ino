
const long myChannelNumber = "YOUR_THINGSPEAK_CHANNEL_ID" ;
const char* myWriteAPIKey = "YOUR_THINGSPEAK_CHANNEL_ID";

// WiFi Credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* pass = "YOUR_WIFI_PASSWORD"; 

// DHT PARAMETERS 
#define DHTPIN D3
#define DHTTYPE DHT11

// LCD PARAMETERS
#define LCD_I2C_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

// Include necessary libraries
#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h> 

// Initialize objects
WiFiClient client;
LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_COLUMNS, LCD_ROWS);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Initialize LCD and show connecting status
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting to");
  lcd.setCursor(0, 1);
  lcd.print("ThingSpeak...");

  // Connect to WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Initialize DHT sensor and ThingSpeak client
  dht.begin();
  ThingSpeak.begin(client);
  
  // Clear the connecting message
  lcd.clear();
}

void loop() {
  // Read sensor values
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  // Check for a failed reading
  if (isnan(h) || isnan(t)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DHT Read Failed!");
    delay(2000); // Wait before trying again
    return;
  }

  // --- Display on LCD ---
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(t, 1); // Print with 1 decimal place
  lcd.print((char)223); 
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Humi: ");
  lcd.print(h, 0);
  lcd.print("%");
  
  // Send to ThingSpeaK
  // Set the fields with the sensor values
  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);

  // Write to the ThingSpeak channel
  int httpCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  if (httpCode == 200) {
    // Optional: Add feedback for successful update
  } else {
    // Optional: Add feedback for failed update
  }
  
  // ThingSpeak has a rate limit of ~15 seconds for free accounts.
  // We'll wait 20 seconds to be safe.
  delay(20000); 
}
