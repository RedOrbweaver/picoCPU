#pragma once

#include "hmain.hpp"

class Triangle : public Shape
{
    public:
    uint8_t GetFill()
    {
        return data[1];
    }
    void SetFill(uint8_t value)
    {
        data[1] = value;
        manager->WriteEntityData(index, data+1, 1, 1);
    }
    vec2<int> GetP0()
    {
        return (*(vec2<int16_t>*)(data + 2)).convert<int>();
    }
    void SetP0(vec2<int> p)
    {
        auto ps16 = p.convert<int16_t>();
        memcpy(data + 2, &ps16, 4);
    }
    vec2<int> GetP1()
    {
        return (*(vec2<int16_t>*)(data + 6)).convert<int>();
    }
    void SetP1(vec2<int> p)
    {
        auto ps16 = p.convert<int16_t>();
        memcpy(data + 6, &ps16, 4);
    }
    vec2<int> GetP2()
    {
        return (*(vec2<int16_t>*)(data + 10)).convert<int>();
    }
    void SetP2(vec2<int> p)
    {
        auto ps16 = p.convert<int16_t>();
        memcpy(data + 10, &ps16, 4);
    }
    bool GetCentered()
    {
        return data[15];
    }
    void SetCentered(bool value)
    {
        data[15] = value;
        manager->WriteEntityData(index, data+15, 15, 1);
    }
    Triangle(EntityManager* manager, uint8_t fill, vec2<int> p0, vec2<int> p1, vec2<int> p2, bool center, bool visible = false, uint8_t layer = 0, 
        uint8_t rotation = 0, vec2<int> position = {0, 0}, vec2<int> size = {0, 0}) 
        : Shape(manager, SHAPE::TRIANGLE, visible, layer, rotation, position, size)
    {
        this->data[1] = fill;
        auto p0s16 = p0.convert<int16_t>();
        memcpy(data + 2, &p0s16, 4);
        auto p1s16 = p1.convert<int16_t>();
        memcpy(data + 6, &p1s16, 4);
        auto p2s16 = p2.convert<int16_t>();
        memcpy(data + 10, &p2s16, 4);
        this->data[15] = center;
        manager->WriteEntityData(index, data+1, 1, 15);
    }
};