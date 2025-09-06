#pragma once

struct Context
{
    unique_ptr<GPU> gpu;
    unique_ptr<EntityManager> entity_manager;
    unique_ptr<TextManager> text_manager;
    unique_ptr<GeometryManager> geometry_manager;
    unique_ptr<TextureManager> texture_manager;
};