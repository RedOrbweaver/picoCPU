#pragma once

#include "hmain.hpp"

class Line : public Object
{
    public:
    vec2<int> GetP0()
    {
        return GetPosition();
    }
    void SetP0(vec2<int> p)
    {
        SetPosition(p);
    }
    vec2<int> GetP1()
    {
        return GetSize();
    }
    void SetP1(vec2<int> p)
    {
        SetSize(p);
    }
    Line(EntityManager* manager, uint8_t color, vec2<int> p0, vec2<int> p1, bool visible = false, 
        uint8_t layer = 0, uint8_t rotation = 0) 
        : Object(manager, ENTITY_TYPE::LINE, visible, layer, rotation, p0, p1, &color, 1)
    {

    }
};