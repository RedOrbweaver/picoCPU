#pragma once

#include "hmain.hpp"

class MainMenuProgram : public Program
{
    protected:
    public:
    const static int PROG_LIST_LEN = 10;
    const static int DESC_LINE_MAX_CHARS = 25;
    int caret_pos = 0;
    int scroll = 0;
    shared_ptr<Text> gamepad_state_text;
    shared_ptr<Text> desc_text;
    shared_ptr<Text> prog_list[PROG_LIST_LEN];
    void OnSelect()
    {
        assert(NextProgram == nullptr);
        NextProgram = programs[caret_pos+scroll].generator();
    }
    void OnBack()
    {

    }
    void OnUp()
    {
        if(caret_pos == 0)
        {
            if(scroll > 0)
            {
                SetProgListScroll(scroll-1);
            }
        }
        else
            SetCaretPos(caret_pos-1);
    }
    void OnDown()
    {
        if(caret_pos+scroll == ArraySize(programs)-1)
            return;
        if(caret_pos == PROG_LIST_LEN-1)
        {
            SetProgListScroll(scroll+1);
        }
        else
            SetCaretPos(caret_pos+1);
    }
    void InitGamepad(shared_ptr<Gamepad> gamepad)
    {
        gamepad->SetOnButtonDownHandle(Gamepad::RIGHT_UP, [this]()
        {
            OnUp();
        });
        gamepad->SetOnButtonDownHandle(Gamepad::RIGHT_DOWN, [this]()
        {
            OnDown();
        });
        gamepad->SetOnButtonDownHandle(Gamepad::RIGHT_LEFT, [this]()
        {
            OnBack();
        });
        gamepad->SetOnButtonDownHandle(Gamepad::RIGHT_RIGHT, [this]()
        {
            OnSelect();
        });
    }
    void SetCaretPos(int pos)
    {
        assert(pos >= 0 && pos < PROG_LIST_LEN);
        std::string text = prog_list[caret_pos]->GetText();
        text[0] = ' ';
        prog_list[caret_pos]->SetText(text);
        text = prog_list[pos]->GetText();
        text[0] = '>';
        prog_list[pos]->SetText(text);
        caret_pos = pos;
    }
    void SetProgListScroll(int v)
    {
        scroll = v;
        for(int i = 0; i+v < ArraySize(programs) && i < PROG_LIST_LEN; i++)
        {
            prog_list[i]->SetText(((i == caret_pos) ? ">" : " ") + programs[i+v].name);
        }
    }
    virtual bool Tick(float delta) override 
    {
        if(NextProgram != nullptr)
        {
            return false;
        }
        auto gamepads = gamepad_manager.GetGamepads();
        if(gamepads.size() == 0)
        {
            gamepad_state_text->SetText("NO GAMEPADS CONNECTED");
        }
        else if(gamepads.size() == 1)
        {
            gamepad_state_text->SetText("GAMEPAD CONNECTED");
        }
        else
        {
            gamepad_state_text->SetText(std::to_string(gamepads.size()) + " GAMEPADS CONNECTED");
        }
        desc_text->SetText("(" + std::to_string(caret_pos+scroll+1) + "/" + std::to_string(ArraySize(programs)) +
            ") " + programs[caret_pos+scroll].description);
        return true;
    }
    virtual void Initialize() override
    {
        gamepad_state_text = std::make_shared<Text>(entity_manager, text_manager, "Gamepad State", 
        FONT::FIXED_10_20, TEXT_ALIGNMENT::LEFT, true, true, 0, int2{20, 20});
        desc_text = std::make_shared<Text>(entity_manager, text_manager, "(0/0) Description", 
        FONT::FIXED_7_14, TEXT_ALIGNMENT::LEFT, true, true, 0, int2{20, 44});
        int h = 64;
        for(int i = 0; i < PROG_LIST_LEN; i++)
        {
            prog_list[i] = std::make_shared<Text>(entity_manager, text_manager, "PROG " + std::to_string(i+1), 
                FONT::FIXED_7_14, TEXT_ALIGNMENT::LEFT, true, true, 0, int2{30, h});
            h += 17;
        }
        SetProgListScroll(0);
        gamepad_manager.AddOnGamepadAddedHandler([&](shared_ptr<Gamepad> gamepad)
        {
            InitGamepad(gamepad);
        });
        auto gamepads = gamepad_manager.GetGamepads();
        for(auto it : gamepads)
        {
            InitGamepad(it.second);
        }
    }
};