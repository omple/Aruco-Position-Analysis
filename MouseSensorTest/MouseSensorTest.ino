#include <PS2Mouse.h>
#include <M5StickCPlus.h>


// Define the pins used for Clock and Data
#define MOUSE_DATA 32
#define MOUSE_CLOCK 33

// Initialize the PS2Mouse object
PS2Mouse mouse(MOUSE_DATA,MOUSE_CLOCK);


void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setRotation(3);
  M5.Lcd.setCursor(0, 0, 2);
  M5.Lcd.println(" Dynamixel Code Demo 2");
  delay(500);
  


  mouse.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setRotation(3);
  M5.Lcd.setCursor(0, 0, 2);
  M5.Lcd.println(" Done");
  delay(1000);

}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t stat;
  int x,y;

  mouse.getPosition(stat,x,y);

  /*
  Serial.print(stat, BIN);
  Serial.print("\tx=");
  Serial.print(x, DEC);
  Serial.print("\ty=");
  Serial.println(y, DEC);
*/
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0, 2); 
  M5.Lcd.printf(" LX: %4d\n", x);
  M5.Lcd.printf(" LY: %4d\n", y);
  
  delay(500);  

}
