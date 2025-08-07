#pragma once
#include <Arduino.h>

// Call once in setup()
void ledSetup();

// Call every loop()
void ledUpdate();

// Call when a valid remote command is processed
void ledBlinkQuick();

// Call when entering debug mode
void ledBlinkDebugMode();

void ledMainMenu();
// You can add more custom blink pattern triggers as needed
