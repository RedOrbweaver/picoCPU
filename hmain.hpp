#pragma once
#include <stdio.h>
#include <cstring>
#include <string>

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "hardware/interp.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "hardware/uart.h"

#include "redpicolib/RedPicoLib.hpp"

#include "../picoGPU/shared.hpp"

namespace PIN
{
    constexpr uint8_t DAC_OUT[8] = {2, 3, 4, 5, 6, 7, 8, 9};
    constexpr uint8_t I2C_SCL = 17;
    constexpr uint8_t I2C_SDA = 16;
}

constexpr uint8_t I2C_ADDR = 0x11;