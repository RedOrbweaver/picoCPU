#pragma once

#include "hmain.hpp"

class Texture
{
    friend class TextureManager;
    protected:
    uint8_t* data;
    vec2<int> size;
    shared_ptr<TextureManager::Segment> segment;
    TextureManager* texture_manager;
    public:
    shared_ptr<TextureManager::Segment> GetSegment()
    {
        return segment;
    }
    vec2<int> GetSize()
    {
        return size;
    }
    void SetPixel(vec2<int> pos, uint8_t value)
    {
        int p = pos.y*size.x + pos.x;
        int len = size.x*size.y;
        assert(len >= p);
        data[p] = value;
    }
    uint8_t GetPixel(vec2<int> pos)
    {
        int p = pos.y*size.x + pos.x;
        int len = size.x*size.y;
        assert(len >= p);
        return data[p];
    }
    void SetPixels(uint8_t* data)
    {
        int len = size.x*size.y;
        for(int i = 0; i < len; i++)
        {
            this->data[i] = data[i];
        }
    }
    Texture(TextureManager* manager, vec2<int> size, uint8_t* data)
    {
        this->texture_manager = manager;
        this->size = size;
        int len = size.x*size.y;
        this->data = new uint8_t[len];
        for(int i = 0; i < len; i++)
        {
            this->data[i] = data[i];
        }
    }
    virtual ~Texture()
    {
        delete[] data;
        texture_manager->DeAllocateSegment(segment);
    }
};