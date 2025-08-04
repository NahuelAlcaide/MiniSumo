#include <Arduino.h>
//#include <IRremote.h>

#include "debug.h"

#include "sensors.h"
#include "motors.h"

void setup() {
  Serial.begin(115200);
  //IrReceiver.begin(IR_RECEIVE_PIN);

  sensorSetup();
  motorSetup();
}

void loop() {
  debug();
}