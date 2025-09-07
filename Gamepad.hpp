#pragma once
#include "hmain.hpp"

class Gamepad
{
    public:
    static const int QUEUE_SIZE = 32;

    static const uint8_t CENTER = 0;

    static const uint8_t RIGHT_UP = 1;
    static const uint8_t RIGHT_LEFT = 2;
    static const uint8_t RIGHT_RIGHT = 3;
    static const uint8_t RIGHT_DOWN = 4;

    static const uint8_t RIGHT_TRIGGER = 5;

    static const uint8_t LEFT_UP = 6;
    static const uint8_t LEFT_LEFT = 7;
    static const uint8_t LEFT_RIGHT = 8;
    static const uint8_t LEFT_DOWN = 9;

    static const uint8_t LEFT_TRIGGER = 10;
    static const int N_BUTTONS = 11;
    private:
    protected:
    bool ButtonStates[N_BUTTONS] = {0};
    std::function<void(bool)> OnStateChangedHandlers[N_BUTTONS] = {0};
    std::function<void()> OnButtonDownHandlers[N_BUTTONS] = {0};
    std::function<void()> OnButtonUpHandlers[N_BUTTONS] = {0};
    std::function<void(uint8_t, bool)> GeneralHandler = nullptr;
    std::string id;
    FIFO<uint16_t, QUEUE_SIZE> update_queue;
    uint16_t copy_queue[QUEUE_SIZE] = {0};
    critical_section_t cs;
    bool HandleButtonUpdate(uint16_t data)
    {
        uint8_t state = data >> 8;
        uint8_t button = data & 0x00FF;
        if(state != 1 && state != 2 || button >= N_BUTTONS)
        {
            printf("Got an invalid button update %i %i\n", (int)state, (int)button);
            return false;
        }
        bool isdown = state == 1;
        if(ButtonStates[button] != isdown)
        {
            ButtonStates[button] = isdown;
            if(isdown)
            {
                if(OnButtonDownHandlers[button] != nullptr)
                    OnButtonDownHandlers[button]();
            }
            else
            {
                if(OnButtonUpHandlers[button] != nullptr)
                    OnButtonUpHandlers[button]();
            }
            if(OnStateChangedHandlers[button] != nullptr)
                OnStateChangedHandlers[button](isdown);
            if(GeneralHandler != nullptr)
                GeneralHandler(button, isdown);
        }
        return true;
    }
    public:
    std::string GetID()
    {
        return id;
    }
    bool IsButtonDown(uint8_t button)
    {
        assert(button < N_BUTTONS);
        return ButtonStates[button];
    }
    void SetOnButtonDownHandle(uint8_t button, std::function<void()> f)
    {
        assert(button < N_BUTTONS);
        OnButtonDownHandlers[button] = f;
    }
    void SetOnButtonUpHandle(uint8_t button, std::function<void()> f)
    {
        assert(button < N_BUTTONS);
        OnButtonUpHandlers[button] = f;
    }
    void SetOnButtonStateChangedHandle(uint8_t button, std::function<void(bool)> f)
    {
        assert(button < N_BUTTONS);
        OnStateChangedHandlers[button] = f;
    }
    void SetGeneralHandler(std::function<void(uint8_t, bool)> f)
    {
        GeneralHandler = f;
    }
    void QueueButtonUpdate(uint16_t value)
    {
        update_queue.push(value);
    }
    void ProcessButtonUpdates()
    {
        critical_section_enter_blocking(&cs);
        int i = 0;
        int sz = update_queue.size();
        while(update_queue.size() > 0)
        {
            copy_queue[i++] = update_queue.pop();
        }
        critical_section_exit(&cs);
        for(i = 0; i < sz; i++)
            HandleButtonUpdate(copy_queue[i]);
    }
    void ClearHandlers()
    {
        for(int i = 0; i < N_BUTTONS; i++)
            OnButtonUpHandlers[i] = nullptr;
        for(int i = 0; i < N_BUTTONS; i++)
            OnButtonDownHandlers[i] = nullptr;
        for(int i = 0; i < N_BUTTONS; i++)
            OnStateChangedHandlers[i] = nullptr;
        GeneralHandler = nullptr;
        for(int i = 0; i < N_BUTTONS; i++)
            ButtonStates[i] = false;
        critical_section_enter_blocking(&cs);
        update_queue.popall();
        critical_section_exit(&cs);
    }
    Gamepad(std::string id)
    {
        critical_section_init(&cs);
        this->id = id;
    }
    ~Gamepad()
    {
        critical_section_deinit(&cs);
        ClearHandlers();
    }
};