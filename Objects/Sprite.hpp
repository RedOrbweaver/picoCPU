#pragma once

#include "hmain.hpp"

class Sprite : public Object
{
    protected:
    shared_ptr<Texture> texture;
    void SetTexturePosition(uint32_t start, uint32_t end)
    {
        memcpy(data, &start, 4);
        memcpy(data+4, &end, 4);
        manager->WriteEntityData(index, data, 0, 8);
    }
    public:
    bool GetCentered()
    {
        return data[8];
    }
    void SetCentered(bool center)
    {
        data[8] = center;
        manager->WriteEntityData(index, data+8, 8, 1);
    }
    bool GetUseTransparency()
    {
        return data[9];
    }
    void SetUseTransparency(bool value)
    {
        data[9] = value;
        manager->WriteEntityData(index, data+9, 9, 1);
    }
    uint8_t GetTransparencyValue()
    {
        return data[10];
    }
    void SetTransparencyValue(uint8_t value)
    {
        data[10] = value;
        manager->WriteEntityData(index, data+10, 10, 1);
    }
    shared_ptr<Texture> GetTexture()
    {
        return texture;
    }
    void SetTexture(shared_ptr<Texture> texture)
    {
        auto seg = texture->GetSegment();
        SetTexturePosition(seg->GetPosition(), seg->GetPosition() + seg->GetLength()-1);
    }
    Sprite(EntityManager* manager, shared_ptr<Texture> texture, bool center, bool usetransparency, 
        uint8_t transparency_value, vec2<int> pos, bool visible, uint8_t layer)
        : Object(manager, ENTITY_TYPE::SPRITE, visible, layer, 0, pos, texture->GetSize())
    {
        SetTexture(texture);
        SetCentered(center);
        SetUseTransparency(usetransparency);
        SetTransparencyValue(transparency_value);
    }
};