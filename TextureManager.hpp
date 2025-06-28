#pragma once

#include "hmain.hpp"
class Texture;
class TextureManager : protected MemoryManager
{
    friend class Texture;
    struct PACK TGAHeader
    {
        char  idlength;
        char  colourmaptype;
        char  datatypecode;
        short int colourmaporigin;
        short int colourmaplength;
        char  colourmapdepth;
        short int x_origin;
        short int y_origin;
        short width;
        short height;
        char  bitsperpixel;
        char  imagedescriptor;
    };
    int buffer_size;
    protected:
    GPU* gpu;
    void SendData(uint8_t* data, int pos, int len)
    {
        assert(pos >= 0 && pos+len <= 2*UINT16_MAX);
        int i = pos;
        int tpos = 0;
        while(i < pos+len)
        {
            auto source = (i > UINT16_MAX) ? SOURCE::TEXTURE1 : SOURCE::TEXTURE0;
            int towrite = std::min(len-i, 255);
            gpu->I2CWrite(source, (source == SOURCE::TEXTURE1) ? i - UINT16_MAX : i, towrite, data + tpos);
            tpos += towrite;
            i += towrite;
        }
    }
    virtual void MoveSegment(shared_ptr<Segment> segment, int newpos, int oldpos) override;
    virtual void WriteToSegment(shared_ptr<Segment> segment, uint8_t* data, int pos, int len) override;
    void AddTexture(shared_ptr<Texture> texture);
    public:
    shared_ptr<Texture> CreateTexture(vec2<int> size, uint8_t* data)
    {
        auto tex = std::make_shared<Texture>(this, size, data);
        AddTexture(tex);
        return tex;
    }
    shared_ptr<Texture> CreateTextureFromTGA(uint8_t* data, int filelen)
    {
        TGAHeader header;
        memcpy(&header, data, sizeof(TGAHeader));

        if(header.datatypecode != 3 && header.datatypecode != 11)
            throw UnsupportedTGAFileException();
        shared_ptr<Texture> tex;

        vec2<int> size = {header.width, header.height};
        int len = size.area();
        if(header.datatypecode == 3)
        {
            tex = std::make_shared<Texture>(this, size, data + sizeof(TGAHeader) + header.idlength);
        }
        else if(header.datatypecode == 11)
        {
            uint8_t* imgdata = new uint8_t[len];
            volatile int pos = 0;
            volatile int i = sizeof(TGAHeader) + header.idlength;
            while(pos < len)
            {
                volatile int n = (data[i] & 0b01111111) + 1;
                if(data[i] & 0b10000000) // Run-length packet
                {
                    i++;
                    int val = data[i];
                    for(int ii = 0; ii < n; ii++)
                    {
                        imgdata[pos] = val;
                        pos++;
                    }
                    i++;
                }
                else // Raw packet
                {
                    i++;
                    for(volatile int ii = 0; ii < n; ii++)
                    {
                        if(i >= filelen)
                            throw InvalidTGAFileException();
                        imgdata[pos] = data[i];
                        pos++;
                        i++;
                    }
                }
            }
            if(pos != len)
                throw InvalidTGAFileException();
            tex = std::make_shared<Texture>(this, size, imgdata);
            delete[] imgdata;
        }
        else 
            assert(false);

        AddTexture(tex);
        return tex;
        return nullptr;
    }
    TextureManager(TextureManager&)=delete;
    TextureManager(GPU* gpu, uint32_t size) : MemoryManager(size)
    {
        this->gpu = gpu;
        this->buffer_size = size;
        assert(size <= 2*UINT16_MAX);

        gpu->I2CWrite(SOURCE::TEXTURE_BUFFER_SIZE, 0, 4, (uint8_t*)&size);
    }
};