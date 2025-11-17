#pragma once
#include <Arduino.h>

/**
* @brief Struct que contiene las lecturas de todos los sensores.
 */
struct SensorData {
    int right;
    int center;
    int left;
    uint16_t lineLeft;
    uint16_t lineRight;
};

/**
 * @brief Configura los pines y la librería QTR para los sensores del robot.
 */
void sensorSetup();

/**
 * @brief Lee todos los sensores del robot usando un cooldown no bloqueante.
 *
 * Si se llama antes de que pase el periodo SENSOR_COOLDOWN_US, esta función
 * devolverá los datos de sensores previamente cacheados. De lo contrario,
 * realizará una nueva lectura y actualizará la caché.
 *
 * @return Un struct de tipo @ref SensorData con los valores más recientes de los sensores.
 */
SensorData readAllSensors();

extern bool INVERT_LINE;