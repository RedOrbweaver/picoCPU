#pragma once

#include "hmain.hpp"

class Program
{
    protected:
    shared_ptr<vector<shared_ptr<Object>>> objects;
    shared_ptr<vector<shared_ptr<AudioChannel>>> audio_channels;

    public:
    shared_ptr<vector<shared_ptr<Object>>> GetObjects()
    {
        return objects;
    }
    shared_ptr<vector<shared_ptr<AudioChannel>>> GetAudioChannels()
    {
        return audio_channels;
    }
    virtual void Enter()=0;
    virtual void Pause()=0;
    virtual void Exit()=0;
    virtual void Tick()=0;
    Program()
    {
        objects = std::make_shared<vector<shared_ptr<Object>>>();
        audio_channels = std::make_shared<vector<shared_ptr<AudioChannel>>>();
    }
};