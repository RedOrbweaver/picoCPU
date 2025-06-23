#pragma once

#include "hmain.hpp"

class TextureManager : public MemoryManager
{
    protected:
    public:
    TextureManager(TextureManager&)=delete;
    TextureManager() : MemoryManager(0)
    {

    }
};