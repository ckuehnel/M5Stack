// M5StickC_Yun_Hat.ino

// Original at https://github.com/m5stack/M5-ProductExampleCodes/tree/master/Hat/hat-yun
// please install Adafruit_BMP280 lib first
// Adafruit_BMP280 lib in Sketch->Includ Library->Library Manager

// Minor modifications Claus Kuehnel 2019-09-13 info@ckuehnel.ch

#define DEBUG 0

#include "M5StickC.h"
#include <Adafruit_BMP280.h>
#include "SHT20.h"
#include "yunBoard.h"
#include <math.h>
#include "display.h"

SHT20 sht20;
Adafruit_BMP280 bmp;

uint32_t update_time = 0;
float tmp, hum;
float pressure;
uint16_t light;
extern uint8_t  lightR;
extern uint8_t  lightG;

void setup() 
{
  int8_t i,j;
  M5.begin();
  Wire.begin(0, 26, 100000);
  M5.Lcd.setRotation(1);
  M5.Lcd.setTextSize(2);

  if (DEBUG) Serial.println(F("Debug Mode for M5StickC Yun Hat"));
  
  // RGB888
  // led_set(uint8_t 1, 0x080808);
  
  if (!bmp.begin(0x76)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_1000); /* Standby time. */
  if (DEBUG) Serial.println(F("BMP280 initialized."));
  
  display_light4();
}

uint8_t light_flag=0;

void loop() 
{
  // led_set_all((lightR << 16) | (lightG << 8) | lightB);
  if(millis() > update_time) {
    update_time = millis() + 1000;
    tmp = sht20.read_temperature();
    hum = sht20.read_humidity();
    light = light_get();
    pressure = bmp.readPressure();
    M5.Lcd.setCursor(3, 3);
    M5.Lcd.setTextColor(TFT_BLUE, TFT_BLACK);
    M5.Lcd.print("Yun Hat");
    M5.Lcd.setCursor(0, 25);
    M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
    M5.Lcd.printf("%.1f *C\r\n", tmp);
    M5.Lcd.setCursor(0, 25+20);
    M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
    M5.Lcd.printf("%d %%\r\n", int(hum));
    M5.Lcd.setCursor(0, 25+20+20);
    M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
    M5.Lcd.printf("%d Pa\r\n", int(pressure));
    if (DEBUG)
    {
      Serial.print("\nTemperature = "); Serial.print(tmp, 1); Serial.println(" *C");
      Serial.print("Humidity = "); Serial.print((int) hum); Serial.println(" %");
      Serial.print("Pressure = "); Serial.print((int) pressure); Serial.println(" *C");
      Serial.print("Light = "); Serial.println(light);
      Serial.println("");
    }
  }
  M5.update();

  if(M5.BtnA.wasPressed()) {
    esp_restart();  
  }

  delay(10);
  
  display_light();
  if(light>1500)
  {
    if(light_flag==0)
    {
      light_flag=1;
       lightR=40;
       lightG=40;
      
    }
//    led_breath();
  led_on(15);
  }
  else 
  {
    led_off();
    light_flag=0;
  }
  

}
