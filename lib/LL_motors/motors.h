#ifndef MOTORS_H
#define MOTORS_H

void motorSetup();
void sameDirection(int dirPow, float turn);
void oppositeDirection(int dirPow);
void brake();
void limpMotors();

#endif // MOTORS_H