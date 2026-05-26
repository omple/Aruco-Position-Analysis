#include <M5StickCPlus.h>
#include <Dynamixel2Arduino.h>
#include "BluetoothSerial.h"



#define RS485_RX 26
#define RS485_TX 0

#define MAX_RPM 29

#define DXL_SERIAL Serial2



const uint8_t DXL_ID_1 = 1;
const uint8_t DXL_ID_2 = 2;
const uint8_t DXL_ID_3 = 3;

// Protocol & baud
const float   DXL_PROTOCOL_VERSION = 2.0;
const uint32_t DXL_BAUD = 57600;   // set to your motor's baud

Dynamixel2Arduino dxl(DXL_SERIAL);

BluetoothSerial SerialBT;

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setRotation(3);
  M5.Lcd.setCursor(0, 0, 2);
  M5.Lcd.println(" Spherical Wheel Laptop Connection");

  
  // Initialize RS485 UART on Serial2
  Serial2.begin(9600, SERIAL_8N1, RS485_RX, RS485_TX);  

  Serial.begin(9600); // For debugging
  Serial.println(" RS485 initialized");

  SerialBT.begin("Spherical_Wheel_BT"); 

  DXL_SERIAL.begin(DXL_BAUD, SERIAL_8N1, RS485_RX, RS485_TX);

  // Init Dynamixel2Arduino
  dxl.begin(DXL_BAUD);
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);

  dxl.torqueOff(DXL_ID_1);
  dxl.setOperatingMode(DXL_ID_1, OP_VELOCITY);
  dxl.torqueOn(DXL_ID_1);

  dxl.torqueOff(DXL_ID_2);
  dxl.setOperatingMode(DXL_ID_2, OP_VELOCITY);
  dxl.torqueOn(DXL_ID_2);

  dxl.torqueOff(DXL_ID_3);
  dxl.setOperatingMode(DXL_ID_3, OP_VELOCITY);
  dxl.torqueOn(DXL_ID_3);
}

void loop() {
  // put your main code here, to run repeatedly:
  M5.update();
  
  int lastLx = 0, lastLy = 0;
  if (SerialBT.available() > 0) {
    // read velocity from 0-100 and angle from 0-360
    int R = SerialBT.parseInt();
    int angle = SerialBT.parseInt();

    float angleRad = angle * PI / 180.0;
    float angleRad = atan2(lyNew,lxNew);
    float angle = angleRad / PI * 180;

    float M1 = 0, M2 = 0,M3 = 0;
    if(abs(R) > 50){
      if((angle < 180 && angle > 90) || (angle > -180 && angle < -135)){
        M1 = R * sin(angleRad) - R * cos(angleRad) * tan(-7*PI/4);
        M3 = R * cos(angleRad) / cos(-7*PI/4);
      }else if (angle < -45 && angle > -135){
        M2 = (R * sin(angleRad)- R * cos(angleRad) * tan(-7*PI/4)) / (sin(-1*PI/4) - cos(-1*PI/4) * tan(-7*PI/4));
        M3 = ( R *sin(angleRad)- R * cos(angleRad) * tan(-1*PI/4)) / (sin(-7*PI/4) - cos(-7*PI/4) * tan(-1*PI/4));
      }else{
        M1 = R * sin(angleRad) - R * cos(angleRad) * tan(-1*PI/4);
        M2 = R * cos(angleRad) / cos(-1*PI/4);
      }
    }

    M1 = abs(M1);
    M2 = abs(M2);
    M3 = abs(M3);
    
    dxl.setGoalVelocity(DXL_ID_1, M1 * MAX_RPM / 1000.0, UNIT_RPM);
    dxl.setGoalVelocity(DXL_ID_2, M2 * MAX_RPM / 1000.0, UNIT_RPM);
    dxl.setGoalVelocity(DXL_ID_3, M3 * MAX_RPM / 1000.0, UNIT_RPM);
    

    
    if (lx != lastLx || ly != lastLy) {
      M5.Lcd.fillScreen(BLACK);
      M5.Lcd.setCursor(0, 0, 2); 
      M5.Lcd.printf(" LX: %4d\n", lxNew);
      M5.Lcd.printf(" LY: %4d\n", lyNew);
      M5.Lcd.printf(" R: %4f\n", R);
      M5.Lcd.printf(" Angle: %4f\n", angle);
      M5.Lcd.printf(" M1: %4f\n", M1);
      M5.Lcd.printf(" M2: %4f\n", M2);
      M5.Lcd.printf(" M3: %4f\n", M3);
    }
    lastLx = lx;
    lastLy = ly;
    delay(20);
  } else {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0, 2);
    M5.Lcd.println(" No Laptop Connection Detected");
    delay(400);
  }

  /*
  if(Serial.available() > 0){
    // read velocity from 0-100 and angle from 0-360
    int R = Serial.parseInt();
    int angle = Serial.parseInt();
    float angleRad = angle * PI / 180.0;

    float Rx = vel * cos(angleRad);
    float Ry = vel * sin(angleRad);
    if(angle < 135 && angle > 90){
      M1 = R * sin(angleRad) - R * cos(angleRad) * tan(-7*PI/4);
      M3 = R * cos(angleRad) / cos(-7*PI/4);
    }else if (angle < 225 && angle > 90){
      M2 = (R*sin(angleRad)-R*cos(angleRad)*tan(-7*PI/4))/(sin(-1*PI/4)-cos(-1*PI/4)*tan(-7*PI/4));
      M3 = (R*sin(angleRad)-R*cos(angleRad)*tan(-1*PI/4))/(sin(-7*PI/4)-cos(-7*PI/4)*tan(-1*PI/4));
    }else{
      M1 = R * sin(angleRad) - R * cos(angleRad) * tan(-1*PI/4);
      M2 = R * cos(angleRad) / cos(-1*PI/4);
    }
  }
    */


  

}
