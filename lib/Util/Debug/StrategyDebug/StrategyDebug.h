#pragma once

#include <stdint.h>

// Forward declarations
class IStrategy;
class IMotorController;
struct SensorData;

/**
 * @brief Populates and sends a binary debug packet over Serial.
 *
 * @param strategyId The ID of the active strategy.
 * @param strategy A pointer to the active strategy.
 * @param sensors A const reference to the latest sensor data.
 * @param motors A pointer to the active motor controller.
 */
void sendRawDebugPacket(uint8_t strategyId, IStrategy* strategy, const SensorData& sensors, IMotorController* motors);
