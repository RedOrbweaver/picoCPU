#pragma once

#include "hmain.hpp"

class Program
{
    protected:
    Context* context;
    GPU* gpu;
    EntityManager* entity_manager;
    TextManager* text_manager;
    GeometryManager* geometry_manager;
    TextureManager* texture_manager;
    vec2<int> lines;
    public:
    Program* NextProgram = nullptr;
    virtual bool Tick(float delta)=0;
    virtual void Initialize()=0;
    void SetContext(Context* context)
    {
        this->context = context;
        this->gpu = context->gpu.get();
        this->entity_manager = context->entity_manager.get();
        this->text_manager = context->text_manager.get();
        this->geometry_manager = context->geometry_manager.get();
        this->texture_manager = context->texture_manager.get();
        auto info = gpu->ReadInfo();
        this->lines = {info.lines_x, info.lines_y};
    }
    virtual ~Program()
    {

    }
};