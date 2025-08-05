#include "config.h"
#include <Arduino.h>
#include <QTRSensors.h>

#include "sensors.h"

QTRSensors linea;
uint16_t valoresLinea[2];

int pulse(int sensor, int led){
    int noise = analogRead(sensor);
    digitalWrite(led,HIGH);
    delayMicroseconds(IR_MICROS_DELAY);
    int reading = analogRead(sensor);
    digitalWrite(led,LOW);
    int denoised = reading - noise;
    return denoised;    
}

void sensorSetup() {
    pinMode(RIGHT_SENSOR_PIN, INPUT);
    pinMode(CENTER_SENSOR_PIN, INPUT);
    pinMode(LEFT_SENSOR_PIN, INPUT);
    pinMode(RIGHT_LED_PIN,OUTPUT);
    pinMode(CENTER_LED_PIN,OUTPUT);
    pinMode(LEFT_LED_PIN,OUTPUT);

    linea.setTypeRC();
    linea.setSensorPins((const uint8_t[]){LEFT_LINE_PIN,RIGHT_LINE_PIN}, 2);
}

SensorData readAllSensors() {
    SensorData data;

    // Sweep sensores frontales
    data.right = pulse(RIGHT_SENSOR_PIN, RIGHT_LED_PIN);
    data.center = pulse(CENTER_SENSOR_PIN, CENTER_LED_PIN);
    data.left = pulse(LEFT_SENSOR_PIN, LEFT_LED_PIN);

    // Delay de seguridad para mantener el duty cycle máximo permitido
    delayMicroseconds((IR_MICROS_DELAY / IR_DUTY_CYCLE) - (IR_MICROS_DELAY * IR_LED_COUNT));

    // Leer sensores de linea
    linea.read(valoresLinea);
    data.lineLeft = valoresLinea[0];
    data.lineRight = valoresLinea[1];

    return data;
}