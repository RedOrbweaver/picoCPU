#include "hmain.hpp"



int main()
{
    stdio_init_all();

    printf("Setting sysclock to %i khz\n", sysclockkhz);
    set_sys_clock_khz(sysclockkhz, true);
    uart_set_baudrate(uart0, 115200);
    printf("clock set successfully\n");

    multicore_launch_core1(AudioLoop);

    TestMemoryManager();

    sleep_ms(100); // wait for the GPU to start

    unique_ptr<GPU> gpu = std::make_unique<GPU>(i2c0, I2C_ADDR, PIN::I2C_SCL, PIN::I2C_SDA);
    unique_ptr<EntityManager> entity_manager = std::make_unique<EntityManager>(gpu.get(), N_ENTITIES);
    unique_ptr<TextManager> text_manager = std::make_unique<TextManager>(gpu.get(), TEXT_BUFFER_SIZE);
    unique_ptr<GeometryManager> geometry_manager = std::make_unique<GeometryManager>(gpu.get(), GEOMETRY_BUFFER_SIZE);
    unique_ptr<TextureManager> texture_manager = std::make_unique<TextureManager>(gpu.get(), 15000);

    Info info = gpu->ReadInfo();
    int lines_x = info.lines_x;
    int lines_y = info.lines_y;

    float sx = 10.0f;
    float sy = 10.0f;
    float px = 50.00f;
    float py = 50.0f;
    float r = 15;

    unique_ptr<Circle> ball = std::make_unique<Circle>(entity_manager.get(), 255, 100, true, true);
    ball->SetSize({(int)r, (int)r});

    Line* line = new Line(entity_manager.get(), 255, {50, 50}, {250, 150}, true);

    Triangle* triangle = new Triangle(entity_manager.get(), 100, {0, 0}, {0, 40}, {40, 40}, true, true, 2, 0, {lines_x/2, lines_y/2}, {1, 1});

    Rectangle* rectangle = new Rectangle(entity_manager.get(), 255, 50, true, true, 0, 0, {50, 50}, {50, 50});
    Rectangle* rectangle0 = new Rectangle(entity_manager.get(), 255, 50, true, true, 0, 0, {200, 250}, {75, 75});
    Rectangle* rectangle1 = new Rectangle(entity_manager.get(), 255, 50, true, true, 0, 0, {300, 250}, {75, 50});

    EmptyRectangle* empty_rectangle = new EmptyRectangle(entity_manager.get(), 255, true, true, 0, 0, {100, 250}, {100, 20});

    EmptyCircle* empty_circle = new EmptyCircle(entity_manager.get(), 255, EMPTY_CIRCLE_MODE::FULL, true, true, 0, {0,0}, {15, 15});

    Text* text0 = new Text(entity_manager.get(), text_manager.get(), "test0", FONT::FIXED_10_20, TEXT_ALIGNMENT::CENTER, true, true, 0, {lines_x/2, lines_y/2});
    Text* text1 = new Text(entity_manager.get(), text_manager.get(), "test1", FONT::FIXED_7_14, TEXT_ALIGNMENT::CENTER, true, true, 0, {lines_x/2, lines_y/2 + 17});

    vector<vec2<int>> ml = {{0, 0}, {50, 0}, {40, 30}, {20, 30}, {0, 0}};
    auto mlg = geometry_manager->AllocateGeomentry(ml);
    MultiLine* multiline = new MultiLine(entity_manager.get(), geometry_manager.get(), 255, mlg, true, 0, 0, {50, 150});
    
    vec2<int> points[128];
    for(int i = 0; i < 128; i++)
    {
        points[i] = {rand() % lines_x, (rand() % (2*lines_y)) - lines_y};
    }

    vector<vec2<int>> vp (points, points+128);
    auto vpg = geometry_manager->AllocateGeomentry(vp);
    MultiPoint* multipoint = new MultiPoint(entity_manager.get(), geometry_manager.get(), 255, vpg, true);

    vector<vec2<int>> bp = {{0, 0}, {70, -50}, {50, 0}};
    auto bpg = geometry_manager->AllocateGeomentry(bp);
    Bezier* bezier = new Bezier(entity_manager.get(), geometry_manager.get(), 255, bpg, true, 0, 0, {200, 200});

    shared_ptr<Texture> texture = texture_manager->CreateTextureFromTGA(__test_tga, __test_tga_len);
    Sprite* sprite = new Sprite(entity_manager.get(), texture, true, true, 0, {0, 0}, true, 0);

    while(true)
    {
        uint64_t ttm = get_time_us();
        float nx = px + sx;
        float ny = py + sy;
        if(nx < r || nx > float(lines_x)-r)
        {
            sx = -sx;
        }
        else
            px = nx;
        if(ny < r || ny > float(lines_y)-r)
        {
            sy = -sy;
        }
        else
            py = ny;

        ball->SetPosition({(int)nx, (int)ny});
        //empty_circle->SetPosition({lines_x-(int)nx, lines_y-(int)ny});
        empty_circle->SetPosition(ball->GetPosition());

        sprite->SetPosition({lines_x-(int)nx, lines_y-(int)ny});

        triangle->SetRotation(triangle->GetRotation()+1);

        sprite->SetRotation(sprite->GetRotation()+1);

        empty_rectangle->SetRotation(empty_rectangle->GetRotation()+1);

        rectangle->SetRotation(rectangle->GetRotation()-1);
        rectangle0->SetRotation(rectangle0->GetRotation()-1);
        rectangle1->SetRotation(rectangle1->GetRotation()-1);

        multiline->SetRotation(multiline->GetRotation()-1);

        vec2<int> pmp = multipoint->GetPosition();
        if(pmp.y > lines_y)
            pmp.y = 0;
        else
            pmp.y += 1;
        multipoint->SetPosition(pmp);

        Info info = gpu->ReadInfo();
        clear_console();
        printf("Audio time: %lluus\n", LastAudioProcessingTime);
        printf("Render time: %lluus\n", info.last_render_time_us);
        printf("Entities drawn: %u\n", info.entities_drawn);
        printf("GPU temperature: %.3f\n", info.temperature);
        printf("GPU memory: %u/%u\n", info.free_memory, info.total_memory);
        printf("CPU memory: %u/%u\n", GetFreeHeap(), GetTotalHeap());
        text0->SetText(std::to_string(info.frame_number));
        text1->SetText(std::to_string(info.last_render_time_us) + "us");
        printf("I2CTime: %u\n", gpu->i2ctime);
        gpu->i2ctime = 0;
        sleep_ms(25);
        //getchar();
        //int test = ReadTest();
        //printf("%i\n", test);
    }
}
