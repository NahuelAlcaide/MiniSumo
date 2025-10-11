#include "config.h"
#include <Arduino.h>

#include "Motors/MotorController/MotorController.h"

void MotorController::motorSetup() {
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);
    pinMode(PWM_RIGHT, OUTPUT);
    pinMode(PWM_LEFT, OUTPUT);
}

void MotorController::limpMotors() {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
    analogWrite(PWM_RIGHT, 0);
    analogWrite(PWM_LEFT, 0);
    Serial.println("limp");
}


void MotorController::sameDirection(int dirPow, float turn) {
    if (dirPow > 0) {
        digitalWrite(AIN1, HIGH);
        digitalWrite(AIN2, LOW);
        digitalWrite(BIN1, HIGH);
        digitalWrite(BIN2, LOW);
    } else {
        digitalWrite(AIN1, LOW);
        digitalWrite(AIN2, HIGH);
        digitalWrite(BIN1, LOW);
        digitalWrite(BIN2, HIGH);
    }

    if (turn > 0) {
        analogWrite(PWM_RIGHT, abs(dirPow) * abs(turn)); 
        analogWrite(PWM_LEFT, abs(dirPow)); 
    } else if (turn < 0) {
        analogWrite(PWM_RIGHT, abs(dirPow)); 
        analogWrite(PWM_LEFT, abs(dirPow) * abs(turn)); 
    } else {
        analogWrite(PWM_RIGHT, abs(dirPow)); 
        analogWrite(PWM_LEFT, abs(dirPow)); 
    }
}

void MotorController::oppositeDirection(int dirPow){
    if(dirPow > 0){
        digitalWrite(AIN1, LOW);
        digitalWrite(AIN2, HIGH);
        digitalWrite(BIN1, HIGH);
        digitalWrite(BIN2, LOW);
        analogWrite(PWM_RIGHT, abs(dirPow));
        analogWrite(PWM_LEFT, abs(dirPow));
    } else {
        digitalWrite(AIN1, HIGH);
        digitalWrite(AIN2, LOW);
        digitalWrite(BIN1, LOW);
        digitalWrite(BIN2, HIGH);
        analogWrite(PWM_RIGHT, abs(dirPow));
        analogWrite(PWM_LEFT, abs(dirPow));
    }
}

void MotorController::brake() {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, HIGH);
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWM_RIGHT, 255);
    analogWrite(PWM_LEFT,255);
}

