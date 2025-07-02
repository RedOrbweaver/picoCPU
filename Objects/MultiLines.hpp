#pragma once

#include "hmain.hpp"

class MultiLines : public Object, public Geometric
{
    public:
    uint8_t GetColor()
    {
        return data[0];
    }
    void SetColor(uint8_t value)
    {
        data[0] = value;
        manager->WriteEntityData(index, data, 0, 1);
    }
    virtual void SetStartEnd(int start, int end) override
    {
        uint16_t s = start;
        uint16_t e = end;
        memcpy(data+1, &s, 2);
        memcpy(data+3, &e, 2);
        manager->WriteEntityData(index, data+1, 1, 4);
    }
    MultiLines(EntityManager* manager, GeometryManager* geometry_manager, uint8_t color, shared_ptr<GeometryManager::Geometry> geometry, bool visible=false, uint8_t layer=0, uint8_t rotation=0, vec2<int> position = {0,0})
        : Object(manager, ENTITY_TYPE::MULTI_LINES, visible, layer, rotation, position, {1,1})
        , Geometric(geometry_manager, 2)
    {
        SetColor(color);
        assert(geometry->points.size() % 2 == 0);
        SetGeometry(geometry);
    }
};