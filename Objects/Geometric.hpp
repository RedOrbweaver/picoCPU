#pragma once

#include "hmain.hpp"

class Geometric
{
    protected:
    shared_ptr<GeometryManager::Geometry> geometry = nullptr;
    GeometryManager* geometry_manager;
    int minimum_points;
    public:
    virtual void SetStartEnd(int start, int end)=0;
    int GetGeometrySize()
    {
        return geometry->points.size();
    }
    vec2<int> GetPoint(int pos)
    {
        assert(pos >= 0 && pos < geometry->points.size());
        return geometry->points[pos];
    }
    vector<vec2<int>> GetGeometry()
    {
        return geometry->points;
    }
    void OnMoveFunc()
    {
        SetStartEnd(geometry->segment->GetPosition(), geometry->segment->GetPosition() + geometry->segment->GetLength() - 1);
    }
    void SetGeometry(shared_ptr<GeometryManager::Geometry> geometry)
    {
        if(this->geometry != nullptr)
            geometry_manager->DetachFromGeometry(this->geometry, this);
        this->geometry = geometry;
        geometry_manager->AttachToGeometry(this->geometry, this);
        SetStartEnd(geometry->segment->GetPosition(), geometry->segment->GetPosition() + geometry->segment->GetLength() - 1);
    }
    Geometric(GeometryManager* geometry_manager, int minimum_points)
    {
        this->geometry_manager = geometry_manager;
        this->minimum_points = minimum_points;
    }
    virtual ~Geometric()
    {
        geometry_manager->DetachFromGeometry(geometry, this);
    }
};