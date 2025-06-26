#pragma once

#include "hmain.hpp"

class Geometric
{
    protected:
    vector<vec2<int>> geometry;
    GeometryManager* geometry_manager;
    shared_ptr<GeometryManager::Segment> segment = nullptr;
    GeometryManager::OnMoveSegmentFunction movefunc;
    int minimum_points;
    public:
    virtual void SetStartEnd(int start, int end)=0;
    int GetGeometrySize()
    {
        return geometry.size();
    }
    vec2<int> GetPoint(int pos)
    {
        assert(pos >= 0 && pos < geometry.size());
        return geometry[pos];
    }
    void SetPoint(int pos, vec2<int> value)
    {
        assert(pos < geometry.size() && pos >= 0);
        geometry[pos] = value;
        geometry_manager->WriteToSegment(segment, (uint8_t*)&geometry[pos], pos, 1);
    }
    vector<vec2<int>> GetGeometry()
    {
        return geometry;
    }
    void SetGeometry(vector<vec2<int>> ngeometry)
    {
        assert(ngeometry.size() >= minimum_points);
        if(ngeometry.size() != geometry.size() || segment == nullptr)
        {
            if(segment != nullptr)
                geometry_manager->DeAllocateSegment(segment);
            segment = geometry_manager->AllocateSegment(ngeometry.size());
            movefunc = [&](int newpos)
            {
                geometry_manager->WriteToSegment(segment, (uint8_t*)&geometry[0], 0, geometry.size());
                SetStartEnd(segment->GetPosition(), segment->GetPosition() + segment->GetLength() - 1);
            };
            segment->data = &movefunc;
        }
        geometry = ngeometry;
        geometry_manager->WriteToSegment(segment, (uint8_t*)&geometry[0], 0, geometry.size());
        SetStartEnd(segment->GetPosition(), segment->GetPosition() + segment->GetLength() - 1);
    }
    Geometric(GeometryManager* geometry_manager, int minimum_points)
    {
        this->geometry_manager = geometry_manager;
        this->minimum_points = minimum_points;
    }
    virtual ~Geometric()
    {
        if(segment != nullptr)
            geometry_manager->DeAllocateSegment(segment);
    }
};