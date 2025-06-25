#pragma once

#include "hmain.hpp"

class Circle : public Shape
{
    public:
    uint8_t GetBorder()
    {
        return data[1];
    }
    void SetBorder(uint8_t value)
    {
        data[1] = value;
        manager->WriteEntityData(index, data+1, 1, 1);
    }
    uint8_t GetFill()
    {
        return data[2];
    }
    void SetFill(uint8_t value)
    {
        data[2] = value;
        manager->WriteEntityData(index, data+2, 2, 1);
    }
    bool GetCentered()
    {
        return data[3];
    }
    void SetCentered(bool value)
    {
        data[3] = value;
        manager->WriteEntityData(index, data+3, 3, 1);
    }
    Circle(EntityManager* manager, uint8_t border, uint8_t fill, bool center, bool visible = false, uint8_t layer = 0, 
        uint8_t rotation = 0, vec2<int> position = {0, 0}, vec2<int> size = {0, 0}) 
        : Shape(manager, SHAPE::CIRCLE, visible, layer, rotation, position, size)
    {
        this->data[1] = border;
        this->data[2] = fill;
        this->data[3] = center;
        manager->WriteEntityData(index, data+1, 1, 3);
    }
};