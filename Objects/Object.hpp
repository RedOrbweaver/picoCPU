#pragma once

#include "hmain.hpp"


class Object
{
    protected:
    int index;
    EntityManager* manager;
    ENTITY_TYPE type;
    bool visible;
    uint8_t rotation;
    uint8_t layer;
    vec2<int> position;
    vec2<int> size;
    uint8_t data[ENTITY_DATA_SIZE];
    public:
    int GetIndex()
    {
        return index;
    }
    ENTITY_TYPE GetEntityType()
    {
        return type;
    }
    void SetEntityType(ENTITY_TYPE type)
    {
        this->type = type;
        manager->WriteEntityType(index, type);
    }
    bool GetVisible()
    {
        return visible;
    }
    void SetVisible(bool value)
    {
        this->visible = value;
        manager->WriteEntityVisible(index, value);
    }
    uint8_t GetRotation()
    {
        return rotation;
    }
    void SetRotation(uint8_t value)
    {
        this->rotation = value;
        manager->WriteEntityRotation(index, value);
    }
    uint8_t GetLayer()
    {
        return layer;
    }
    void SetLayer(uint8_t value)
    {
        this->layer = value;
        manager->WriteEntityLayer(index, value);
    }
    vec2<int> GetPosition()
    {
        return position;
    }
    void SetPosition(vec2<int> position)
    {
        this->position = position;
        manager->WriteEntityPosition(index, position.convert<uint16_t>());
    }
    vec2<int> GetSize()
    {
        return size;
    }
    void SetSize(vec2<int> size)
    {
        this->size = size;
        manager->WriteEntitySize(index, size.convert<uint16_t>());
    }
    void GetData(uint8_t* buffer, int len)
    {
        assert(len > 0 && len <= ENTITY_DATA_SIZE);
        memcpy(buffer, data, len);
    }
    void SetData(uint8_t* buffer, int start, int len)
    {
        assert(start >= 0);
        assert(len > 0 && (start+len) <= ENTITY_DATA_SIZE);
        memcpy(data + start, buffer, len);
        manager->WriteEntityData(index, buffer, start, len);
    }

    Object()=delete;
    Object(Object&)=delete;
    Object(EntityManager* manager, ENTITY_TYPE type, bool visible = false, uint8_t layer = 0, 
        uint8_t rotation = 0, vec2<int> position = {0, 0}, vec2<int> size = {0, 0}, 
        uint8_t* data = nullptr, int datalen = 0)
    {
        this->manager = manager;
        this->type = type;
        this->visible = visible;
        this->rotation = rotation;
        this->layer = layer;
        this->position = position;
        this->size = size;
        if(datalen > 0)
        {
            assert(datalen <= ENTITY_DATA_SIZE);
            memcpy(this->data, data, datalen);
        }
        Entity ent;
        ent.type = type;
        ent.visible = visible;
        ent.rotation = rotation;
        ent.layer = layer;
        ent.pos = position.convert<uint16_t>();
        ent.size = size.convert<uint16_t>();
        if(datalen > 0)
        {
            memcpy(ent.data, data, datalen);
        }
        index = manager->AddEntity(ent);
    }
    virtual ~Object()
    {
        manager->FreeEntity(index);
    }
};