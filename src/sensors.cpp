#include "Sensor.hpp"

SensorSet sset;

void init_sensors(void) {
    Sensor *s = new Ruuvi();
    sset.add(s);
}

void sensor_loop(void) {
    sset.render();
}
