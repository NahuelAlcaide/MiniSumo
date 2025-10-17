#pragma once
#include <stdint.h>

/**
 * @brief Enum debug para comandos del controllador de motores.
 */
enum class MotorCommand : uint8_t {
    NONE,
    LIMP,
    SAME_DIRECTION,
    OPPOSITE_DIRECTION,
    BRAKE
};

/**
 * @brief Struct con info de debug del controlador de motores.
 */
struct MotorDebugInfo {
    MotorCommand lastCommand = MotorCommand::NONE;
    int16_t param_dirPow = 0;
    float param_turn = 0.0f;
    uint8_t final_left_speed = 0;
    uint8_t final_right_speed = 0;
};

class IMotorController {
public:
    virtual ~IMotorController() = default;

    virtual void motorSetup();
    virtual void sameDirection(int power, float diff) = 0;
    virtual void oppositeDirection(int power) = 0;
    virtual void brake();
    virtual void limpMotors();

    /**
     * @brief Gets the last executed command and its resulting state.
     * @return A const reference to the motor debug info struct.
     */
    virtual const MotorDebugInfo& getDebugInfo() const = 0;
};