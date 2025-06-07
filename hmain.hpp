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

#include "../picoGPU/shared.hpp"

namespace PIN
{
    uint8_t DAC_OUT[8] = {2, 3, 4, 5, 6, 7, 8, 9};
    uint8_t SPI_RX = 16;
    uint8_t SPI_CS = 17;
    uint8_t SPI_SCK = 18;
    uint8_t SPI_TX = 19;
}