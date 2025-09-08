#pragma once

class PongProgram : public Program
{
    static const int VICTORY_SCORE = 10;
    static const int BALL_RADIUS = 3;
    float BALL_SPEED_INITIAL = 80.0f;
    float BALL_SPEED_INCREASE_PER_SEC = 0.5f;
    float BOUNCE_FACTOR = 3.0f;
    float PLAYER_SPEED_PER_SEC = 125.0f;


    float time = 0;

    vec4<int> play_area;
    vec2<int> play_area_size;
    vec2<int> play_area_center;

    vec2<int> player_rect_size = {6, 20};

    vec2<float> ball_position;
    vec2<float> ball_speed;
    vec2<float> ball_size = {BALL_RADIUS, BALL_RADIUS};

    int player0_score;
    vec2<float> player0_position;
    int player1_score;
    vec2<float> player1_position;

    bool exit=false;

    float message_time = 0.0f;
    float player0_score_highlight_time = 1.0f;
    float player1_score_highlight_time = 1.0f;

    shared_ptr<Rectangle> ball_circle;
    shared_ptr<Rectangle> player0_rect;
    shared_ptr<Rectangle> player1_rect;
    shared_ptr<Text> player0_score_text;
    shared_ptr<Text> player1_score_text;
    shared_ptr<Text> playtime_text;
    shared_ptr<EmptyRectangle> play_area_rect;

    shared_ptr<Gamepad> gamepad;

    shared_ptr<StreamChannel> goal_channel;
    shared_ptr<StreamChannel> bounce_channel;
    shared_ptr<StreamChannel> gameover_channel;

