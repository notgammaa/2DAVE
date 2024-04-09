#include <Arduino.h>
#include <U8g2lib.h>
#include <SoftwareSerial.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);

void setup(void){
  u8g2.begin();
  Serial.begin(9600);
}

void loop(void) {
  u8g2.clearBuffer();         // clear the internal memory
  u8g2.setFont(u8g2_font_crox4hb_tr); // choose a suitable font
  u8g2.drawStr(0,16,"Testing OLED display here"); // write something to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display 
  delay(300);
}

