#pragma once

#include "Motors/IMotorController.h"
#include <Arduino.h>

class Debug {
public:
    Debug();

    /**
     * @brief Main update loop, call this repeatedly.
     * Processes commands and runs active debug routines.
     */
    void update(uint16_t remoteCommand, IMotorController* motors);

    /**
     * @brief Resets all debug toggles to OFF.
     */
    void reset();

private:
    // --- State Flags ---
    bool m_sensorReadoutEnabled;
    bool m_controlTestEnabled;
    bool m_cleanWheelsEnabled;
    // Note: m_strategyDebugEnabled is removed!

    // --- Private Helper Functions ---

    /**
     * @brief Processes incoming remote commands to toggle debug modes.
     */
    void processRemoteCommands(uint16_t remoteCommand, IMotorController* motors);

    /**
     * @brief Active debug routine for testing motor control based on sensors.
     */
    void controlTest(IMotorController* motors);

    /**
     * @brief Stops motors when controlTest is disabled.
     */
    void stopMotors(IMotorController* motors);

    /**
     * @brief Turns the wheels at a constant low speed to assist cleaning them
     */
    void cleanWheels(IMotorController* motors);

    /**
     * @brief Toggles the line sensor inversion flag.
     */
    void invertLineSensors();
};

extern Debug g_debugModule;