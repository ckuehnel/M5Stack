#include <M5StickC.h>
#include <Wire.h>

#define CARDKB_ADDR 0x5F

void setup()
{
  M5.begin();
  Wire.begin(0, 26);
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0, 2 );
  M5.Lcd.setTextColor(WHITE);

  M5.Lcd.println("IIC Address: 0x5F");
  M5.Lcd.println(">>");
  M5.Lcd.setTextColor(GREEN);
  Serial.println("Press any key of M5StickC KB...");
}

void loop()
{
  Wire.requestFrom(CARDKB_ADDR, 1);
  while(Wire.available())
  {
    char c = Wire.read(); // receive a byte as characterif
    if (c != 0)
    {
      M5.Lcd.printf("%c", c);
      Serial.print(c, HEX);
      Serial.print("\t");
      Serial.println(char(c));
    }
  }
  // delay(10);
}
