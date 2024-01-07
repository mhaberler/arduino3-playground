#include "Sensor.hpp"

Equipment e;

void init_sensors(void) {
    Unit *u = new Unit();
    e.add(u);
}

void sensor_loop(void) {
    e.render();
}
