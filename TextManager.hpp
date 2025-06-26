#pragma once

#include "hmain.hpp"

class TextManager : public MemoryManager
{
    public:
    typedef std::function<void(int newpos)> OnMoveSegmentFunction;
    protected:
    GPU* gpu;
    void SendData(const char* data, int pos, int len)
    {
        assert(pos >= 0 && pos+len <= UINT16_MAX);
        gpu->I2CWriteMultipleBlocks(SOURCE::TEXT_BUFFER, pos, len, (uint8_t*)data);
    }
    public:
    
    virtual void MoveSegment(shared_ptr<Segment> segment, int newpos, int oldpos) override
    {
        assert(segment->data != nullptr);
        OnMoveSegmentFunction* func = (OnMoveSegmentFunction*)segment->data;
        (*func)(newpos);
    }
    virtual void WriteToSegment(shared_ptr<Segment> segment, uint8_t* data, int pos, int len) override
    {
        assert(pos == 0);
        SendData((const char*)data, segment->GetPosition(), len);
    }
    TextManager(TextManager&)=delete;
    TextManager(GPU* gpu, int length) : MemoryManager(length)
    {
        this->gpu = gpu;
    }
};