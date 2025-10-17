#include "Config.h"
#include <Arduino.h>

#include "DummyMotorController.h"

const MotorDebugInfo& DummyMotorController::getDebugInfo() const {
    return m_debugInfo;
}

void DummyMotorController::updateDebugValues(MotorCommand command, int dirPow, float turn, uint8_t leftSpeed, uint8_t rightSpeed) {
    m_debugInfo.lastCommand = command;
    m_debugInfo.param_dirPow = static_cast<int16_t>(dirPow);
    m_debugInfo.param_turn = turn;
    m_debugInfo.final_left_speed = leftSpeed;
    m_debugInfo.final_right_speed = rightSpeed;
}

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

    updateDebugValues(
        MotorCommand::LIMP,
        0,
        0.0f,
        0,
        0
    );
}


void DummyMotorController::sameDirection(int dirPow, float turn) {

    uint8_t rightSpeed, leftSpeed;
    if (turn > 0) {
        rightSpeed = static_cast<uint8_t>(dirPow * (1 - fabs(turn)));
        leftSpeed = static_cast<uint8_t>(dirPow);
    } else if (turn < 0) {
        rightSpeed = static_cast<uint8_t>(dirPow);
        leftSpeed = static_cast<uint8_t>(dirPow * (1 - fabs(turn)));
    } else {
        rightSpeed = static_cast<uint8_t>(dirPow);
        leftSpeed = static_cast<uint8_t>(dirPow);
    }

    updateDebugValues(
        MotorCommand::SAME_DIRECTION,
        dirPow,
        turn,
        leftSpeed,
        rightSpeed
    );
}

void DummyMotorController::oppositeDirection(int dirPow){
    uint8_t speed = abs(dirPow);
    updateDebugValues(
        MotorCommand::OPPOSITE_DIRECTION,
        dirPow,
        0.0f,
        speed,
        speed
    );

}

void DummyMotorController::brake() {
    updateDebugValues(
        MotorCommand::BRAKE,
        0,
        0.0f,
        255,
        255
    );
}

