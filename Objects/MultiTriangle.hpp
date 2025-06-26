#pragma once

#include "hmain.hpp"

class MultiTriangle : public Shape, public Geometric
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
    bool GetCentered()
    {
        return data[6];
    }
    void SetCentered(bool value)
    {
        data[6] = value;
        manager->WriteEntityData(index, data+6, 6, 1);
    }
    MultiTriangle(EntityManager* manager, GeometryManager* gmanager, uint8_t fill, std::vector<vec2<int>> points, bool center, bool visible = false, uint8_t layer = 0, 
        uint8_t rotation = 0, vec2<int> position = {0, 0}, vec2<int> size = {0, 0}) 
        : Shape(manager, SHAPE::MULTI_TRIANGLE, visible, layer, rotation, position, size)
        , Geometric(gmanager, 3)
    {
        SetFill(fill);
        SetCentered(center);
        SetGeometry(points);
    }
};