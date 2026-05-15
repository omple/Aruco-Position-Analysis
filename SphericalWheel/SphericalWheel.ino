#include <M5StickCPlus.h>
#include <Dynamixel2Arduino.h>


#define RS485_RX 26
#define RS485_TX 0

#define DXL_SERIAL Serial2

const uint8_t DXL_ID_1 = 1;
const uint8_t DXL_ID_2 = 2;
const uint8_t DXL_ID_3 = 3;

// Protocol & baud
const float   DXL_PROTOCOL_VERSION = 2.0;
const uint32_t DXL_BAUD = 57600;   // set to your motor's baud

Dynamixel2Arduino dxl(DXL_SERIAL);

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setRotation(3);
  M5.Lcd.setCursor(0, 0, 2);
  M5.Lcd.println("Dynamixel Code Demo 2");

  // Initialize RS485 UART on Serial2
  Serial2.begin(9600, SERIAL_8N1, RS485_RX, RS485_TX);  

  Serial.begin(9600); // For debugging
  Serial.println("RS485 initialized");

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
}

void loop() {
  // put your main code here, to run repeatedly:
  M5.update();

  /*
  if(M5.BtnA.isPressed()){
    dxl.setGoalVelocity(DXL_ID_1, 29, UNIT_RPM);
  }else{
    dxl.setGoalVelocity(DXL_ID_1, 0, UNIT_RPM);
  }
  */

  float M1 = 0, M2 = 0,M3 = 0;
  
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
}
