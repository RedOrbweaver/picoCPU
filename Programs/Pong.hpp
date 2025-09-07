#pragma once

class PongProgram : public Program
{
    vec4<int> play_area;
    vec2<int> play_area_size;
    vec2<int> play_area_center;

    vec2<int> player_rect_size = {10, 50};

    vec2<float> ball_position;
    vec2<float> ball_speed;
    vec2<float> ball_size = {6, 6};
    vec2<float> player0_position;
    vec2<float> player1_position;

    bool exit=false;

    shared_ptr<Circle> ball_circle;
    shared_ptr<Rectangle> player0_rect;
    shared_ptr<Rectangle> player1_rect;
    shared_ptr<Text> player0_score_text;
    shared_ptr<Text> player1_score_text;
    shared_ptr<Text> playtime_text;
    shared_ptr<EmptyRectangle> play_area_rect;

    shared_ptr<Gamepad> gamepad;

    virtual void Initialize() override
    {
        srand(get_time_us());
        play_area = {25, 50, lines.x-25, lines.y-15};
        play_area_size = play_area.zw()-play_area.xy();
        play_area_center = play_area.xy() + play_area_size/2;

        play_area_rect = std::make_shared<EmptyRectangle>(entity_manager, 255, false, true, 0, 0, 
            play_area.xy(), play_area.zw()-play_area.xy());

        ball_position = play_area_center.convert<float>();
        ball_circle = std::make_shared<Circle>(entity_manager, 255, 255, true, true, 0, 0, 
            ball_position.convert<int>(), ball_size.convert<int>());
        ball_speed = vec2<float>{10.0f, 10.0f} * ((rand() % 1) ? -1.0f : 1.0f); 
        
        player0_position = {play_area.x + 2.0f + player_rect_size.x/2.0f, (float)play_area_center.y};
        player0_rect = std::make_shared<Rectangle>(entity_manager, 255, 255, true, true, 0, 0, 
            player0_position.convert<int>(), player_rect_size);

        player1_position = {play_area.z - 2.0f - player_rect_size.x/2.0f, (float)play_area_center.y};
        player1_rect = std::make_shared<Rectangle>(entity_manager, 255, 255, true, true, 0, 0, 
            player1_position.convert<int>(), player_rect_size);

        playtime_text = std::make_shared<Text>(entity_manager, text_manager, "00:00", FONT::FIXED_10_20, 
            TEXT_ALIGNMENT::CENTER, true, true, 0, vec2<int>{play_area.x+(play_area.z-play_area.x)/2, (play_area.y - 20)});
        player0_score_text = std::make_shared<Text>(entity_manager, text_manager, "000000", FONT::FIXED_10_20, 
            TEXT_ALIGNMENT::CENTER, true, true, 0, vec2<int>{int(play_area.x+(play_area.z-play_area.x)*0.75f), (play_area.y - 20)});
        player1_score_text = std::make_shared<Text>(entity_manager, text_manager, "000000", FONT::FIXED_10_20, 
            TEXT_ALIGNMENT::CENTER, true, true, 0, vec2<int>{int(play_area.x+(play_area.z-play_area.x)*0.25f), (play_area.y - 20)});
        
        auto gamepads = gamepad_manager.GetGamepads();
        if(gamepads.size() > 0)
        {
            gamepad = gamepads.begin()->second;
            for(auto it : gamepads)
                it.second->SetOnButtonDownHandle(Gamepad::CENTER, [&](){exit=true;});
        }
        gamepad_manager.AddOnGamepadAddedHandler([&](shared_ptr<Gamepad> gamepad)
        {
            gamepad = gamepad;
            gamepad->SetOnButtonDownHandle(Gamepad::CENTER, [&](){exit=true;});
        });
        gamepad_manager.AddOnGamepadRemovedHandler([&](shared_ptr<Gamepad> gamepad)
        {
            if(gamepad == gamepad)
            {
                gamepad = nullptr;
                auto gamepads = gamepad_manager.GetGamepads();
                if(gamepads.size() > 0)
                {
                    gamepad = gamepads.begin()->second;
                }
            }
        });
    }
    virtual bool Tick(float delta) override
    {
        if(exit)
            return false;
        if(gamepad == nullptr)
        {
            playtime_text->SetText("NO GAMEPAD");
            return true;
        }

        return true;
    }
};