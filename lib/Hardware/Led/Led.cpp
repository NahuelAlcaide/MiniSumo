#include <Arduino.h>
#include "Led.h"
#include "Config.h"

//================================================================================
// Internal State
//================================================================================
static unsigned long led_last_update = 0;
static int led_step = 0;
static bool led_active = false;
static int led_repeat = 0;
static int led_max_repeat = 0;

// Use enum for pattern types
enum LedPattern {
    LED_PATTERN_NONE = 0,
    LED_PATTERN_QUICK_BLINK,
    LED_PATTERN_DEBUG_MENU,
    LED_PATTERN_MAIN_MENU,
    // Add more patterns here
};
static LedPattern led_pattern = LED_PATTERN_NONE;

// Pattern definitions (ms)
constexpr int LED_BLINK_FAST_ON   = 60;
constexpr int LED_BLINK_FAST_OFF  = 100;
constexpr int LED_BLINK_SLOW_ON   = 250;
constexpr int LED_BLINK_SLOW_OFF  = 250;

//================================================================================
// Pattern Handlers
//================================================================================
void handleQuickBlink(unsigned long now) {
    switch (led_step) {
        case 0:
            digitalWrite(DEBUG_LED_PIN, HIGH);
            led_last_update = now;
            led_step = 1;
            break;
        case 1:
            if (now - led_last_update >= LED_BLINK_FAST_ON) {
                digitalWrite(DEBUG_LED_PIN, LOW);
                led_last_update = now;
                led_step = 2;
            }
            break;
        case 2:
            if (now - led_last_update >= LED_BLINK_FAST_OFF) {
                led_step = 0;
                led_repeat++;
                if (led_repeat >= led_max_repeat) led_active = false;
            }
            break;
    }
}

void handleDebugBlink(unsigned long now) {
    switch (led_step) {
        case 0:
            digitalWrite(DEBUG_LED_PIN, HIGH);
            led_last_update = now;
            led_step = 1;
            break;
        case 1:
            if (now - led_last_update >= LED_BLINK_SLOW_ON) {
                digitalWrite(DEBUG_LED_PIN, LOW);
                led_last_update = now;
                led_step = 2;
            }
            break;
        case 2:
            if (now - led_last_update >= LED_BLINK_SLOW_OFF) {
                led_step = 0;
                led_repeat++;
                if (led_repeat >= led_max_repeat) led_active = false;
            }
            break;
    }
}

void handleMainMenuBlink(unsigned long now) {
    switch (led_step) {
        case 0:
            digitalWrite(DEBUG_LED_PIN, HIGH);
            led_last_update = now;
            led_step = 1;
            break;
        case 1:
            if (now - led_last_update >= LED_BLINK_SLOW_ON) {
                digitalWrite(DEBUG_LED_PIN, LOW);
                led_last_update = now;
                led_step = 2;
            }
            break;
        case 2:
            if (now - led_last_update >= LED_BLINK_SLOW_OFF) {
                led_step = 0;
                led_repeat++;
                if (led_repeat >= led_max_repeat) led_active = false;
            }
            break;
    }
}

//================================================================================
// API Functions
//================================================================================
void ledSetup() {
    pinMode(DEBUG_LED_PIN, OUTPUT);
    digitalWrite(DEBUG_LED_PIN, LOW);
}

void ledUpdate() {
    if (!led_active) return;
    unsigned long now = millis();
    switch (led_pattern) {
        case LED_PATTERN_QUICK_BLINK:
            handleQuickBlink(now);
            break;
        case LED_PATTERN_DEBUG_MENU:
            handleDebugBlink(now);
            break;
        case LED_PATTERN_MAIN_MENU:
            handleMainMenuBlink(now);
            break;
        default:
            break;
    }
}

// Trigger a quick blink (e.g., remote command processed)
void ledBlinkQuick(int repeatCount) {
    led_pattern = LED_PATTERN_QUICK_BLINK;
    led_step = 0;
    led_repeat = 0;
    led_max_repeat = repeatCount;
    led_active = true;
    led_last_update = millis();
}

// Trigger two slow blinks (e.g., entering debug mode)
void ledBlinkDebugMode() {
    led_pattern = LED_PATTERN_DEBUG_MENU;
    led_step = 0;
    led_repeat = 0;
    led_max_repeat = 2;
    led_active = true;
    led_last_update = millis();
}

void ledMainMenu() {
    led_pattern = LED_PATTERN_MAIN_MENU;
    led_step = 0;
    led_repeat = 0;
    led_max_repeat = 1;
    led_active = true;
    led_last_update = millis();
}

void ledBattleMenu() {
    led_pattern = LED_PATTERN_MAIN_MENU;
    led_step = 0;
    led_repeat = 0;
    led_max_repeat = 3;
    led_active = true;
    led_last_update = millis();
}

void ledStrategyMenu() {
    led_pattern = LED_PATTERN_MAIN_MENU;
    led_step = 0;
    led_repeat = 0;
    led_max_repeat = 4;
    led_active = true;
    led_last_update = millis();
}

