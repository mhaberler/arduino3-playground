#ifdef TEST_SPDLOG

#include <M5Unified.h>
#undef B1 // fmt workaround
#undef F
#include <fmt/core.h>
#include <array>
#include <numeric>

#include "spdlog/fmt/bin_to_hex.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

auto main_log = spdlog::stdout_color_mt("main", spdlog::color_mode::always);

extern "C"
{
    void fmtlib_error(const char *file, int line, const char *message)
    {
        Serial.printf("%s:%d: assertion failed: %s\n", file, line, message);
    }
}

void test_spdlog(void)
{
    spdlog::set_level(spdlog::level::trace); // Set global log level to trace
    spdlog::set_pattern("[%t] [%C-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] %v");

    main_log->trace("spdlog on ESP{}", 32);
    std::string s = fmt::format("The answer is {}.", 42);
    main_log->trace(s);

    std::string str{"ESP32"};
    main_log->debug("  |{:20}|", str);   // |ESP32               |
    main_log->info("   |{:<20}|", str);  // |ESP32               |
    main_log->warn("|{:_>20}|", str);    // |_______________ESP32|
    main_log->error("  |{:_^20}|", str); // |_______ESP32________|

    std::array<char, 64> a;
    std::iota(a.begin(), a.end(), 1);
    main_log->critical("{:s:X}", spdlog::to_hex(a));
}
#else
void test_spdlog(void) {}
#endif