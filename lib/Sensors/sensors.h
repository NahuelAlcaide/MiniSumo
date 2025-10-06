#ifndef SENSORS_H
#define SENSORS_H

struct SensorData {
    int right;
    int center;
    int left;
    int lineLeft;
    int lineRight;
};

void sensorSetup();
SensorData readAllSensors();

extern bool INVERT_LINE;

#endif // SENSORS_H