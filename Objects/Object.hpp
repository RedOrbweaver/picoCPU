#pragma once

#include "hmain.hpp"


class Object
{
    protected:
    uint8_t index;
    ENTITY_TYPE type;
    public:
    uint8_t GetIndex()
    {
        return index;
    }
    ENTITY_TYPE GetEntityType()
    {
        return type;
    }
    virtual void Detach()
    {
        
    }
};