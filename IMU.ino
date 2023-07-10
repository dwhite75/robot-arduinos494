#include "ICM_20948.h"
#include <Servo.h>

#define AD0_VAL 1
#define SERIAL_PORT Serial

ICM_20948_I2C myICM;

int bot_pin = 10;
int top_pin = 11;

Servo botServo;
Servo topServo;

const int botAngles[] = {110, 101, 92, 86, 76, 68, 60, 50};

void setup() {

  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(400000);
  myICM.enableDebugging();
  myICM.begin(Wire, AD0_VAL);
  Serial.print(F("Initialization of the sensor returned: "));
  Serial.println(myICM.statusString());

  botServo.attach(bot_pin);
  topServo.attach(top_pin);
}

void loop() {

  myICM.getAGMT();       

  int topAngle = map(myICM.agmt.acc.axes.x, -11000, 11000, 180, 0) - 90;
  int botAngle = map(myICM.agmt.acc.axes.y, -9000, 9000, 0, 15) / 2;
  
  if (botAngle < 0) {
    botAngle = 0;
  } else if (botAngle > 7) {
    botAngle = 7;
  }

  if (topAngle <= 70) {
    topAngle = 0;
  } else {
    topAngle = 90;
  }

  botServo.write(botAngles[botAngle]);

  topServo.write(topAngle);
  delay(175);
  topServo.write(0);
  delay(200);
}