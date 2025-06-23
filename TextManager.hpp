#pragma once

#include "hmain.hpp"

class TextManager : public MemoryManager
{
    protected:
    public:
    TextManager(TextManager&)=delete;
    TextManager() : MemoryManager(TEXT_BUFFER_SIZE)
    {

    }
};