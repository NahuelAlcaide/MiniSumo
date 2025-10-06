#include "config.h"
#include <Arduino.h>
#include <QTRSensors.h>

#include "sensors.h"

// --- Constants ---
// Define the minimum time in microseconds that must pass before reading the sensors again.
#define SENSOR_COOLDOWN_US ((IR_MICROS_DELAY / IR_DUTY_CYCLE) - (IR_MICROS_DELAY * IR_LED_COUNT))

// --- Global Variables ---
QTRSensors linea;
uint16_t valoresLinea[2];
bool INVERT_LINE = false;

/**
 * @brief Pulsa un LED IR específico y lee el valor del fototransistor asociado.
 *
 * @param sensor El pin analógico del fototransistor.
 * @param led El pin digital asociado al transistor que controla el LED IR.
 * @return El valor leído del fototransistor, con el ruido ambiental restado.
 */
int pulse(int sensor, int led) {
    // Leer el valor con el LED apagado para capturar el ruido ambiental
    int noise = analogRead(sensor);
    
    // Encender el LED IR
    digitalWrite(led, HIGH);
    // Esperar el tiempo necesario para que el LED influya en la lectura
    delayMicroseconds(IR_MICROS_DELAY);
    
    // Leer el valor con el LED encendido
    int reading = analogRead(sensor);
    
    // Apagar el LED IR
    digitalWrite(led, LOW);
    
    // Restar el ruido ambiente de la lectura
    int denoised = reading - noise;
    return denoised;
}

/**
 * @brief Configura los pines y la librería QTR para los sensores del robot.
 */
void sensorSetup() {
    // Pin modes para los fototransistores
    pinMode(RIGHT_SENSOR_PIN, INPUT);
    pinMode(CENTER_SENSOR_PIN, INPUT);
    pinMode(LEFT_SENSOR_PIN, INPUT);

    // Pin modes para los LEDs IR frontales
    pinMode(RIGHT_LED_PIN, OUTPUT);
    pinMode(CENTER_LED_PIN, OUTPUT);
    pinMode(LEFT_LED_PIN, OUTPUT);

    // Configuración de los QTR
    linea.setTypeRC();
    linea.setSensorPins((const uint8_t[]){LEFT_LINE_PIN, RIGHT_LINE_PIN}, 2);
}

/**
 * @brief Lee todos los sensores del robot usando un cooldown no bloqueante.
 *
 * Si se llama antes de que pase el periodo SENSOR_COOLDOWN_US, esta función
 * devolverá los datos de sensores previamente cacheados. De lo contrario,
 * realizará una nueva lectura y actualizará la caché.
 *
 * @return Un struct de tipo @ref SensorData con los valores más recientes de los sensores.
 */
SensorData readAllSensors() {
    // Las variables estáticas persisten su valor entre llamadas a la función.
    // Se usan para almacenar el último tiempo de lectura y los últimos valores leídos.
    static unsigned long lastReadTimestamp = 0;
    static SensorData lastValues;

    unsigned long currentTime = micros();

    // Verifica si ha pasado el periodo de cooldown desde la última lectura exitosa.
    // También maneja la primera ejecución donde lastReadTimestamp es 0.
    if (currentTime - lastReadTimestamp >= SENSOR_COOLDOWN_US) {
        // Ha pasado suficiente tiempo, se realiza una nueva lectura de sensores.
        
        // Actualiza el timestamp al tiempo actual.
        lastReadTimestamp = currentTime;

        // Lee los sensores de proximidad frontales.
        lastValues.right = pulse(RIGHT_SENSOR_PIN, RIGHT_LED_PIN);
        lastValues.center = pulse(CENTER_SENSOR_PIN, CENTER_LED_PIN) - 100;
        lastValues.left = pulse(LEFT_SENSOR_PIN, LEFT_LED_PIN);

        if (lastValues.center < 0) {
            lastValues.center = 0; // Evita valores negativos
        }

        // Lee los sensores de línea.
        linea.read(valoresLinea);
        
        if (INVERT_LINE) {
            valoresLinea[0] = static_cast<uint16_t>(map(valoresLinea[0], 2500, 0, 0, 2500));
            valoresLinea[1] = static_cast<uint16_t>(map(valoresLinea[1], 2500, 0, 0, 2500));
        }

        lastValues.lineLeft = valoresLinea[0];
        lastValues.lineRight = valoresLinea[1];
    }

    // Devuelve los últimos valores leídos (ya sean nuevos o en caché).
    return lastValues;
}
