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
#define IR_MICROS_DELAY 1000
#define IR_DUTY_CYCLE 0.01 // Porcentaje de tiempo que el led está prendido

// PARAMETROS DE BATALLA
#define SEEK_THRESHOLD 60
#define ATTACK_THRESHOLD 150
#define LINE_THRESHOLD 100

// COMANDOS CONTROL REMOTO

#define E_STOP 0x19 // Stop de emergencia
#define E_STOP_OFF 0xFF // Stop de emergencia desactivado

// Comandos para menu principal
#define REMOTE_CMD_DEBUG_MODE    0x11 // Menu debug
#define REMOTE_CMD_BATTLE_MODE   0x21 // Menu battle

// Comandos para debug menu
#define REMOTE_CMD_SENSOR_READOUT 0x1 // Toggle sensor readout
#define REMOTE_CMD_CONTROL_TEST  0x2 // Toggle control test

// Comandos para battle menu
#define REMOTE_CMD_BATTLE_START 0x20 // Iniciar secuencia de batalla

#endif // CONFIG_H