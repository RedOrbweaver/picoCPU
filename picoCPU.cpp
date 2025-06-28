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
    unique_ptr<TextureManager> texture_manager = std::make_unique<TextureManager>(gpu.get(), 1000);


    int lines_x = 408;
    int lines_y = 304;

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

    EmptyRectangle* empty_rectangle = new EmptyRectangle(entity_manager.get(), 255, true, true, 0, 0, {100, 250}, {100, 20});

    EmptyCircle* empty_circle = new EmptyCircle(entity_manager.get(), 255, EMPTY_CIRCLE_MODE::FULL, true, true, 0, {0,0}, {15, 15});

    Text* text0 = new Text(entity_manager.get(), text_manager.get(), "test0", FONT::FIXED_10_20, TEXT_ALIGNMENT::CENTER, true, true, 0, {lines_x/2, lines_y/2});
    Text* text1 = new Text(entity_manager.get(), text_manager.get(), "test1", FONT::FIXED_7_14, TEXT_ALIGNMENT::CENTER, true, true, 0, {lines_x/2, lines_y/2 + 17});

    MultiLine* multiline = new MultiLine(entity_manager.get(), geometry_manager.get(), 255, {{0, 0}, {50, 0}, {40, 30}, {20, 30}, {0, 0}}, true, 0, 0, {50, 150});
    
    vec2<int> points[128];
    for(int i = 0; i < 128; i++)
    {
        points[i] = {rand() % lines_x, (rand() % (2*lines_y)) - lines_y};
    }

    vector<vec2<int>> vp (points, points+128);

    MultiPoint* multipoint = new MultiPoint(entity_manager.get(), geometry_manager.get(), 255, vp, true);

    vector<vec2<int>> bp = {{0, 0}, {70, -50}, {50, 0}};

    Bezier* bezier = new Bezier(entity_manager.get(), geometry_manager.get(), 255, bp, true, 0, 0, {200, 200});

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

        empty_rectangle->SetRotation(empty_rectangle->GetRotation()+1);

        rectangle->SetRotation(rectangle->GetRotation()-1);

        multiline->SetRotation(multiline->GetRotation()-1);

        multipoint->SetStartEnd(5, 128+5);
        multipoint->SetGeometry(vp);

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
        sleep_ms(25);
        //getchar();
        //int test = ReadTest();
        //printf("%i\n", test);
    }
}
