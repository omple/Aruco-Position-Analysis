#include <PS2Mouse.h>
#include <M5StickCPlus.h>


// Define the pins used for Clock and Data
#define MOUSE_DATA 33
#define MOUSE_CLOCK 32


// Initialize the PS2Mouse object
PS2Mouse mouse(MOUSE_CLOCK, MOUSE_DATA);


void setup() {  
  pinMode(33, OUTPUT);
  pinMode(32,OUTPUT);


  M5.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setRotation(3);
  M5.Lcd.setCursor(0, 0, 2);
  M5.Lcd.println(" Mouse Sensor Demo 2");
  delay(500);

  M5.Lcd.println(" Init mouse...");
  unsigned long start = millis();
  mouse.begin();        // or mouse.initialize() depending on lib
  // if code after this never runs, begin() is blocking forever
  M5.Lcd.println(" Done");
  delay(1000);

}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t stat;
  int x,y;

  mouse.getPosition(stat,x,y);

  int sx = (x > 127) ? (int)x - 65536 : (int)x;
  int sy = (y > 127) ? (int)y - 65536 : (int)y;
  /*
  Serial.print(stat, BIN);
  Serial.print("\tx=");
  Serial.print(x, DEC);
  Serial.print("\ty=");
  Serial.println(y, DEC);
*/
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0, 2); 
  M5.Lcd.printf(" LX: %4d\n", sx);
  M5.Lcd.printf(" LY: %4d\n", sy);
  
  delay(20);

}
