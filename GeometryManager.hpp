#pragma once

#include "hmain.hpp"

class GeometryManager : public MemoryManager
{
    public:
    typedef std::function<void(int newpos)> OnMoveSegmentFunction;
    protected:
    GPU* gpu;
    void SendData(vec2<int>* data, int pos, int len)
    {
        assert(pos >= 0 && pos+len <= UINT16_MAX);
        gpu->I2CWriteMultipleBlocks(SOURCE::GEOMETRY, pos*sizeof(vec2<int>), len*sizeof(vec2<int>), (uint8_t*)data);
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
        assert(pos+len <= segment->GetLength());
        SendData((vec2<int>*)data, segment->GetPosition() + pos, len);
    }
    GeometryManager(TextManager&)=delete;
    GeometryManager(GPU* gpu, int length) : MemoryManager(length)
    {
        this->gpu = gpu;
    }
};