#pragma once

#include "ArduinoJson.h"

#define UI_MESSAGESIZE 8192

bool sendUiMessage(const JsonDocument &jdoc);
void sendUiMessage(const char * fmt, ...);


