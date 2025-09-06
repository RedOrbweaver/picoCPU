#pragma once

class BasicTestProgram : public Program
{
    shared_ptr<Circle> ball;
    shared_ptr<Line> line;
    shared_ptr<Triangle> triangle;
    shared_ptr<Rectangle> rectangle, rectangle0, rectangle1;
    shared_ptr<EmptyRectangle> empty_rectangle;
    shared_ptr<EmptyCircle> empty_circle;
    shared_ptr<Text> text0, text1;
    shared_ptr<MultiLine> multiline;
    shared_ptr<MultiPoint> multipoint;
    shared_ptr<Bezier> bezier;
    shared_ptr<Sprite> sprite;

    shared_ptr<Texture> texture;
    shared_ptr<GeometryManager::Geometry> vpg, bpg;

    shared_ptr<Gamepad> current_gamepad;

    float sx = 10.0f;
    float sy = 10.0f;
    float px = 50.00f;
    float py = 50.0f;
    float r = 15;

    public:
    virtual bool Tick(float delta) override
    {
        uint64_t ttm = get_time_us();
        float nx = px + sx;
        float ny = py + sy;
        if(nx < r || nx > float(lines.x)-r)
        {
            sx = -sx;
        }
        else
            px = nx;
        if(ny < r || ny > float(lines.y)-r)
        {
            sy = -sy;
        }
        else
            py = ny;

        ball->SetPosition({(int)nx, (int)ny});
        //empty_circle->SetPosition({lines_x-(int)nx, lines_y-(int)ny});
        empty_circle->SetPosition(ball->GetPosition());

        sprite->SetPosition({lines.x-(int)nx, lines.y-(int)ny});

        triangle->SetRotation(triangle->GetRotation()+1);

        sprite->SetRotation(sprite->GetRotation()+1);

        empty_rectangle->SetRotation(empty_rectangle->GetRotation()+1);

        rectangle->SetRotation(rectangle->GetRotation()-1);
        rectangle0->SetRotation(rectangle0->GetRotation()-1);
        rectangle1->SetRotation(rectangle1->GetRotation()-1);

        multiline->SetRotation(multiline->GetRotation()-1);

        vec2<int> pmp = multipoint->GetPosition();
        if(pmp.y > lines.y)
            pmp.y = 0;
        else
            pmp.y += 1;
        multipoint->SetPosition(pmp);

        if(current_gamepad != nullptr)
        {
            if(current_gamepad->IsButtonDown(Gamepad::LEFT_DOWN))
                triangle->SetPosition(triangle->GetPosition() + int2{0, 1});
            if(current_gamepad->IsButtonDown(Gamepad::LEFT_UP))
                triangle->SetPosition(triangle->GetPosition() + int2{0, -1});
            if(current_gamepad->IsButtonDown(Gamepad::LEFT_LEFT))
                triangle->SetPosition(triangle->GetPosition() + int2{-1, 0});
            if(current_gamepad->IsButtonDown(Gamepad::LEFT_RIGHT))
                triangle->SetPosition(triangle->GetPosition() + int2{1, 0});
        }

        Info info = gpu->ReadInfo();
        //clear_console();
        // printf("Audio time: %lluus\n", LastAudioProcessingTime);
        // printf("Render time: %lluus\n", info.last_render_time_us);
        // printf("Entities drawn: %u\n", info.entities_drawn);
        // printf("GPU temperature: %.3f\n", info.temperature);
        // printf("GPU memory: %u/%u\n", info.free_memory, info.total_memory);
        // printf("CPU memory: %u/%u\n", GetFreeHeap(), GetTotalHeap());
        text0->SetText(std::to_string(info.frame_number));
        text1->SetText(std::to_string(info.last_render_time_us) + "us");
        //printf("I2CTime: %u\n", gpu->i2ctime);
        gpu->i2ctime = 0;
        //getchar();
        //int test = ReadTest();
        //printf("%i\n", test);
        return true;

    }
    virtual void Initialize() override
    {

        ball = std::make_shared<Circle>(entity_manager, 255, 100, true, true);
        ball->SetSize({(int)r, (int)r});

        line = shared_ptr<Line>(new Line(entity_manager, 255, {50, 50}, {250, 150}, true));

        triangle = shared_ptr<Triangle>(new Triangle(entity_manager, 100, {0, 0}, {0, 40}, {40, 40}, true, true, 2, 0, {lines.x/2, lines.y/2}, {1, 1}));

        rectangle = shared_ptr<Rectangle>(new Rectangle(entity_manager, 255, 50, true, true, 0, 0, {50, 50}, {50, 50}));
        rectangle0 = shared_ptr<Rectangle>(new Rectangle(entity_manager, 255, 50, true, true, 0, 0, {200, 250}, {75, 75}));
        rectangle1 = shared_ptr<Rectangle>(new Rectangle(entity_manager, 255, 50, true, true, 0, 0, {300, 250}, {75, 50}));

        empty_rectangle = shared_ptr<EmptyRectangle>(new EmptyRectangle(entity_manager, 255, true, true, 0, 0, {100, 250}, {100, 20}));

        empty_circle = shared_ptr<EmptyCircle>(new EmptyCircle(entity_manager, 255, EMPTY_CIRCLE_MODE::FULL, true, true, 0, {0,0}, {15, 15}));

        text0 = shared_ptr<Text>(new Text(entity_manager, text_manager, "test0", FONT::FIXED_10_20, TEXT_ALIGNMENT::CENTER, true, true, 0, {lines.x/2, lines.y/2}));
        text1 = shared_ptr<Text>(new Text(entity_manager, text_manager, "test1", FONT::FIXED_7_14, TEXT_ALIGNMENT::CENTER, true, true, 0, {lines.x/2, lines.y/2 + 17}));

        vector<vec2<int>> ml = {{0, 0}, {50, 0}, {40, 30}, {20, 30}, {0, 0}};
        auto mlg = geometry_manager->AllocateGeomentry(ml);
        multiline = shared_ptr<MultiLine>(new MultiLine(entity_manager, geometry_manager, 255, mlg, true, 0, 0, {50, 150}));
        
        vec2<int> points[128];
        for(int i = 0; i < 128; i++)
        {
            points[i] = {rand() % lines.x, (rand() % (2*lines.y)) - lines.y};
        }

        vector<vec2<int>> vp (points, points+128);
        vpg = geometry_manager->AllocateGeomentry(vp);
        multipoint = shared_ptr<MultiPoint>(new MultiPoint(entity_manager, geometry_manager, 255, vpg, true));

        vector<vec2<int>> bp = {{0, 0}, {70, -50}, {50, 0}};
        bpg = geometry_manager->AllocateGeomentry(bp);
        bezier = shared_ptr<Bezier>(new Bezier(entity_manager, geometry_manager, 255, bpg, true, 0, 0, {200, 200}));

        texture = texture_manager->CreateTextureFromTGA(__test_tga, __test_tga_len);
        sprite = shared_ptr<Sprite>(new Sprite(entity_manager, texture, true, true, 0, {0, 0}, true, 0));


        gamepad_manager.AddOnGamepadAddedHandler([&](shared_ptr<Gamepad> gamepad)
        {
            current_gamepad = gamepad;
        });
        gamepad_manager.AddOnGamepadRemovedHandler([&](shared_ptr<Gamepad> gamepad)
        {
            if(current_gamepad == gamepad)
                current_gamepad = nullptr;
        });
        auto gamepads = gamepad_manager.GetGamepads();
        if(gamepads.size() > 0)
        {
            current_gamepad = gamepads.begin()->second;
        }
    }
};