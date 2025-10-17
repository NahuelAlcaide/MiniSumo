#include "StrategyDebug.h"

#include <Arduino.h>

#include "Strategies/IStrategy.h"
#include "Motors/IMotorController.h"
#include "Sensors/Sensors.h"

// Define the structure of the binary packet.
// #pragma pack ensures the compiler doesn't add padding bytes,
// which is crucial for consistent parsing on the Python side.
#pragma pack(push, 1)
struct DebugPacket {
    const uint8_t startByte = 0x7E; // Start of Frame Delimiter '~'
    uint8_t strategyId;
    uint8_t strategyState;
    int16_t leftSensor;
    int16_t centerSensor;
    int16_t rightSensor;
    int16_t lineLeftSensor;
    int16_t lineRightSensor;
    MotorCommand motorCommand;
    int16_t motorDirPow;
    float motorTurn;
    uint8_t motorLeftSpeed;
    uint8_t motorRightSpeed;
    uint8_t checksum;
};
#pragma pack(pop)

/**
 * @brief Calculates a simple XOR checksum over a block of data.
 * @param data Pointer to the start of the data.
 * @param len The number of bytes to include in the checksum.
 * @return The calculated 8-bit checksum.
 */
static uint8_t calculateChecksum(const uint8_t* data, size_t len) {
    uint8_t checksum = 0;
    for (size_t i = 0; i < len; ++i) {
        checksum ^= data[i];
    }
    return checksum;
}

void sendRawDebugPacket(uint8_t strategyId, IStrategy* strategy, const SensorData& sensors, IMotorController* motors) {
    if (!strategy || !motors) {
        return; // Safety check
    }

    DebugPacket packet;
    const MotorDebugInfo& motorInfo = motors->getDebugInfo();

    // Populate the packet with the latest data
    packet.strategyId = strategyId;
    packet.strategyState = strategy->getCurrentStateId();
    packet.leftSensor = sensors.left;
    packet.centerSensor = sensors.center;
    packet.rightSensor = sensors.right;
    packet.lineLeftSensor = sensors.lineLeft;
    packet.lineRightSensor = sensors.lineRight;
    packet.motorCommand = motorInfo.lastCommand;
    packet.motorDirPow = motorInfo.param_dirPow;
    packet.motorTurn = motorInfo.param_turn;
    packet.motorLeftSpeed = motorInfo.final_left_speed;
    packet.motorRightSpeed = motorInfo.final_right_speed;

    // Calculate checksum on the payload (all fields except startByte and checksum)
    uint8_t* packetBytes = reinterpret_cast<uint8_t*>(&packet);
    packet.checksum = calculateChecksum(packetBytes + 1, sizeof(packet) - 2);

    // Send the entire packet struct as raw bytes
    Serial.write(packetBytes, sizeof(packet));
}
