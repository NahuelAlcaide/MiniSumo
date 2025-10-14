#include "Config.h"
#include <Arduino.h>

#include "DummyMotorController.h"

void DummyMotorController::motorSetup() {
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);
    pinMode(PWM_RIGHT, OUTPUT);
    pinMode(PWM_LEFT, OUTPUT);
}

void DummyMotorController::limpMotors() {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
    analogWrite(PWM_RIGHT, 0);
    analogWrite(PWM_LEFT, 0);
}


void DummyMotorController::sameDirection(int dirPow, float turn) {

}

void DummyMotorController::oppositeDirection(int dirPow){

}

void DummyMotorController::brake() {

}

