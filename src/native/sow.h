#pragma once

#ifndef __linux__
#error This module is only compatible with linux
#endif

#include <cstdint>

namespace __MODULE__{

enum class feature_t : uint8_t {DISABLED, ENABLED};
enum class stop_t : uint8_t  {ONE_BIT, TWO_BITS};
enum class bits_t  : uint8_t {FIVE, SIX, SEVEN, EIGHT};
enum class hardware_flow_t : uint8_t  {DISABLED, ENABLED};

struct cfg_serial {
    feature_t parity: 1 = feature_t::ENABLED;
    stop_t stop:1 = stop_t::ONE_BIT;
    bits_t bits:2 = bits_t::EIGHT;
    feature_t hardware_flow:1 = feature_t::DISABLED;

    uint8_t vtime = 0;
    uint8_t vmin = 0;

    uint32_t ispeed = 9600;
    uint32_t ospeed = 9600;
};


int configure(const char* path, const cfg_serial& cfg);

}