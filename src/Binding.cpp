#include "Sensor.hpp"

bool Binding::link(Producer &p, Consumer &c) {}

bool Binding::link(Unit &src, Consumer &c) {}

bool Binding::link(Unit &src, Unit &dst) {}

bool Binding::link(actorType_t &src, actorType_t &to) {}

