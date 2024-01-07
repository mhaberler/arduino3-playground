#include "Sensor.hpp"

Equipment e;

void init_sensors(void) {
    TPMS *tpms = new TPMS();
    Mopeka *mopeka = new Mopeka();
    Unit *u = new Unit();
    u->add(tpms);
    u->add(mopeka);
    
    e.add(u);
}

void sensor_loop(void) {
    e.render();
}
