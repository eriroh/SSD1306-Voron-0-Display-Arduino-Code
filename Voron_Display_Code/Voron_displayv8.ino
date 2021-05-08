/* Code Written - May 6th 2021
  by Jesse Fabbro & Eric Rohrbeck

  To display Voron Design Bootscreen/ DHT22 Environmental sensor data on an SSD1306 OLED Display.
*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif18pt7b.h>
#include <D:/Voron_displayv6/Voron_displayv8/bootscreenbmp.h>
#include <D:/Voron_displayv6/Voron_displayv8/logobmp.h>
#include <D:/Voron_displayv6/Voron_displayv8/voronv0.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT22   // DHT 11

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

//define Time [sec] screen is shown
#define BOOTSCREENTIME 15
#define TEMPSCREENTIME 25
#define VORONSCREENTIME 15

int h;
int t;
int32_t timer_current = 0;
int32_t last0 = 0;
int32_t last_i = 0;

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);
  dht.begin();

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  // Clear the buffer
  display.clearDisplay();
  showBootscreen();
  display.display();
  delay(BOOTSCREENTIME);
  display.clearDisplay();
}

void loop() {
  timer_current = millis();

// Show bootscreen for BOOTSCREENTIME, Voronscreen for VORONSCREENTIME, Tempscreen for TEMPSCREENTIME 
// BOOTSCREENTIME
// VORONSCREENTIME
// TEMPSCREENTIME

  if(timer_current >= last0 + 1000){
    h = dht.readHumidity();
    t = dht.readTemperature();
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    if (last_i >= BOOTSCREENTIME){
      if (last_i >= BOOTSCREENTIME + VORONSCREENTIME){
        display.clearDisplay();
        showBitmap();
        printText();
        display.display();
        if(last_i >= BOOTSCREENTIME + VORONSCREENTIME + TEMPSCREENTIME){
          last_i = 0;
        }
      }
      else{
        display.clearDisplay();
        showVoronv0();
        display.display();
      }      
      last_i = last_i + 1;
    }
    
   else{
      display.clearDisplay();
      showBootscreen();
      display.display();
      last_i = last_i + 1;
    }
    
   
    last0 = millis();
  }
  else {
    delay(10);
  }
}

void printText() {
  display.setFont(&FreeSerif18pt7b);
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(50, 28);            // Start at top-left corner
  display.print(t);
  display.drawCircle(92, 8, 3, WHITE);
  display.setCursor(100, 27);
  display.print("C");
  display.setCursor(50, 62);
  display.print(h);
  display.print("%");

}

void showVoronv0(void) {
  display.drawBitmap(0, 0, voron0v0_bmp, bitmap_height1, bitmap_width1, WHITE);
}

void showBitmap(void) {
  display.drawBitmap(0, 10, logo_bmp, bitmap_height, bitmap_width, WHITE);
  //display.display();
}

void showBootscreen(void) {
  display.drawBitmap(0, 0, bootscreen_bmp, bitmap_height2, bitmap_width2, WHITE);
  //display.display();
}
