#pragma once

#include "hmain.hpp"
class Geometric;
class GeometryManager : protected MemoryManager
{
    public:
    struct Geometry
    {
        GeometryManager* manager;
        vector<vec2<int>> points;
        shared_ptr<Segment> segment;
        vector<Geometric*> attached = {};
        ~Geometry()
        {
            manager->DeAllocateSegment(segment);
        }
    };
    friend struct Geometry;
    protected:
    GPU* gpu;
    void SendData(vec2<int>* data, int pos, int len)
    {
        assert(pos >= 0 && pos+len <= UINT16_MAX);
        gpu->I2CWriteMultipleBlocks(SOURCE::GEOMETRY, pos*sizeof(vec2<int>), len*sizeof(vec2<int>), (uint8_t*)data);
    }
    public:
    
    virtual void MoveSegment(shared_ptr<Segment> segment, int newpos, int oldpos) override;
    virtual void WriteToSegment(shared_ptr<Segment> segment, uint8_t* data, int pos, int len) override
    {
        assert(pos+len <= segment->GetLength());
        SendData((vec2<int>*)data, segment->GetPosition() + pos, len);
    }
    void AttachToGeometry(shared_ptr<Geometry> geometry, Geometric* geometric);
    void DetachFromGeometry(shared_ptr<Geometry> geometry, Geometric* geometric);
    shared_ptr<Geometry> AllocateGeomentry(vector<vec2<int>> points)
    {
        auto seg = AllocateSegment(points.size());
        WriteToSegment(seg, (uint8_t*)&points[0], 0, points.size());
        shared_ptr<Geometry> geometry = std::make_shared<Geometry>();
        geometry->points = points;
        geometry->segment = seg;
        geometry->manager = this;
        seg->data = geometry.get();
        return geometry;
    }
    GeometryManager(TextManager&)=delete;
    GeometryManager(GPU* gpu, int length) : MemoryManager(length)
    {
        this->gpu = gpu;
    }
};