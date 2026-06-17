#include <M5StickCPlus.h>
#include "OpticalFlowSensor.h"

// Define the pins used for Clock and Data
#define MOUSE_DATA 33
#define MOUSE_CLOCK 32

// Combined sensor object
OpticalFlowSensor* opticalSensor = nullptr;

void setup() {  
  pinMode(MOUSE_DATA, OUTPUT);
  pinMode(MOUSE_CLOCK, OUTPUT);

  M5.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setRotation(3);
  M5.Lcd.setCursor(0, 0, 2);
  M5.Lcd.println(" Optical Flow Test");
  delay(500);

  M5.Lcd.println(" Initializing sensor...");
  
  // Create combined sensor object
  opticalSensor = new OpticalFlowSensor(MOUSE_CLOCK, MOUSE_DATA);
  
  // Initialize the sensor
  opticalSensor->initialize();
  
  M5.Lcd.println(" Done");
  delay(1000);
}

void loop() {
  int signedX, signedY;
  
  // Read and process position data
  opticalSensor->readAndProcessPosition(signedX, signedY);
  
  // Display the position on screen
  opticalSensor->displayPosition(signedX, signedY);
  
  delay(20);
}
