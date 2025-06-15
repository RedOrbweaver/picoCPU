#pragma once
#include "hmain.hpp"

constexpr int audio_sample_rate = 44100;

class AudioChannel
{
    protected:

    mutex_t mutex;
    bool enabled;
    bool looping;

    public:

    virtual bool IsEnabled()
    {
        return enabled;
    }
    virtual void SetEnabled(bool value)
    {
        enabled = value;
    }
    virtual bool GetLooping()
    {
        return looping;
    }
    virtual void SetLooping(bool value)
    {
        looping = value;
    }
    virtual uint8_t GetNextSample(int samplerate)=0;
    virtual void Lock()
    {
        mutex_enter_blocking(&mutex);
    }
    virtual void UnLock()
    {
        mutex_exit(&mutex);
    }

    AudioChannel()
    {
        mutex_init(&mutex);
    }
    
};

class StreamChannel : public AudioChannel
{
    protected:
    uint8_t* data;
    int sample_rate;
    int nsamples;
    float sample_position;
    float speed;
    public:

    virtual uint8_t GetNextSample(int samplerate) override
    {
        float relsamplerate = (float(this->sample_rate)/float(samplerate))*speed;
        sample_position += relsamplerate;
        if(sample_position > nsamples-1)
        {
            sample_position = 0.0f;
            if(!GetLooping())
                SetEnabled(false);
        }
        return data[(int)sample_position];
    }

    StreamChannel(uint8_t* data, int samplerate, int nsamples, float speed, bool looping)
    {
        this->data = data;
        this->sample_rate = samplerate;
        this->sample_position = 0.0f;
        this->nsamples = nsamples;
        this->speed = speed;
        SetLooping(looping);
    }
};

class ToneChannel : public AudioChannel
{
    protected:
    float tone;
    float length;
    float position;
    public:
    virtual uint8_t GetNextSample(int samplerate) override
    {
        uint8_t val = (sin(2.0f*M_PI*position*tone)*127.5f) + 127.5f;

        position += 1.0f/float(samplerate);
        if(position > length)
        {
            position = 0.0f;
            if(!GetLooping())
                SetEnabled(false);
        }
        return val;
    }
    ToneChannel(float tone, float len, bool looping)
    {
        this->tone = tone;
        this->length = len;
        this->position = 0.0f;
        SetLooping(looping);
    }
};

inline AudioChannel* AudioChannels[8];


void LockAudio();
void UnLockAudio();

void AudioLoop();