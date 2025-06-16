#include "hmain.hpp"

#include "Music/returnof.h"

static int sm;
static auto pio = pio0;

static mutex_t audiomx;
void LockAudio()
{
    mutex_enter_blocking(&audiomx);
}
void UnLockAudio()
{
    mutex_exit(&audiomx);
}

void SetDAC(uint8_t value)
{
    pio_sm_put_blocking(pio, sm, value);
}
void AudioStep()
{
    uint64_t start = get_time_us();
    mutex_enter_blocking(&audiomx);
    int value = 0;
    for(int i = 0; i < ArraySize(AudioChannels); i++)
    {
        if(AudioChannels[i] == nullptr)
            continue;
        auto chan = AudioChannels[i];
        chan->Lock();
        if(chan->IsEnabled())
        {
            value += chan->GetNextSample(AUDIO_SAMPLE_RATE);
        }
        chan->UnLock();
    }
    mutex_exit(&audiomx);
    if(value > 255)
        value = 255;
    LastAudioProcessingTime = get_time_us()-start;
    SetDAC(value);
}
static void dac_program_init(PIO pio, uint sm, uint offset, uint pin_base, float divider) 
{
    for(uint i=pin_base; i<pin_base+8; i++) 
    {
        pio_gpio_init(pio, i);
    }
    pio_sm_set_consecutive_pindirs(pio, sm, pin_base, 8, true);

    pio_sm_config c = dac_out_program_get_default_config(offset); 

    sm_config_set_out_shift(&c, true, true, 8); // true - shift right, auto pull, # of bits

    sm_config_set_out_pins(&c, pin_base, 8);

    sm_config_set_clkdiv(&c, divider);

    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
    pio_sm_clear_fifos(pio, sm);
}
void AudioLoop()
{
    mutex_init(&audiomx);

    sm = pio_claim_unused_sm(pio, true);
    uint8_t offset = pio_add_program(pio, &dac_out_program);
    float div = float(sysclockkhz*1000.0f)/(AUDIO_SAMPLE_RATE);
    dac_program_init(pio, sm, offset, PIN::DAC_OUT[0], div);

    auto ch00 = new VariableToneChannel(500.0f, 1000.0f, 0.5f, true);
    auto ch01 = new VariableToneChannel(1000.0f, 500.0f, 0.5f, true);
    auto ch02 = new VariableToneChannel(500.0f, 2000.0f, 0.5f, true);
    auto ch03 = new VariableToneChannel(2000.0f, 500.0f, 0.5f, true);

    auto ch0 = new MultiChannel({ch00, ch01, ch02, ch03}, true);
    ch0->SetEnabled(true);
    ch0->SetVolume(1.0f);
    AudioChannels[0] = ch0;

    auto ch1 = new StreamChannel((uint8_t*)__returnof_wav + 44, 11025, ArraySize(__returnof_wav) - 44, 1.0f, true);
    ch1->SetEnabled(true);
    AudioChannels[1] = ch1;


    while(true)
    {
        AudioStep();
    }
}