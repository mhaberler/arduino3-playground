#ifdef TEST_SPDLOG
#if defined(M5UNIFIED)
#include <M5Unified.h>
#else
#include <Arduino.h>
#endif
#include <fmt/core.h>
#include <array>
#include <numeric>

#include "spdlog/spdlog.h"
#include "spdlog/fmt/bin_to_hex.h"
#include "spdlog/sinks/stdout_color_sinks.h"


const char *boardName(void);

auto main_log = spdlog::stdout_color_mt("main", spdlog::color_mode::always);

void test_spdlog(void)
{
    spdlog::set_level(spdlog::level::trace); // Set global log level to trace
    spdlog::set_pattern("[%C-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] %v");

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

    main_log->info("board type: {}", boardName());
}
#else
void test_spdlog(void) {}
#endif