#include "hmain.hpp"

int main()
{
    stdio_init_all();

    printf("Setting sysclock to %i khz\n", sysclockkhz);
    set_sys_clock_khz(sysclockkhz, true);
    uart_set_baudrate(uart0, 115200);
    //cyw43_set_pio_clkdiv_int_frac8(2, 0);
    printf("clock set successfully\n");


    multicore_launch_core1(AudioLoop);

    sleep_ms(1); // wait for flash_safe_execute_core_init() to be called on the other core

    bt_init();

    TestMemoryManager();


    sleep_ms(100); // wait for the GPU to start

    unique_ptr<Context> context = std::make_unique<Context>();
    context->gpu = std::make_unique<GPU>(i2c0, I2C_ADDR, PIN::I2C_SCL, PIN::I2C_SDA);
    context->entity_manager = std::make_unique<EntityManager>(context->gpu.get(), N_ENTITIES);
    context->text_manager = std::make_unique<TextManager>(context->gpu.get(), TEXT_BUFFER_SIZE);
    context->geometry_manager = std::make_unique<GeometryManager>(context->gpu.get(), GEOMETRY_BUFFER_SIZE);
    context->texture_manager = std::make_unique<TextureManager>(context->gpu.get(), 15000);

    Program* current_program = new MainMenuProgram();
    current_program->SetContext(context.get());
    current_program->Initialize();
    uint64_t tmstart = get_time_us();
    while(true)
    {
        uint64_t tmdif = get_time_us() - tmstart;
        float tm = float(tmdif)/1000.0f/1000.0f;
        tmstart = get_time_us();
        gamepad_manager.ProcessInputs();
        bool ret = current_program->Tick(tm);
        if(!ret)
        {
            Program* next_program = current_program->NextProgram;
            delete current_program;
            if(next_program == nullptr)
            {
                next_program = new MainMenuProgram();
            }
            gamepad_manager.ClearHandlers();
            current_program = next_program;
            current_program->SetContext(context.get());
            current_program->Initialize();
            tmstart = get_time_us();
        }
    }
}
