#ifndef LED_H
#define LED_H

#pragma once

void ledSetup();

void ledUpdate();

void ledBlinkQuick(int repeatCount = 1);

void ledBlinkDebugMode();

void ledMainMenu();

void ledBattleMenu();

#endif // LED_H
