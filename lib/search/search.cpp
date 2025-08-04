#include "sensors.h"
#include "motors.h"

void controlTest(){
    SensorData data = readAllSensors();
    if (data.center < 70){
        int diff = static_cast<int>((data.right - data.left) * (90.0 / 1000.0));
        oppositeDirection(diff);
    } else {
        brake();
    }
}