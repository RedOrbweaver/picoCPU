#pragma once

#include "hmain.hpp"

void GeometryManager::AttachToGeometry(shared_ptr<Geometry> geometry, Geometric* geometric)
{
    geometry->attached.push_back(geometric);
}
void GeometryManager::DetachFromGeometry(shared_ptr<Geometry> geometry, Geometric* geometric)
{
    int i = 0;
    while(i < geometry->attached.size())
    {
        if(geometry->attached[i] == geometric)
        {
            geometry->attached.erase(geometry->attached.begin() + i);
            return;
        }
        i++;
    }
    assert(false); // not found
}
void GeometryManager::MoveSegment(shared_ptr<Segment> segment, int newpos, int oldpos)
{
    assert(segment->data != nullptr);
    Geometry* geometry = (Geometry*)segment->data;
    WriteToSegment(geometry->segment, (uint8_t*)&geometry->points[0], 0, geometry->points.size());
    for(int i = 0; i < geometry->attached.size(); i++)
    {
        geometry->attached[i]->OnMoveFunc();
    }
}
void GeometryManager::Geometry::SetPoints(vector<vec2<int>> points)
{
    if(this->points.size() != points.size())
    {
        manager->DeAllocateSegment(segment);
        segment = manager->AllocateSegment(points.size());
        segment->data = this;
    }
    manager->WriteToSegment(segment, &points[0], 0, points.size());
    this->points = points;
    for(auto it : attached)
    {
        it->OnMoveFunc();
    }
}