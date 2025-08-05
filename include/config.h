#ifndef CONFIG_H
#define CONFIG_H

#define IR_RECEIVE_PIN 11
#define DEBUG_LED_PIN 12

// Motores
// Motor derecho
#define AIN1 3 // Adelante
#define AIN2 2 // Atrás
#define PWM_RIGHT 10

// Motor izquierdo
#define BIN1 4 // Adelante
#define BIN2 5 // Atrás
#define PWM_LEFT 9

// Sensores de linea
#define LEFT_LINE_PIN A2
#define RIGHT_LINE_PIN A1

// Sensores frontales
#define RIGHT_SENSOR_PIN A5
#define CENTER_SENSOR_PIN A4
#define LEFT_SENSOR_PIN A3
#define RIGHT_LED_PIN 6
#define CENTER_LED_PIN 7
#define LEFT_LED_PIN 8

// Parámetros sensores frontales
#define IR_LED_COUNT 3
#define IR_MICROS_DELAY 100
#define IR_DUTY_CYCLE 0.05 // Porcentaje de tiempo que el led está prendido

#endif // CONFIG_H