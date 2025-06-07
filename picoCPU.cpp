#include "hmain.hpp"


void I2CWrite(SOURCE source, uint8_t address0, uint8_t address1, uint8_t len, uint8_t* data)
{
    
    uint8_t buffer[255+5];
    buffer[0] = 0xFF; // WRITE
    buffer[1] = (uint8_t)source;
    buffer[2] = address0;
    buffer[3] = address1;
    buffer[4] = len;
    memcpy(buffer + 5, data, len);
    i2c_write_blocking(i2c0, I2C_ADDR, buffer, len + 5, false);
    
}

int main()
{
    stdio_init_all();

    printf("Setting sysclock to %i khz\n", sysclockkhz);
    set_sys_clock_khz(sysclockkhz, true);
    uart_set_baudrate(uart0, 115200);
    printf("clock set successfully\n");

    i2c_init(i2c0, 2 * 1000 * 1000);
    gpio_set_function(PIN::I2C_SCL, GPIO_FUNC_I2C);
    gpio_set_pulls(PIN::I2C_SCL, true, false);
    gpio_set_function(PIN::I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_pulls(PIN::I2C_SDA, true, false);


    int lines_x = 408;
    int lines_y = 304;

    float sx = 10.0f;
    float sy = 10.0f;
    float px = 50.00f;
    float py = 50.0f;
    float r = 15;

    Entity ball;
    ball.visible = true;
    ball.type = ENTITY_TYPE::SHAPE;
    ball.layer = 0;
    ball.size = vec2<uint16_t>{uint16_t(r), uint16_t(r)};
    ball.data[0] = (uint8_t)SHAPE::CIRCLE;
    ball.data[1] = 128;
    ball.data[2] = 0;

    while(true)
    {
        uint64_t ttm = get_time_us();
        float nx = px + sx;
        float ny = py + sy;
        if(nx < r || nx > float(lines_x)-2*r)
        {
            sx = -sx;
        }
        else
            px = nx;
        if(ny < 2*r || ny > float(lines_y)-r)
        {
            sy = -sy;
        }
        else
            py = ny;

        ball.pos = vec2<uint16_t>{(uint16_t)px, (uint16_t)py};
        ball.rotation += 1;
        I2CWrite(SOURCE::ENTITY_BUFFER, 0, 0, sizeof(Entity), (uint8_t*)&ball);
    }
}
