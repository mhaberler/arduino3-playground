#include "Sensor.hpp"

bool Binding::link(Producer &p, Consumer &c) {
    return false;
}

bool Binding::link(Unit &src, Consumer &c) {
    return false;
}

bool Binding::link(Unit &src, Unit &dst)  {
    return false;
}

bool Binding::link(actorType_t &src, actorType_t &to) {
    return false;
}
