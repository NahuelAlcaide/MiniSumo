#include <Arduino.h>
#include "config.h"
#include "sensors.h"
#include "debug.h"
#include "motors.h"
#include "search.h"


void sensorReadout() {
    SensorData readings = readAllSensors();
    
    Serial.print("Sen I: "); Serial.print(readings.left);
    Serial.print(" Sen M: "); Serial.print(readings.center);
    Serial.print(" Sen D: "); Serial.print(readings.right);
    Serial.print(" izquierda: "); Serial.print(readings.lineLeft);
    Serial.print(" derecha: "); Serial.print(readings.lineRight);
    Serial.print(" Ruido I: "); Serial.print(analogRead(LEFT_SENSOR_PIN));
    Serial.print(" Ruido M: "); Serial.print(analogRead(CENTER_SENSOR_PIN));
    Serial.print(" Ruido D: "); Serial.println(analogRead(RIGHT_SENSOR_PIN));
}

bool sensorReadoutToggle = false;
bool controlTestToggle = false;

void debug() {
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        if (input.startsWith("SD,")) {
        input = input.substring(3); // Remove prefix
        int commaIndex = input.indexOf(',');
        int dirPow = 0;
        float turn = 0.0;

        if (commaIndex != -1) {
            dirPow = input.substring(0, commaIndex).toInt();
            turn = input.substring(commaIndex + 1).toFloat();
            sameDirection(dirPow, turn);
        } else {
            Serial.println("Invalid input for SD. Format: SD,<dirPow>,<turn>");
        }
        } else if (input.startsWith("OD,")) {
            input = input.substring(3); // Remove prefix
            int dirPow = input.toInt();
            oppositeDirection(dirPow);
        } else if (input.startsWith("DB")) {
            sensorReadoutToggle = !sensorReadoutToggle;
        } else if (input.startsWith("CT")) {
            controlTestToggle = !controlTestToggle;
        } else {
            Serial.println("Invalid command. Use SD,<dirPow>,<turn> - OD,<dirPow> or DB");
        }
    }
    if(sensorReadoutToggle){
        sensorReadout();
    }
    if(controlTestToggle){
        controlTest();
    }
}