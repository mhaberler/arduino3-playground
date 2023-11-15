SET(SOURCES screens/ui_MAIN_PAGE.c
    screens/ui_ALT_Plot.c
    screens/ui_MENU.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c)

add_library(ui ${SOURCES})
