/*
   M5Stack_DHT12_Thingspeak.ino

   Measuring temperature by ENV Unit and sending data
   containing measuring result to the Thingspeak platform.

   For internet access SIM800L Module om M5Stack Base Core is used.
   Connection to Thingspeak based on 
   https://community.thingspeak.com/forum/arduino/uploading-via-sim800-gsm-module-using-at-commands/

   createtd 2019-02-24 by Claus Kühnel (info@ckuehnel.ch) 
 */
#include <M5Stack.h>
#include "DHT12.h"
#include <Wire.h> //The DHT12 uses I2C comunication.
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>
#include "arduino_secrets.h"

DHT12 dht12; //Preset scale CELSIUS and ID 0x5c.
Adafruit_BMP280 bme;

#define RX_PIN      16
#define TX_PIN      17
#define RESET_PIN   5   // pin to the reset pin Sim800L
#define BAUD 115200

#define INFO 0
#define DEBUG 1

int minZ=1000;
int midZ=2500;  //time delays
int maxZ=5000;

float temp, hum, pressure;

void(* resetFunc) (void) = 0;   // declare reset function at address 0

void setup() 
{  
  M5.begin();

  Serial.begin(BAUD);
  Serial2.begin(BAUD, SERIAL_8N1, RX_PIN, TX_PIN);
  delay(2000);  // wait for serial monitor
  if (INFO)  printInfo();

  //M5.Lcd.setBrightness(50);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 20);
  M5.Lcd.setTextColor(LIGHTGREY, BLACK);
  M5.Lcd.setTextSize(3);
  M5.Lcd.printf("ENV Unit Values\r\n");
  M5.Lcd.setTextSize(2);
  
  M5.Lcd.setCursor(0,200);
  M5.Lcd.printf("Initialize...");

  if (DEBUG) Serial.println(F("\nInitializing..."));
  Serial.println(F("ENV Unit values will be sent to Thingspeak..."));
  if (DEBUG) Serial.println(F("Running..."));     //last line in setup()
  if (!bme.begin(0x76))
  {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    M5.Lcd.println("Could not find a valid BMP280 sensor, check wiring!");
    while(1);
  }
  delay(7000);  // wait for SIM800L
}

void loop()
{
  M5.Lcd.setCursor(0,200);
  M5.Lcd.printf("Read ENV Unit...   \n                  ");
  delay(500);
  
  temp = dht12.readTemperature();
  hum = dht12.readHumidity();
  pressure = bme.readPressure();
  Serial.printf("Temperature: %2.1f *C  Humidity: %2.0f %%  Pressure: %5.0f Pa\r\n", temp, hum, pressure);

  M5.Lcd.setCursor(0, 80);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.printf("Temperature: %2.1f *C\r\n\nHumidity: %2.0f %%\r\n\nPressure:%5.0f Pa\r\n", temp, hum, pressure);

  M5.Lcd.setCursor(0,200);
  M5.Lcd.setTextColor(LIGHTGREY, BLACK);
  M5.Lcd.printf("Updating Thingspeak\nDashboard...      ");
  sendUpdate();               // send temperature value only
  M5.Lcd.setCursor(0,200);
  M5.Lcd.printf("Updating Thingspeak\nDashboard done.");
  
  delay(1 * 60000);           // 5 minutes
  //resetFunc();                //call reset
}


/*--------------------------------------------------------------*/

void printInfo()
{
  // Compile file path (internal file of compilation proccess)
  Serial.print("File "); Serial.println(__FILE__);
  
  String s1 = __DATE__;
  String s2 = __TIME__;
  
  // Date of compilation
  Serial.print("Compilation @ "); Serial.println(s1 + " " + s2);
  
  // Arduino IDE SW version
  Serial.print("ARDUINO IDE v"); Serial.println(ARDUINO);
  
  // Oscillator frequency
  Serial.print("CPU Clock in MHz: "); Serial.println(F_CPU/1E6);
}
