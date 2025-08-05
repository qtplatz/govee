
#pragma once

#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <tuple>

// bool parse_govee_payload(const uint8_t* data, size_t len);

std::optional< std::tuple< double                      // temperature
                           , double                    // humidity
                           , int                       // battery
                           , std::string_view          // variant
                           , bool                      // valid
                           > >
parse_govee_payload( std::span< const uint8_t > payload );
