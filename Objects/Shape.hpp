#pragma once

#include "hmain.hpp"

class Shape : public Object
{
    public:
    SHAPE GetShape()
    {
        return (SHAPE)data[0];
    }
    void SetShape(SHAPE shape)
    {
        data[0] = (uint8_t)shape;
        manager->WriteEntityData(index, data, 0, 1);
    }
    Shape(EntityManager* manager, SHAPE shape,  bool visible = false, uint8_t layer = 0, uint8_t rotation = 0, 
        vec2<int> position = {0, 0}, vec2<int> size = {0, 0}) 
        : Object(manager, ENTITY_TYPE::SHAPE, visible, layer, rotation, position, size)
    {
        data[0] = (uint8_t)shape;
        manager->WriteEntityData(index, (uint8_t*)&data, 0, 1);
    }
};