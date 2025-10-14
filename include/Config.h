#pragma once

// ************************************************************
//                      DEFINICIONES DE PINES
// ************************************************************

// ============================================================
//                       MOTORES
// ============================================================

// -------------------- Motor derecho --------------------
#define AIN1 4 // Adelante
#define AIN2 5 // Atrás
#define PWM_RIGHT 9

// -------------------- Motor izquierdo --------------------
#define BIN1 3 // Adelante
#define BIN2 2 // Atrás
#define PWM_LEFT 10

// ============================================================
//                       SENSORES
// ============================================================

// -------------------- Sensores de línea --------------------
#define LEFT_LINE_PIN A2
#define RIGHT_LINE_PIN A1

// -------------------- Sensores frontales --------------------
#define RIGHT_SENSOR_PIN A5
#define CENTER_SENSOR_PIN A4
#define LEFT_SENSOR_PIN A3
#define RIGHT_LED_PIN 6
#define CENTER_LED_PIN 7
#define LEFT_LED_PIN 8

// ============================================================
//                       IR Y DEBUG
// ============================================================
#define IR_RECEIVE_PIN 11
#define DEBUG_LED_PIN 12

// ************************************************************
//                CONFIGURACIÓN Y PARÁMETROS
// ************************************************************

// -------------------- Parámetros sensores frontales --------------------
// 1000 micros con 0.01 de DC para más potencia
#define IR_LED_COUNT 3
#define IR_MICROS_DELAY 1000 // microsegundos
#define IR_DUTY_CYCLE 0.02 // Porcentaje de tiempo que el LED está prendido

// ============================================================
//                 PARÁMETROS DE BATALLA
// ============================================================

// -------------------- Initial Move --------------------
#define INIT_TURN_TIME 80 // ms, tiempo de giro derecha / izquierda
#define INIT_FORWARD_TURN_TIME 30 // ms, tiempo de giro diagonal adelante
#define INIT_BACKWARD_TURN_TIME 130 // ms, tiempo de giro diagonal atras

// -------------------- Blind search --------------------
#define SEARCH_SPEED 100
#define SEARCH_TURN_FACTOR 0.07f
#define SEARCH_TURN_INTERVAL 800 // ms

// -------------------- Defensive Blind search --------------------
#define DEFENSIVE_SEARCH_BRAKE_TIME 50 // ms
#define DEFENSIVE_SEARCH_TURN_TIME 80 // ms
#define DEFENSIVE_SEARCH_WAIT_TIME 1500 // ms

// -------------------- Seek --------------------
#define SEEK_THRESHOLD 60
#define SEEK_TURN_RATE 0.03f // Velocidad de giro al buscar
#define SEEK_SPEED 90 // Velocidad de avance al buscar
#define SEEK_TURN_TIMEOUT 1000 // ms

// -------------------- Attack --------------------
#define ATTACK_THRESHOLD 150
#define ATTACK_POWER_SCALE 190

// -------------------- Line Evade --------------------
#define LINE_EVADE_THRESHOLD 200 // Negro en 800 aprox, arranca a ver la linea en 200 con un mínimo de 50
// Timings de giro
#define LINE_EVADE_TURN_TIME 70 // ms
#define LINE_EVADE_TURN_RATE 0.1f // Velocidad de giro al evadir línea
#define LINE_EVADE_TURN_SPEED 220 // Velocidad de giro al evadir línea
// Timings de reversa
#define LINE_EVADE_REVERSE_TIME 200 // ms
#define LINE_EVADE_REVERSE_SPEED 220 // 0-255
#define LINE_EVADE_REVERSE_TURN_RATE 0.2f // Velocidad de giro al evadir línea en reversa
// Timings de frenos
#define LINE_EVADE_END_BRAKE_TIME 70 // ms
#define LINE_EVADE_START_BRAKE_TIME 70 // ms
// Panico
#define LINE_EVADE_PANIC_TIMER 500 // ms, tiempo máximo de giro antes de entrar en pánico
#define LINE_EVADE_PANIC_REVERSE_TIME 1500 // ms, tiempo de

// ============================================================
//               COMANDOS CONTROL REMOTO
// ============================================================

#define REMOTE_ADDRESS 0x8

#define E_STOP 0x19 // Stop de emergencia
#define E_STOP_OFF 0xFF // Stop de emergencia desactivado

// -------------------- Comandos para menú principal --------------------
#define REMOTE_CMD_DEBUG_MODE    0x11 // Menú debug
#define REMOTE_CMD_BATTLE_MODE   0x21 // Menú battle
#define REMOTE_CMD_STRATEGY_MENU 0x31 // Menú estrategia

// --------------- Comandos para strategy select menu ---------------
#define REMOTE_CMD_STRATEGY_1 0x1 // Estrategia 1 (Default)
#define REMOTE_CMD_STRATEGY_2 0x2 // Estrategia 2
#define REMOTE_CMD_STRATEGY_3 0x3 // Estrategia 3
#define REMOTE_CMD_STRATEGY_4 0x4 // Estrategia 4
#define REMOTE_CMD_STRATEGY_5 0x5 // Estrategia 5
#define REMOTE_CMD_STRATEGY_6 0x6 // Estrategia 6

// -------------------- Comandos para debug menu --------------------
#define REMOTE_CMD_SENSOR_READOUT 0x1 // Toggle sensor readout
#define REMOTE_CMD_CONTROL_TEST   0x2 // Toggle control test
#define REMOTE_CMD_INVERT_LINE    0x3 // Invertir sensores de línea
#define REMOTE_CMD_STRATEGY_DEBUG 0x4 // Debug de estrategia

// -------------------- Comandos para battle menu --------------------
#define REMOTE_CMD_BATTLE_START   0x20 // Iniciar secuencia de batalla
#define REMOTE_CMD_BATTLE_CONFIG  0x0A // Configurar batalla

#define REMOTE_CMD_UP         0x1 // D-Pad up
#define REMOTE_CMD_UP_RIGHT   0x2 // D-Pad up-right
#define REMOTE_CMD_RIGHT      0x3 // D-Pad right
#define REMOTE_CMD_DOWN_RIGHT 0x4 // D-Pad down-right
#define REMOTE_CMD_DOWN       0x5 // D-Pad down
#define REMOTE_CMD_DOWN_LEFT  0x6 // D-Pad down-left
#define REMOTE_CMD_LEFT       0x7 // D-Pad left
#define REMOTE_CMD_UP_LEFT    0x8 // D-Pad up-left
#define REMOTE_CMD_OK         0x9 // Botón OK