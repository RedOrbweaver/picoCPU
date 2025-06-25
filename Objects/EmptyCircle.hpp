#pragma once

#include "hmain.hpp"

class EmptyCircle : public Shape
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
    EMPTY_CIRCLE_MODE GetMode()
    {
        return (EMPTY_CIRCLE_MODE)data[2];
    }
    void SetMode(EMPTY_CIRCLE_MODE mode)
    {
        data[2] = (uint8_t)mode;
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
    EmptyCircle(EntityManager* manager, uint8_t border, EMPTY_CIRCLE_MODE mode, bool center, bool visible = false, uint8_t layer = 0, 
        vec2<int> position = {0, 0}, vec2<int> size = {0, 0}) 
        : Shape(manager, SHAPE::EMPTY_CIRCLE, visible, layer, 0, position, size)
    {
        this->data[1] = border;
        this->data[2] = (uint8_t)mode;
        this->data[3] = center;
        manager->WriteEntityData(index, data+1, 1, 3);
    }
};