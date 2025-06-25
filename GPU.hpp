#pragma once

#include "hmain.hpp"

class GPU
{
    protected:
    i2c_inst_t* i2c;
    uint8_t SCL;
    uint8_t SDA;
    uint8_t address;
    uint8_t write_buffer[255+5];
    public:
    int I2CWrite(SOURCE source, uint8_t address0, uint8_t address1, uint8_t len, uint8_t* data)
    {
        write_buffer[0] = 0xFF; // WRITE
        write_buffer[1] = (uint8_t)source;
        write_buffer[2] = address0;
        write_buffer[3] = address1;
        write_buffer[4] = len;
        memcpy(write_buffer + 5, data, len);
        return i2c_write_blocking(i2c, address, write_buffer, len + 5, false);   
    }

    int I2CRead(SOURCE source, uint8_t address0, uint8_t address1, uint8_t len, uint8_t* data)
    {
        uint8_t buffer[5];
        buffer[0] = 0xEF; // READ
        buffer[1] = (uint8_t)source;
        buffer[2] = address0;
        buffer[3] = address1;
        buffer[4] = len;
        i2c_write_blocking(i2c, address, buffer, sizeof(buffer), true);
        return i2c_read_blocking(i2c, address, data, len, false);
    }

    Info ReadInfo()
    {
        Info info = {0};
        I2CRead(SOURCE::INFO, 5, 7, sizeof(Info), (uint8_t*)&info);
        return info;
    }

    uint8_t ReadTest()
    {
        uint8_t test = {0};
        I2CRead(SOURCE::TEST, 5, 7, 1, &test);
        return test;
    }

    GPU(GPU&)=delete;
    GPU(i2c_inst_t* i2c, uint8_t ADDRESS, uint8_t SCL, uint8_t SDA)
    {
        this->i2c = i2c;
        this->address = ADDRESS;
        this->SCL = SCL;
        this->SDA = SDA;
        i2c_init(i2c0, i2cspeed);
        gpio_set_function(SCL, GPIO_FUNC_I2C);
        gpio_set_pulls(SCL, true, false);
        gpio_set_function(SDA, GPIO_FUNC_I2C);
        gpio_set_pulls(SDA, true, false);
    }
};