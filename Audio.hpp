#pragma once
#include "hmain.hpp"

constexpr int AUDIO_SAMPLE_RATE = 22050;
constexpr int N_AUDIO_CHANNELS = 8;

class AudioChannel
{
    protected:

    mutex_t mutex;
    bool enabled = false;
    bool looping = false;
    float volume = 1.0f;

    public:

    void SetVolume(float val)
    {
        volume = val;
    }
    float GetVolume()
    {
        return volume;
    }
    bool IsEnabled()
    {
        return enabled;
    }
    void SetEnabled(bool value)
    {
        enabled = value;
    }
    bool GetLooping()
    {
        return looping;
    }
    void SetLooping(bool value)
    {
        looping = value;
    }
    virtual void Reset()=0;
    virtual uint8_t GetNextSample(int samplerate)=0;
    void Lock()
    {
        mutex_enter_blocking(&mutex);
    }
    void UnLock()
    {
        mutex_exit(&mutex);
    }

    AudioChannel(const AudioChannel&)=delete;

    AudioChannel()
    {
        mutex_init(&mutex);
        volume = 1.0f;
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
    virtual void Reset() override
    {
        sample_position = 0.0f;
    }
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
        float v = data[(int)sample_position] * GetVolume();
        if(v > 255.0f)
            v = 255.0f;
        return (uint8_t)v;
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
    float GetLength()
    {
        return length;
    }
    void SetLength(float value)
    {
        length = value;
        if(position > length)
        {
            position = 0;
            if(!GetLooping())
                enabled = false;
        }
    }
    float GetPosition()
    {
        return position;
    }
    void SetPosition(float value)
    {
        assert(position < length);
        position = value;
    }
    void SetTone(float tone)
    {
        this->tone = tone;
    }
    virtual float GetTone()
    {
        return tone;
    }
    virtual void Reset() override
    {
        position = 0.0f;
    }
    virtual uint8_t GetNextSample(int samplerate) override
    {
        uint8_t val = ((sin(2.0f*M_PI*GetPosition()*GetTone())*127.5f) + 127.5f)*GetVolume();

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
class VariableToneChannel : public ToneChannel
{
    protected:
    float final_tone;
    public:
    virtual float GetTone() override
    {
        return tone + (final_tone-tone)*(position/length);
    }
    VariableToneChannel(float start_tone, float finaltone, float len, bool looping) : ToneChannel(start_tone, len, looping)
    {
        this->final_tone = finaltone;
    }
};

class MultiChannel : public AudioChannel
{
    protected:
    std::vector<ToneChannel*> channels;
    int current_channel;
    public:
    virtual void Reset() override
    {
        current_channel = 0;
        for(auto it : channels)
        {
            it->Reset();
            it->SetLooping(false);
            it->SetEnabled(true);
        }
    }
    virtual uint8_t GetNextSample(int samplerate) override
    {
        auto chan = channels[current_channel];
        chan->SetVolume(this->GetVolume());
        assert(chan->IsEnabled());
        uint8_t ret = chan->GetNextSample(samplerate);
        if(!chan->IsEnabled())
        {
            current_channel++;
            if(current_channel == channels.size())
            {
                Reset();
                if(!GetLooping())
                    SetEnabled(false);
            }
        }
        return ret;
    }
    MultiChannel(std::initializer_list<ToneChannel*> channels, bool looping)
    {
        this->channels = std::vector<ToneChannel*>(channels);
        assert(this->channels.size() > 0);
        this->current_channel = 0;
        for(auto it : channels)
        {
            it->SetLooping(false);
            it->SetEnabled(true);
        }
        SetLooping(looping);
    }
};

inline AudioChannel* AudioChannels[N_AUDIO_CHANNELS];
inline uint64_t LastAudioProcessingTime;


void LockAudio();
void UnLockAudio();

void AudioLoop();