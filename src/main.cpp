#include <Arduino.h>
#include <IRremote.hpp>

#include "config.h"

#include "debug.h"
#include "sensors.h"
#include "motors.h"
#include "remote.h"

void setup() {
  Serial.begin(115200);

  sensorSetup();
  motorSetup();
  remoteSetup(); 
}

bool test = false;

void loop() {
  debug();
  uint16_t remoteData = remoteTest();
  if (remoteData == 0x1) {
    Serial.println(remoteData);
    test = !test;
    digitalWrite(DEBUG_LED_PIN, test ? HIGH : LOW);
    remoteData = 0;
  }
}