#pragma once

#include "hmain.hpp"

class GeometryManager : public MemoryManager
{
    protected:
    public:
    GeometryManager(GeometryManager&)=delete;
    GeometryManager() : MemoryManager(GEOMETRY_BUFFER_SIZE)
    {

    }
};