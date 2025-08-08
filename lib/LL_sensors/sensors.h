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

#endif // SENSORS_H