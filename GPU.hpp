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
    void I2CWrite(SOURCE source, uint8_t address0, uint8_t address1, uint8_t len, uint8_t* data)
    {
        assert(len > 0);
        write_buffer[0] = 0xFF; // WRITE
        write_buffer[1] = (uint8_t)source;
        write_buffer[2] = address0;
        write_buffer[3] = address1;
        write_buffer[4] = len;
        memcpy(write_buffer + 5, data, len);
        int err = i2c_write_blocking(i2c, address, write_buffer, len + 5, false);   
        if(err == PICO_ERROR_GENERIC)
            throw I2CWriteFailureException("Failed to write to I2C");
    }
    void I2CWrite(SOURCE source, uint16_t address, uint8_t len, uint8_t* data)
    {
        I2CWrite(source, address & 0x00FF, address >> 8, len, data);
    }
    void I2CWriteMultipleBlocks(SOURCE source, uint16_t address, int len, uint8_t* data)
    {
        assert((int)address + len <= UINT16_MAX);
        int pos = 0;
        while(pos < len)
        {
            int towrite = std::min(len-pos, 255);
            I2CWrite(source, address + pos, towrite, data + pos);
            pos += towrite;
        }
    }

    void I2CRead(SOURCE source, uint8_t address0, uint8_t address1, uint8_t len, uint8_t* data)
    {
        uint8_t buffer[5];
        buffer[0] = 0xEF; // READ
        buffer[1] = (uint8_t)source;
        buffer[2] = address0;
        buffer[3] = address1;
        buffer[4] = len;
        int err = i2c_write_blocking(i2c, address, buffer, sizeof(buffer), true);
        if (err == PICO_ERROR_GENERIC)
            throw I2CWriteFailureException("Failed to write in order to read from I2C");
        err = i2c_read_blocking(i2c, address, data, len, false);
        if (err == PICO_ERROR_GENERIC)
            throw I2CReadFailureException("Failed to read from I2C");
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
        sleep_ms(1);
    }
};