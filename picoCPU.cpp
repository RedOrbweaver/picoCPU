#include "hmain.hpp"


void SPIWrite(SOURCE source, uint8_t address0, uint8_t address1, uint8_t len, uint8_t* data)
{
    uint8_t buffer[255+5];
    buffer[0] = 0xFF; // WRITE
    buffer[1] = (uint8_t)source;
    buffer[2] = address0;
    buffer[3] = address1;
    buffer[4] = len;
    memcpy(buffer + 5, data, len);
    spi_write_blocking(spi0, buffer, len + 5);
}

int main()
{
    stdio_init_all();

    printf("Setting sysclock to %i khz\n", sysclockkhz);
    set_sys_clock_khz(sysclockkhz, true);
    uart_set_baudrate(uart0, 115200);
    printf("clock set successfully\n");

    spi_init(spi0, 1000 * 100);
    gpio_set_function(PIN::SPI_RX, GPIO_FUNC_SPI);
    gpio_set_function(PIN::SPI_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN::SPI_TX, GPIO_FUNC_SPI);
    gpio_set_function(PIN::SPI_CS, GPIO_FUNC_SPI);

    uint8_t val;;
    while(true)
    {
        val++;
        std::string text = "this is a test!";
        SPIWrite(SOURCE::DEBUG_PRITNF, 1, 2, text.length(), (uint8_t*)text.c_str());
        sleep_ms(1000);
    }

    while (true) 
    {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
