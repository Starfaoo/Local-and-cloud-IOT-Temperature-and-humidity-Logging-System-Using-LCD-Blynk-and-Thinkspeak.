#define BLYNK_TEMPLATE_ID "REPLACE_WITH_YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "REPLACE_WITH_YOUR_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "REPLACE_WITH_YOUR_BLYNK_TOKEN"

// DHT PARAMETERS 
#define DHTPIN D3
#define DHTTYPE DHT11

// LCD PARAMETERS
#define LCD_I2C_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h> 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h> 

char auth[] = BLYNK_AUTH_TOKEN;

// Wi-Fi details
char ssid[] = "REPLACE_WITH_YOUR_WIFI_SSID";
char pass[] = "REPLACE_WITH_YOUR_WIFI_PASS"; 

LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_COLUMNS, LCD_ROWS);

BlynkTimer timer;

DHT dht(DHTPIN, DHTTYPE);

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 

  if (isnan(h) || isnan(t)) {    
    // Display error on LCD
    lcd.setCursor(0, 0);
    lcd.print("DHT Read Failed!");
    lcd.setCursor(0, 1);
    return;
  }
  
  // Update Blynk Virtual Pins
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);

  // Display on LCD
  // Line 1: Temperature
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(t, 2); // Print temperature with 1 decimal place
  lcd.print((char)223); 
  lcd.print("C");

  // Line 2: Humidity
  lcd.setCursor(0, 1);
  lcd.print("Humi: ");
  lcd.print(h, 1); // Print humidity as an integer (0 decimal places)
  lcd.print("%");
}

void setup()
{ 
  // Initialize LCD before connecting to WiFi
  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("Connecting to");
  lcd.setCursor(0, 1);
  lcd.print("Blynk...");

  // The Blynk.begin call remains the same for hardcoded credentials
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  
  // Clear the connecting message after successful connection
  lcd.clear();

  // Set up timer to call sendSensor() every 100 milliseconds
  timer.setInterval(100, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}
