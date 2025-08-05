//
#include "parse_govee_payload.hpp"
#include <cstdint>
#include <format>
#include <iostream>
#include <optional>
#include <string>
#include <span>

std::optional<
    std::tuple< double                      // temperature
                , double                    // humidity
                , int                       // battery
                , std::string_view          // variant
                , bool                      // valid
                >
    >
parse_govee_payload( std::span< const uint8_t > payload )
{
    if ( payload.size() < 8
         || payload[0] != 0x88
         || payload[1] != 0xec )
        return {};

    const uint8_t type_id = payload[2];

    const uint16_t t_le = payload[3] | (payload[4] << 8);
    const uint16_t h_le = payload[5] | (payload[6] << 8);
    const uint8_t  batt = payload[7];

    const uint16_t t_be = (payload[3] << 8) | payload[4];
    const uint16_t h_be = (payload[5] << 8) | payload[6];

    struct Variant {
        std::string_view name;
        double temp, hum;
        int battery;
    };

    std::array<Variant, 4> candidates = {{
        { "LE",        t_le / 100.0,      h_le / 100.0,      batt },
        { "LE+2000",  (t_le + 2000) / 100.0, h_le / 100.0,   batt },
        { "BE",        t_be / 100.0,      h_be / 100.0,      batt },
        { "Swapped",   h_le / 100.0,      t_le / 100.0,      batt }
    }};

    for (const auto& [name, temp, hum, battery] : candidates) {
        if (temp > 10.0 && temp < 45.0 && hum >= 10.0 && hum <= 100.0) {
            return {{ temp, hum, battery, name, true }};
        }
    }

    return {};
}


#if 0
bool
parse_govee_payload(const uint8_t* data, size_t len)
{
    if (len < 10) return false; // need at least AD header + 8 bytes

    // AD type check
    if (data[0] != 0xFF || data[1] != 0x88 || data[2] != 0xEC)
        return false;

    if (len < 11) return false; // need 8 more bytes: ID + 3×2 + battery

    uint8_t device_type = data[3];
    uint16_t temp_raw = data[4] | (data[5] << 8);
    uint16_t hum_raw  = data[6] | (data[7] << 8);
    uint8_t battery = data[8];

    double temperature = temp_raw / 100.0;
    double humidity = hum_raw / 100.0;

    std::cout << std::format("Govee [type 0x{:02x}] - Temp: {:.2f} °C, Humidity: {:.2f} %, Battery: {}%\n",
                             device_type, temperature, humidity, battery);
    return true;
}
#endif