    virtual void Initialize() override
    {
        srand(get_time_us());
        play_area = {25, 50, lines.x-25, lines.y-15};
        play_area_size = play_area.zw()-play_area.xy();
        play_area_center = play_area.xy() + play_area_size/2;

        play_area_rect = std::make_shared<EmptyRectangle>(entity_manager, 255, false, true, 0, 0, 
            play_area.xy(), play_area.zw()-play_area.xy());

        ball_position = play_area_center.convert<float>();
        // ball_circle = std::make_shared<Circle>(entity_manager, 255, 255, true, true, 0, 0, 
        //     ball_position.convert<int>(), ball_size.convert<int>());
        ball_circle = std::make_shared<Rectangle>(entity_manager, 255, 255, true, true, 0, 0, 
            ball_position.convert<int>(), (ball_size*2).convert<int>());
        ball_speed = {BALL_SPEED_INITIAL, 0.0f};
        ResetBall();
        
        player0_position = {play_area.x + 2.0f + player_rect_size.x/2.0f, (float)play_area_center.y};
        player0_rect = std::make_shared<Rectangle>(entity_manager, 255, 255, true, true, 0, 0, 
            player0_position.convert<int>(), player_rect_size);

        player1_position = {play_area.z - 2.0f - player_rect_size.x/2.0f, (float)play_area_center.y};
        player1_rect = std::make_shared<Rectangle>(entity_manager, 255, 255, true, true, 0, 0, 
            player1_position.convert<int>(), player_rect_size);

        playtime_text = std::make_shared<Text>(entity_manager, text_manager, "00:00", FONT::FIXED_10_20, 
            TEXT_ALIGNMENT::CENTER, true, true, 0, vec2<int>{play_area.x+(play_area.z-play_area.x)/2, (play_area.y - 20)});
        player0_score_text = std::make_shared<Text>(entity_manager, text_manager, "0", FONT::FIXED_10_20, 
            TEXT_ALIGNMENT::CENTER, true, true, 0, vec2<int>{int(play_area.x+(play_area.z-play_area.x)*0.15f), (play_area.y - 20)});
        player1_score_text = std::make_shared<Text>(entity_manager, text_manager, "0", FONT::FIXED_10_20, 
            TEXT_ALIGNMENT::CENTER, true, true, 0, vec2<int>{int(play_area.x+(play_area.z-play_area.x)*0.85f), (play_area.y - 20)});
        
        auto gamepads = gamepad_manager.GetGamepads();
        if(gamepads.size() > 0)
        {
            gamepad = gamepads.begin()->second;
            for(auto it : gamepads)
                it.second->SetOnButtonDownHandle(Gamepad::CENTER, [&](){exit=true;});
        }
        gamepad_manager.AddOnGamepadAddedHandler([&](shared_ptr<Gamepad> gamepad)
        {
            this->gamepad = gamepad;
            gamepad->SetOnButtonDownHandle(Gamepad::CENTER, [&](){exit=true;});
        });
        gamepad_manager.AddOnGamepadRemovedHandler([&](shared_ptr<Gamepad> gamepad)
        {
            if(this->gamepad == gamepad)
            {
                gamepad = nullptr;
                auto gamepads = gamepad_manager.GetGamepads();
                if(gamepads.size() > 0)
                {
                    gamepad = gamepads.begin()->second;
                }
            }
        });

        LockAudio();
        goal_channel = std::make_shared<StreamChannel>((uint8_t*)__goal_wav + 44, 11025, ArraySize(__goal_wav) - 44, 1.0f, false);
        goal_channel->SetVolume(2.0f);
        bounce_channel = std::make_shared<StreamChannel>((uint8_t*)__sharp_pop_wav + 44, 11025, ArraySize(__sharp_pop_wav) - 44, 1.0f, false);
        gameover_channel = std::make_shared<StreamChannel>((uint8_t*)__gameover_wav + 44, 11025, ArraySize(__gameover_wav) - 44, 1.0f, false);
        gameover_channel->SetVolume(1.5f);

        AudioChannels[1] = goal_channel;
        AudioChannels[2] = bounce_channel;
        AudioChannels[3] = gameover_channel;
        UnLockAudio();
    }
    void PlayGoal()
    {
        goal_channel->Lock();
        goal_channel->Reset();
        goal_channel->SetEnabled(true);
        goal_channel->UnLock();
    }
    void PlayBounce(float volume)
    {
        bounce_channel->Lock();
        bounce_channel->Reset();
        bounce_channel->SetVolume(volume);
        bounce_channel->SetEnabled(true);
        bounce_channel->UnLock();
    }
    void PlayGameover()
    {
        gameover_channel->Lock();
        gameover_channel->SetEnabled(true);
        gameover_channel->UnLock();
    }
    void ResetBall()
    {
        ball_position = play_area_center.convert<float>();
        float d = float(rand() % 2000)/1000.0f - 1.0f;
        ball_speed = vec2<float>{cos(d), sin(d)} * ball_speed.length();
        ball_speed.x *= (rand() % 2) ? 1.0f : -1.0f;
    }
    void ResetGame()
    {
        ball_circle->SetVisible(true);
        ball_speed = {BALL_SPEED_INITIAL, 0.0f};
        ResetBall();
        player0_score = 0;
        player0_score_text->SetText("0");
        player0_position = {play_area.x + 2.0f + player_rect_size.x/2.0f, (float)play_area_center.y};
        player0_rect->SetPosition(player0_position.convert<int>());
        player1_score = 0;
        player1_score_text->SetText("0");
        player1_position = {play_area.z - 2.0f - player_rect_size.x/2.0f, (float)play_area_center.y};
        player1_rect->SetPosition(player1_position.convert<int>());
        playtime_text->SetText("00:00");
        time = 0.0f;
        player0_score_highlight_time = 1.0f;
        player1_score_highlight_time = 1.0f;
    }
    void WinCondition(int player)
    {
        playtime_text->SetText(std::string((player == 0) ? "LEFT" : "RIGHT") + " PLAYER WON!");
        ball_circle->SetVisible(false);
        message_time = 5.0f;
        PlayGameover();
    }
    void DrawCondition()
    {
        playtime_text->SetText("DRAW!");
        ball_circle->SetVisible(false);
        message_time = 5.0f;
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

        if(player0_score_highlight_time > 0.0f)
        {
            player0_score_highlight_time -= delta;
            if(player0_score_highlight_time <= 0.0f)
                player0_score_text->SetVisible(true);
            else
                player0_score_text->SetVisible((get_time_ms()/200) % 2);
        }

        if(player1_score_highlight_time > 0.0f)
        {
            player1_score_highlight_time -= delta;
            if(player1_score_highlight_time <= 0.0f)
                player1_score_text->SetVisible(true);
            else
                player1_score_text->SetVisible((get_time_ms()/200) % 2);
        }

        if(message_time > 0.0f)
        {
            message_time -= delta;
            if(message_time <= 0.0f)
            {
                playtime_text->SetVisible(true);
                ResetGame();
            }
            else
            {
                playtime_text->SetVisible((get_time_ms() / 500) % 2);
            }
            return true;
        }

        ball_speed = ball_speed.normalized() * (ball_speed.length() + BALL_SPEED_INCREASE_PER_SEC * delta);

        ball_position += ball_speed * delta;

        if(ball_position.x < play_area.x + BALL_RADIUS)
        {
            ResetBall();
            player1_score++;
            player1_score_highlight_time = 1.0f;
            player1_score_text->SetText(std::to_string(player1_score));
            if(player1_score == VICTORY_SCORE)
            {
                WinCondition(1);
                return true;
            }
            else
                PlayGoal();
        }
        if(ball_position.x > play_area.z - BALL_RADIUS)
        {
            ResetBall();
            player0_score++;
            player0_score_highlight_time = 1.0f;
                player0_score_text->SetText(std::to_string(player0_score));
            if(player0_score == VICTORY_SCORE)
            {
                WinCondition(0);
                return true;
            }
            else
                PlayGoal();
        }
        if(ball_position.y <= play_area.y + BALL_RADIUS + 2)
        {
            ball_speed.y = abs(ball_speed.y);
            ball_position.y += 2;
            PlayBounce(0.5f);
        }
        if(ball_position.y >= play_area.w-BALL_RADIUS - 2)
        {
            ball_speed.y = -abs(ball_speed.y);
            ball_position.y -= 2;
            PlayBounce(0.5f);
        }

        float p0ydist = ball_position.y-player0_position.y;
        if(ball_position.x < player0_position.x + player_rect_size.x/2 + BALL_RADIUS 
            && abs(p0ydist) < player_rect_size.y/2 + BALL_RADIUS)
        {
            float d = p0ydist / player_rect_size.y/2 * BOUNCE_FACTOR;
            vec2<float> direction = {cos(d), sin(d)};
            ball_speed = direction * ball_speed.length();
            PlayBounce(0.8f);
        }

        float p1ydist = ball_position.y-player1_position.y;
        if(ball_position.x > player1_position.x - player_rect_size.x/2 - BALL_RADIUS 
            && abs(p1ydist) < player_rect_size.y/2 + BALL_RADIUS)
        {
            float d = p1ydist / player_rect_size.y/2 * BOUNCE_FACTOR;
            vec2<float> direction = {cos(d), -sin(d)};
            ball_speed = direction * -ball_speed.length();
            PlayBounce(0.8f);
        }

        ball_circle->SetPosition(ball_position.convert<int>());

        float tlim = play_area.y + player_rect_size.y/2 + 1;
        float blim = play_area.w - player_rect_size.y/2 -1;
        if(gamepad->IsButtonDown(Gamepad::LEFT_UP))
        {
            player0_position.y -= PLAYER_SPEED_PER_SEC*delta;
            if(player0_position.y < tlim)
                player0_position.y = tlim;
        }
        if(gamepad->IsButtonDown(Gamepad::LEFT_DOWN))
        {
            player0_position.y += PLAYER_SPEED_PER_SEC*delta;
            if(player0_position.y > blim)
                player0_position.y = blim;
        }
        player0_rect->SetPosition(player0_position.convert<int>());
        if(gamepad->IsButtonDown(Gamepad::RIGHT_UP))
        {
            player1_position.y -= PLAYER_SPEED_PER_SEC*delta;
            if(player1_position.y < tlim)
                player1_position.y = tlim;
        }
        if(gamepad->IsButtonDown(Gamepad::RIGHT_DOWN))
        {
            player1_position.y += PLAYER_SPEED_PER_SEC*delta;
            if(player1_position.y > blim)
                player1_position.y = blim;
        }
        player1_rect->SetPosition(player1_position.convert<int>());

        time += delta;

        int minutes = time / 60;
        if(minutes == 60)
        {
            DrawCondition();
            return true;
        }
        int seconds = int(time) % 60;
        std::string tmstr = ((minutes < 10) ? "0" : "") + std::to_string(minutes) + ":" 
            + ((seconds < 10) ? "0" : "") + std::to_string(seconds);
        playtime_text->SetText(tmstr);

        return true;
    }
};