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


    int lines_x = 408;
    int lines_y = 304;

    float sx = 10.0f;
    float sy = 10.0f;
    float px = 50.00f;
    float py = 50.0f;
    float r = 15;

    unique_ptr<Circle> ball = std::make_unique<Circle>(entity_manager.get(), 50, 0, true, true);
    ball->SetSize({(int)r, (int)r});

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

        

        Info info = gpu->ReadInfo();
        clear_console();
        printf("Audio time: %lluus\n", LastAudioProcessingTime);
        printf("Render time: %lluus\n", info.last_render_time_us);
        printf("Entities drawn: %u\n", info.entities_drawn);
        printf("GPU temperature: %.3f\n", info.temperature);
        printf("GPU memory: %u/%u\n", info.free_memory, info.total_memory);
        printf("CPU memory: %u/%u\n", GetFreeHeap(), GetTotalHeap());
        sleep_ms(100);
        //getchar();
        //int test = ReadTest();
        //printf("%i\n", test);
    }
}
