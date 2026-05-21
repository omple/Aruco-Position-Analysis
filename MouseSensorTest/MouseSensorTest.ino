#include <PS2Mouse.h>

// Define the pins used for Clock and Data
#define MOUSE_DATA 32
#define MOUSE_CLOCK 33

// Initialize the PS2Mouse object
PS2Mouse mouse(MOUSE_DATA,MOUSE_CLOCK);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial);
  Serial.print("Setup...");


  mouse.begin();
  Serial.println("complete!");

}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t stat;
  int x,y;

  mouse.getPosition(stat,x,y);

  Serial.print(stat, BIN);
  Serial.print("\tx=");
  Serial.print(x, DEC);
  Serial.print("\ty=");
  Serial.println(y, DEC);
  
  delay(500);  

}
