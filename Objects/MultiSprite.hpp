#pragma once

class MultiSprite : public Sprite, public Geometric
{
    public:
    virtual void SetStartEnd(int start, int end) override
    {
        uint16_t s = start;
        uint16_t e = end;
        memcpy(data+1, &s, 2);
        memcpy(data+3, &e, 2);
        manager->WriteEntityData(index, data+1, 1, 4);
    }
    MultiSprite(EntityManager* manager, GeometryManager* geometry_manager, shared_ptr<Texture> texture, shared_ptr<GeometryManager::Geometry> geometry, bool center, bool usetransparency, 
        uint8_t transparency_value, vec2<int> pos, bool visible, uint8_t layer)
        : Sprite(manager, texture, center, usetransparency, transparency_value, pos, visible, layer, ENTITY_TYPE::MULTI_SPRITE)
        , Geometric(geometry_manager, 1)
    {
        SetGeometry(geometry);
    }
};