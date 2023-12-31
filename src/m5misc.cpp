#ifdef M5UNIFIED
#include <M5Unified.h>

const char *boardName(void)
{

    switch (M5.getBoard())
    {
#if defined(CONFIG_IDF_TARGET_ESP32S3)
    case m5::board_t::board_M5StackCoreS3:
        return "StackCoreS3";
        break;
    case m5::board_t::board_M5StampS3:
        return "StampS3";
        break;
    case m5::board_t::board_M5AtomS3U:
        return "ATOMS3U";
        break;
    case m5::board_t::board_M5AtomS3Lite:
        return "ATOMS3Lite";
        break;
    case m5::board_t::board_M5AtomS3:
        return "ATOMS3";
        break;
    case m5::board_t::board_M5Dial:
        return "Dial";
        break;
    case m5::board_t::board_M5DinMeter:
        return "DinMeter";
        break;
    case m5::board_t::board_M5Capsule:
        return "Capsule";
        break;
#elif defined(CONFIG_IDF_TARGET_ESP32C3)
    case m5::board_t::board_M5StampC3:
        return "StampC3";
        break;
    case m5::board_t::board_M5StampC3U:
        return "StampC3U";
        break;
#else
    case m5::board_t::board_M5Stack:
        return "Stack";
        break;
    case m5::board_t::board_M5StackCore2:
        return "StackCore2";
        break;
    case m5::board_t::board_M5StickC:
        return "StickC";
        break;
    case m5::board_t::board_M5StickCPlus:
        return "StickCPlus";
        break;
    case m5::board_t::board_M5StickCPlus2:
        return "StickCPlus";
        break;
    case m5::board_t::board_M5StackCoreInk:
        return "CoreInk";
        break;
    case m5::board_t::board_M5Paper:
        return "Paper";
        break;
    case m5::board_t::board_M5Tough:
        return "Tough";
        break;
    case m5::board_t::board_M5Station:
        return "Station";
        break;
    case m5::board_t::board_M5Atom:
        return "ATOM";
        break;
    case m5::board_t::board_M5AtomPsram:
        return "ATOM PSRAM";
        break;
    case m5::board_t::board_M5AtomU:
        return "ATOM U";
        break;
    case m5::board_t::board_M5TimerCam:
        return "TimerCamera";
        break;
    case m5::board_t::board_M5StampPico:
        return "StampPico";
        break;
#endif
    default:
        return "Who am I ?";
        break;
    }
}
#else
const char *boardName(void)
{
    return "unknown board (not M5Unified)";
}
#endif