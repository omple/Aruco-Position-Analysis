#ifndef OPTICAL_FLOW_SENSOR_H
#define OPTICAL_FLOW_SENSOR_H

#include <PS2Mouse.h>
#include <M5StickCPlus.h>

class OpticalFlowSensor {
private:
  PS2Mouse mouse;
  
  int convertSignedValue(uint8_t value);
  
public:
  OpticalFlowSensor(uint8_t clockPin, uint8_t dataPin);
  ~OpticalFlowSensor();
  
  void initialize();
  void readAndProcessPosition(int &signedX, int &signedY);
  void displayPosition(int signedX, int signedY);
};

#endif

