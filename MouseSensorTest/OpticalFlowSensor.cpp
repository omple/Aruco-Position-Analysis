#include "OpticalFlowSensor.h"

OpticalFlowSensor::OpticalFlowSensor(uint8_t clockPin, uint8_t dataPin)
  : mouse(clockPin, dataPin) {
}

OpticalFlowSensor::~OpticalFlowSensor() {
}

int OpticalFlowSensor::convertSignedValue(uint8_t value) {
  return (value > 127) ? (int)value - 65536 : (int)value;
}

void OpticalFlowSensor::initialize() {
  mouse.begin();
}

void OpticalFlowSensor::readAndProcessPosition(int &signedX, int &signedY) {
  uint8_t stat;
  int x, y;
  
  mouse.getPosition(stat, x, y);
  
  signedX = convertSignedValue(x);
  signedY = convertSignedValue(y);
}

void OpticalFlowSensor::displayPosition(int signedX, int signedY) {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0, 2);
  M5.Lcd.printf(" LX: %4d\n", signedX);
  M5.Lcd.printf(" LY: %4d\n", signedY);
}

