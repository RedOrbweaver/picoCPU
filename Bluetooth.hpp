#pragma once

#include "btstack.h"
#include "pico/cyw43_arch.h"

#ifndef ENABLE_BLE
#error Please link to pico_btstack_ble
#endif



#if 1
#define DEBUG_LOG(...) printf(__VA_ARGS__)
#else
#define DEBUG_LOG(...)
#endif

#define LED_QUICK_FLASH_DELAY_MS 100
#define LED_SLOW_FLASH_DELAY_MS 1000

int bt_init();