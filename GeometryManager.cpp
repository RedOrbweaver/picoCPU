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
            break;
        }
        i++;
    }
    assert(i != geometry->attached.size());
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