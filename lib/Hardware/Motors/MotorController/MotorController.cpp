    #include "Config.h"
#include <Arduino.h>

#include "Motors/MotorController/MotorController.h"

const MotorDebugInfo& MotorController::getDebugInfo() const {
    return m_debugInfo;
}

void MotorController::updateDebugValues(MotorCommand command, int dirPow, float turn, uint8_t leftSpeed, uint8_t rightSpeed) {
    m_debugInfo.lastCommand = command;
    m_debugInfo.param_dirPow = static_cast<int16_t>(dirPow);
    m_debugInfo.param_turn = turn;
    m_debugInfo.final_left_speed = leftSpeed;
    m_debugInfo.final_right_speed = rightSpeed;
}

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

    
    updateDebugValues(
        MotorCommand::LIMP,
        0,
        0.0f,
        0,
        0
    );
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

    uint8_t rightSpeed, leftSpeed;
    if (turn > 0) {
        rightSpeed = static_cast<uint8_t>(abs(dirPow) * (1 - fabs(turn)));
        leftSpeed = static_cast<uint8_t>(abs(dirPow));
    } else if (turn < 0) {
        rightSpeed = static_cast<uint8_t>(abs(dirPow));
        leftSpeed = static_cast<uint8_t>(abs(dirPow) * (1 - fabs(turn)));
    } else {
        rightSpeed = static_cast<uint8_t>(abs(dirPow));
        leftSpeed = static_cast<uint8_t>(abs(dirPow));
    }
    analogWrite(PWM_RIGHT, rightSpeed);
    analogWrite(PWM_LEFT, leftSpeed);

    updateDebugValues(
        MotorCommand::SAME_DIRECTION,
        dirPow,
        turn,
        leftSpeed,
        rightSpeed
    );
}

void MotorController::oppositeDirection(int dirPow){
    if(dirPow > 0){
        digitalWrite(AIN1, LOW);
        digitalWrite(AIN2, HIGH);
        digitalWrite(BIN1, HIGH);
        digitalWrite(BIN2, LOW);
    } else {
        digitalWrite(AIN1, HIGH);
        digitalWrite(AIN2, LOW);
        digitalWrite(BIN1, LOW);
        digitalWrite(BIN2, HIGH);
    }
    uint8_t speed = abs(dirPow);
    analogWrite(PWM_RIGHT, speed);
    analogWrite(PWM_LEFT, speed);

    updateDebugValues(
        MotorCommand::OPPOSITE_DIRECTION,
        dirPow,
        0.0f,
        speed,
        speed
    );
}

void MotorController::brake() {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, HIGH);
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWM_RIGHT, 255);
    analogWrite(PWM_LEFT,255);

    updateDebugValues(
        MotorCommand::BRAKE,
        0,
        0.0f,
        255,
        255
    );
}

