SET(SOURCES ui_Main.c
    ui_Compass.c
    ui_GPS.c
    ui_Parameters.c
    ui_Status.c
    ui_Ruuvi.c
    ui_Unit.c
    ui_Config.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_events.cpp)

add_library(ui ${SOURCES})
