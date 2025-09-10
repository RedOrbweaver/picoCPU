#pragma once

class TetrisProgram : public Program
{
    protected:

    static inline constexpr int BOARD_X = 10;
    static inline constexpr int BOARD_Y = 20;
    static inline constexpr float PIECE_STEP_TIME = 1.0f;
    static inline constexpr float EXPLOSION_STEP_TIME = 0.25f;
    static inline constexpr float FALL_STEP_TIME = 0.25f;
    static inline constexpr int BLOCK_TYPES = 6; // 5 tetromino + explosion
    static inline constexpr int BOOM_TYPE = BLOCK_TYPES;

    
    uint8_t board[BOARD_X][BOARD_Y] = {0};

    shared_ptr<Texture> block_textures[BLOCK_TYPES];
    shared_ptr<GeometryManager::Geometry> block_positions[BLOCK_TYPES];
    shared_ptr<MultiSprite> block_sprites[BLOCK_TYPES];
    
    shared_ptr<MultiLines> game_grid_lines;
    shared_ptr<Text> score_text;
    

    public:
    
    virtual bool Tick(float delta) override
    {

    }
    virtual void Initialize() override
    {

    }
};