#pragma once

class TetrisProgram : public Program
{
    protected:

    static inline constexpr int BOARD_X = 10;
    static inline constexpr int BOARD_Y = 20;
    static inline constexpr vec2<int> BOARD_SIZE = {BOARD_X, BOARD_Y};

    static inline constexpr float PIECE_STEP_TIME = 0.5f;
    static inline constexpr float EXPLOSION_STEP_TIME = 0.05f;
    static inline constexpr float REMOVE_LINE_STEP_TIME = 0.1f;
    static inline constexpr float NEW_PIECE_DELAY = 0.5f;
    static inline constexpr float FALL_STEP_TIME = 0.25f;
    static inline constexpr int TETRIS_STEPS = 10;
    static inline constexpr float TETRIS_STEP_TIME = 0.1f;

    static inline constexpr int TETROMINO_TYPES = 5;
    static inline constexpr int BLOCK_TYPES = TETROMINO_TYPES+1; // 5 tetromino + explosion
    static inline constexpr int BLOCK_SIZE_X = 12;
    static inline constexpr int BLOCK_SIZE_Y = 12;
    static inline constexpr vec2<int> BLOCK_SIZE = {BLOCK_SIZE_X, BLOCK_SIZE_Y};
    static inline constexpr vec2<int> BOARD_TOP_LEFT = {35, 50};
    static inline constexpr vec2<int> BOARD_TOP_RIGHT = BOARD_TOP_LEFT + vec2<int>{BLOCK_SIZE_X, 0} * vec2<int>{BOARD_X, 0};

    static inline constexpr int NORMAL_BLOCK_LAYER = 4;
    static inline constexpr int EXPLOSION_BLOCK_LAYER = 1;
    static inline constexpr int TIME_CIRCLE_LAYER = 6;
    static inline constexpr int TIME_TRIANGLE_LAYER = 5;
    static inline constexpr int TIME_CIRCLE_FRAME_LAYER = 4;
    static inline constexpr int NORMAL_UI_LAYER = 0;
    static inline constexpr int INDICATOR_LAYER = 0;
    static inline constexpr int TETRIS_LAYER = 0;
    static inline constexpr int TETRIS_SQUARE_LAYER = 1;

    static inline constexpr int SCORE_TEXT_LENGTH = 8;
    static inline constexpr int SCORE_PIECE_PLACED = 10;
    static inline constexpr int SCORE_BLOW_UP = 100;
    static inline constexpr int SCORE_LINE_DESTROYED = 1000;
    static inline constexpr int SCORE_TETRIS_STEP = 250;
    static inline constexpr int SCORE_TETRIS = 10000;
    static inline constexpr float SCORE_LAST_ADDED_VISIBLE_TIME = 0.5f;

    static inline constexpr int TIME_CIRCLE_RADIUS = 15;

    vec2<int> time_circle_center = {BLOCK_SIZE_X*BOARD_X + 65, 60};
    bool time_circle_color_polarity = false;

    vec2<int> next_piece_center = {BLOCK_SIZE_X*BOARD_X + 65, 60 + BLOCK_SIZE_Y + TIME_CIRCLE_RADIUS*2};

    vec2<int> score_title_text_center = BOARD_TOP_RIGHT + vec2<int>{120, 10};
    vec2<int> score_text_center = BOARD_TOP_RIGHT + vec2<int>{120, 30};
    vec2<int> score_added_text_center = BOARD_TOP_RIGHT + vec2<int>{120, 45};

    int score = 0;
    int last_score = 0;
    int last_score_added = 0;
    float last_score_added_time = 0.0f;

    struct block
    {
        bool toexplode : 1 = false;
        bool exploding : 1 = false;
        uint8_t value : 6 = 0;
        bool IsEmpty()
        {
            return value == 0;
        } 
        uint8_t GetBlockIndex()
        {
            assert(!IsEmpty());
            return value - 1;
        }
    };

    enum class STATE
    {
        START,
        PLAYING,
        PIECE_STUCK,
        NEW_PIECE_DELAY,
        EXPLODING,
        TETRIS,
        REMOVING_LINES,
        GAME_OVER,
    };

    STATE state = STATE::START;

    struct tetromino
    {
        std::vector<std::vector<vec2<int>>> block_positions;
        uint8_t rotations;
        block type;
    };

    tetromino tetrominoes[TETROMINO_TYPES] = 
    {
        {
            // straight
            std::vector<std::vector<vec2<int>>>
            {
                {
                    vec2<int>{-1, 0},
                    vec2<int>{0, 0},
                    vec2<int>{1, 0},
                    vec2<int>{2, 0}
                },
                {
                    vec2<int>{0, -2},
                    vec2<int>{0, -1},
                    vec2<int>{0, 0},
                    vec2<int>{0, 1}
                },
            },
            2,
            block{false, false, 1}
        },
        {
            // square
            std::vector<std::vector<vec2<int>>>
            {
                {   
                    vec2<int>{-1, -1},
                    vec2<int>{0, -1},
                    vec2<int>{-1, 0},
                    vec2<int>{0, 0}
                },
            },
            1,
            block{false, false, 2}
        },
        {
            // T
            std::vector<std::vector<vec2<int>>>
            {
                {   
                    vec2<int>{0, 0},
                    vec2<int>{-1, 0},
                    vec2<int>{1, 0},
                    vec2<int>{0, 1}
                },
                {
                    vec2<int>{0, 0},
                    vec2<int>{0, -1},
                    vec2<int>{0, 1},
                    vec2<int>{1, 0}
                },
                {
                    vec2<int>{0, 0},
                    vec2<int>{0, -1},
                    vec2<int>{0, 1},
                    vec2<int>{-1, 0}                    
                },
                {
                    vec2<int>{0, 0},
                    vec2<int>{-1, 0},
                    vec2<int>{1, 0},
                    vec2<int>{0, -1}                    
                } 
            },
            4,
            block{false, false, 3}
        },
        {
            // L
            std::vector<std::vector<vec2<int>>>
            {
                {   
                    vec2<int>{0, 0},
                    vec2<int>{1, 0},
                    vec2<int>{0, -1},
                    vec2<int>{0, -2}
                },
                {
                    vec2<int>{0, 0},
                    vec2<int>{0, 1},
                    vec2<int>{0, 2},
                    vec2<int>{-1, 0}                    
                },
                {
                    vec2<int>{0, 0},
                    vec2<int>{0, -1},
                    vec2<int>{1, 0},
                    vec2<int>{2, 0}                    
                },
                {
                    vec2<int>{0, 0},
                    vec2<int>{0, -1},
                    vec2<int>{-1, 0},
                    vec2<int>{-2, 0}                    
                }
            },
            4,
            block{false, false, 4}
        },
        {
            // Skew
            std::vector<std::vector<vec2<int>>>
            {
                {   
                    vec2<int>{0, 0},
                    vec2<int>{-1, 0},
                    vec2<int>{0, -1},
                    vec2<int>{1, -1}
                },
                {   
                    vec2<int>{0, 0},
                    vec2<int>{0, -1},
                    vec2<int>{1, 0},
                    vec2<int>{1, 1}
                },                
                {   
                    vec2<int>{0, 0},
                    vec2<int>{1, 0},
                    vec2<int>{0, -1},
                    vec2<int>{-1, -1}
                },
                {   
                    vec2<int>{0, 0},
                    vec2<int>{0, -1},
                    vec2<int>{-1, 0},
                    vec2<int>{-1, 1}
                },                                
            },
            4,
            block{false, false, 5}
        },
    };
    
    block board[BOARD_Y][BOARD_X] = {0};
    int line_fills[BOARD_Y] = {0};

    uint8_t* texture_datas[BLOCK_TYPES] = {__block0_tga, __block1_tga, __block2_tga, __block3_tga, __block4_tga, __explosion_tga};
    uint32_t texture_lens[BLOCK_TYPES] = {__block0_tga_len, __block1_tga_len, __block2_tga_len, __block3_tga_len, __block4_tga_len, __explosion_tga_len};
    shared_ptr<Texture> block_textures[BLOCK_TYPES];
    shared_ptr<GeometryManager::Geometry> block_positions[BLOCK_TYPES];
    shared_ptr<MultiSprite> block_sprites[BLOCK_TYPES];
    shared_ptr<EmptyRectangle> block_landing_indicators[4];

    
    shared_ptr<MultiLines> game_grid_lines;

    shared_ptr<Text> score_title_text;
    shared_ptr<Text> score_text;
    shared_ptr<Text> score_last_added_text;
    shared_ptr<Text> time_text;

    shared_ptr<Text> tetris_text;
    shared_ptr<Rectangle> tetris_rectangle;

    shared_ptr<Circle> time_circle;
    shared_ptr<Triangle> time_triangles[4];
    shared_ptr<EmptyCircle> time_frame_circle;

    

    tetromino piece;
    uint8_t rotation;
    tetromino next_piece;
    vec2<int> position;
    float game_time = 0.0f;
    float timer = 0.0f;
    bool rotate_pressed = false;
    bool move_pressed = false;
    int move_direction = 0;
    int to_explode = 0;
    int exploded = 0;
    bool exit = false;

    shared_ptr<Gamepad> gamepad;

    
    
    bool IsInsideBoard(int x, int y)
    {
        return x >= 0 && x < BOARD_X && y >= 0 && y < BOARD_Y;
    }
    void SetBoard(int x, int y, block value)
    {
        assert(IsInsideBoard(x, y));
        board[y][x] = value;
    }
    block& GetBoard(int x, int y)
    {
        assert(IsInsideBoard(x, y));
        return board[y][x];
    }

    void SetGamepad(shared_ptr<Gamepad> gamepad)
    {
        this->gamepad = gamepad;
        gamepad->SetOnButtonDownHandle(Gamepad::CENTER, [&](){exit=true;});
        gamepad->SetOnButtonDownHandle(Gamepad::LEFT_LEFT, [&](){if(state == STATE::PLAYING) move_pressed = true; move_direction = -1;});
        gamepad->SetOnButtonDownHandle(Gamepad::LEFT_RIGHT, [&](){if(state == STATE::PLAYING) move_pressed = true; move_direction = 1;});
        gamepad->SetOnButtonDownHandle(Gamepad::LEFT_UP, [&](){if(state == STATE::PLAYING) rotate_pressed=true;});
    }

    void AddScore(int value)
    {
        score += value;
        last_score_added = value;
        last_score_added_time = 0.0f;
    }

    void SetLandingPoints()
    {
        assert(state == STATE::PLAYING || state == STATE::PIECE_STUCK);
        int fpos = position.y;
        for(; fpos < BOARD_Y; fpos++)
        {
            bool found = false;
            for(int i = 0; i < 4; i++)
            {
                auto block = piece.block_positions[rotation][i];
                auto pos = block + vec2<int>{position.x, fpos};
                if((pos.y >= 0) && (!IsInsideBoard(pos.x, pos.y) || !GetBoard(pos.x, pos.y).IsEmpty()))
                {
                    found = true;
                    break;
                }
            }
            if(found)
                break;
        }
        auto tpos = vec2<int>{position.x, fpos-1};
        for(int i = 0; i < ArraySize(block_landing_indicators); i++)
        {
            auto block = piece.block_positions[rotation][i];
            block_landing_indicators[i]->SetPosition((tpos + block) * vec2<int>{BLOCK_SIZE_X, BLOCK_SIZE_Y} + BOARD_TOP_LEFT);
            block_landing_indicators[i]->SetVisible(true);
        }
    }

    bool RotatePiece()
    {
        assert(state == STATE::PLAYING);
        if(piece.rotations == 1)
            return false;
        int nrot = (rotation + 1) % piece.rotations;
        auto npositions = piece.block_positions[nrot];
        bool stuck = false;
        for(int i = 0; i < npositions.size(); i++)
        {
            auto pos = npositions[i] + position;
            if(!IsInsideBoard(pos.x, pos.y) || !GetBoard(pos.x, pos.y).IsEmpty())
            {
                return false;
            }
            auto below = pos + vec2<int>{0, 1};
            if(!IsInsideBoard(below.x, below.y) || !GetBoard(below.x, below.y).IsEmpty())
                stuck = true;
        }
        rotation = nrot;
        if(stuck)
            state = STATE::PIECE_STUCK;
        SetLandingPoints();
        return true;
    }

    bool MovePiece(int direction)
    {
        assert(state == STATE::PLAYING);
        auto npos = position + vec2<int>{direction, 0};
        auto positions = piece.block_positions[rotation];
        bool stuck = false;
        for(int i = 0; i < positions.size(); i++)
        {
            auto pos = positions[i] + npos;
            if(!IsInsideBoard(pos.x, pos.y) || !GetBoard(pos.x, pos.y).IsEmpty())
            {
                return false;
            }
            auto below = pos + vec2<int>{0, 1};
            if(IsInsideBoard(below.x, below.y) && !GetBoard(below.x, below.y).IsEmpty())
                stuck = true;
        }
        position = npos;
        if(stuck)
            state = STATE::PIECE_STUCK;
        SetLandingPoints();
        return true;
    }

    void PlacePiece()
    {
        assert(state == STATE::START || state == STATE::NEW_PIECE_DELAY);
        position = {BOARD_X/2, -1};
        rotation = 0;
        if(state == STATE::START)
            piece = tetrominoes[(rand() % TETROMINO_TYPES)];
        else 
            piece = next_piece;
        state = STATE::PLAYING;
        next_piece = tetrominoes[(rand() % TETROMINO_TYPES)];
        AddScore(SCORE_PIECE_PLACED);
    }
    void StepGame()
    {
        assert(state == STATE::PLAYING || state == STATE::PIECE_STUCK);
        auto positions = piece.block_positions[rotation];
        bool block_placed = false;
        bool any_outside_board = false;
        bool any_line_filled = false;
        for(int i = 0; i < positions.size(); i++)
        {
            auto pos = position + positions[i];
            auto below = pos + vec2<int>{0, 1};
            if(pos.y < -1)
                continue;
            if(!IsInsideBoard(below.x, below.y) || !GetBoard(below.x, below.y).IsEmpty())
            {
                block_placed = true;
            }
            if(!IsInsideBoard(pos.x, pos.y))
                any_outside_board = true;
        }
        if(block_placed)
        {
            for(int i = 0; i < ArraySize(block_landing_indicators); i++)
                block_landing_indicators[i]->SetVisible(false);
            for(int i = 0; i < positions.size(); i++)
            {
                auto pos = position + positions[i];
                if(IsInsideBoard(pos.x, pos.y))
                {
                    line_fills[pos.y]++;
                    SetBoard(pos.x, pos.y, piece.type);
                    if(line_fills[pos.y] == BOARD_X)
                    {
                        GetBoard(pos.x, pos.y).toexplode = true;
                        any_line_filled = true;
                    }
                }
            }
            if(any_outside_board)
            {
                state = STATE::GAME_OVER;
                return;
            }
            if(any_line_filled)
            {
                state = STATE::EXPLODING;
                return;
            }
            state = STATE::NEW_PIECE_DELAY;
        }
        else
        {
            position.y += 1;
            SetLandingPoints();
        }
    }

    void StepExplosion()
    {
        assert(state == STATE::EXPLODING);
        if(to_explode == 0)
        {
            for(int i = 0; i < BOARD_Y; i++)
            {
                if(line_fills[i] == BOARD_X)
                    to_explode += BOARD_X;
            }
            exploded = 0;
            assert(to_explode > 0);
        }
        if(exploded == to_explode)
        {
            to_explode = 0;
            if(exploded == BOARD_X * 4)
                state = STATE::TETRIS;
            else
                state = STATE::REMOVING_LINES;
            return;
        }
        int blowingup = 0;
        for(int i = 0; i < BOARD_Y; i++)
        {
            if(line_fills[i] == BOARD_X)
            {
                blowingup++;
                int ii = 0;
                for(ii = 0; ii < BOARD_X; ii++)
                {
                    auto& b = GetBoard(ii, i);
                    if(b.toexplode)
                    {
                        b.toexplode = false;
                        b.exploding = true;
                        exploded++;
                        AddScore(SCORE_BLOW_UP);
                        if(ii > 0 && !GetBoard(ii-1, i).exploding)
                            GetBoard(ii-1, i).toexplode = true;
                        if(ii < BOARD_X-1 && !GetBoard(ii+1, i).exploding)
                        {
                            GetBoard(ii+1, i).toexplode = true;
                            ii++;
                        }
                        
                    }
                }
            }
        }
    }
    void StepTestris()
    {
        assert(state == STATE::TETRIS);
        static int top_line = -1;
        static int steps = 0;

        AddScore(SCORE_TETRIS_STEP);

        if(steps == TETRIS_STEPS)
        {
            top_line = -1;
            steps = 0;
            state = STATE::REMOVING_LINES;
            tetris_text->SetVisible(false);
            tetris_rectangle->SetVisible(false);
            AddScore(SCORE_TETRIS);
            return;
        }
        
        if(top_line == -1)
        {
            for(int i = 0; i < BOARD_Y; i++)
            {
                if(line_fills[i] == BOARD_X)
                {
                    top_line = i;
                    break;
                }
            }
            vec2<int> tetris_text_pos = vec2<int>{BOARD_X*BLOCK_SIZE_X/2, BLOCK_SIZE_Y * (top_line)} + BOARD_TOP_LEFT;
            tetris_text->SetPosition(tetris_text_pos);
            tetris_text->SetVisible(true);
            tetris_rectangle->SetPosition(tetris_text_pos);
            tetris_rectangle->SetVisible(true);
        }
        tetris_text->SetVisible(!tetris_text->GetVisible());
        steps++;
    }
    void StepRemoveLine()
    {
        assert(state == STATE::REMOVING_LINES);

        bool removed_line = false;
        for(int i = 0 ; i < BOARD_Y; i++)
        {
            if(line_fills[i] == BOARD_X) 
            {
                removed_line = true;
                for(; i >= 0; i--)
                {
                    for(int ii = 0; ii < BOARD_X; ii++)
                    {
                        if(i == 0)
                            SetBoard(ii, i, block());
                        else
                            SetBoard(ii, i, GetBoard(ii, i-1));
                    }
                    if(i == 0)
                        line_fills[i] = 0;
                    else
                        line_fills[i] = line_fills[i-1];
                }
                break;
            }
        }
        if(!removed_line)
        {
            state = STATE::NEW_PIECE_DELAY;
            AddScore(SCORE_LINE_DESTROYED);
        }
    }

    public:
    
    virtual bool Tick(float delta) override
    {
        if(exit)
        {
            return false;
        }
        if(gamepad == nullptr)
        {
            score_text->SetText("NO GAMEPAD");
            return true;
        }
        if(last_score != score)
        {
            std::string scorestr = std::to_string(score);
            while(scorestr.length() < SCORE_TEXT_LENGTH)
            {
                scorestr = "0" + scorestr;
            }
            score_text->SetText(scorestr);
            last_score = score;
        }
        timer += delta;
        if(last_score_added_time <= SCORE_LAST_ADDED_VISIBLE_TIME)
        {
            last_score_added_time += delta;
            std::string satext = "+" + std::to_string(last_score_added);
            while(satext.length() < SCORE_TEXT_LENGTH)
                satext = " " + satext;
            score_last_added_text->SetText(satext);
            score_last_added_text->SetVisible(true);
        }
        else
            score_last_added_text->SetVisible(false);
        bool board_changed = false;
        switch(state)
        {
            case STATE::START:
            {
                srand(get_time_us());
                timer = 0.0f;
                PlacePiece();
                board_changed = true;
                break;
            }
            case STATE::PIECE_STUCK:
            {
                timer = 0.0f;
                StepGame();
                board_changed = true;
                break;
            }
            case STATE::PLAYING:
            {
                game_time += delta;
                if(move_pressed)
                {
                    move_pressed = false;
                    board_changed = MovePiece(move_direction);
                }
                if(rotate_pressed)
                {
                    rotate_pressed = false;
                    if(state != STATE::PIECE_STUCK)
                        board_changed = board_changed || RotatePiece();
                }
                float steptm = PIECE_STEP_TIME;
                if(gamepad->IsButtonDown(Gamepad::LEFT_DOWN))
                    steptm /= 10.0f;

                float ratio = timer/steptm;
                if(ratio > 1.0f)
                    ratio = 1.0f;
                for(int i = 0; i < ArraySize(time_triangles); i++)
                {
                    float pratio = ratio - 0.25f*i;
                    if(pratio < 0.0f)
                    {
                        pratio = 0.0f;
                    }
                    pratio *= 4.0f;
                    if(pratio > 1.0f)
                        pratio = 1.0f;
                    float dir = M_PI_2 * (i-1);
                    auto p1  = (vec2<float>{cosf(dir + M_PI_2 * time_circle_color_polarity), sinf(dir + M_PI_2 * time_circle_color_polarity)}*sqrt(2)*TIME_CIRCLE_RADIUS);
                    auto p2 = vec2<float>{cosf(pratio * M_PI_2 + dir), sinf(pratio * M_PI_2 + dir)} 
                        * float(TIME_CIRCLE_RADIUS) * sqrt(2);
                    time_triangles[i]->SetP1(p1.convert<int>());
                    time_triangles[i]->SetP2(p2.convert<int>());
                }

                if(timer >= steptm)
                {
                    timer = 0.0f;
                    StepGame();
                    board_changed = true;
                    time_circle_color_polarity = !time_circle_color_polarity;
                }
                break;
            }
            case STATE::NEW_PIECE_DELAY:
            {
                if(timer >= NEW_PIECE_DELAY)
                {
                    timer = 0.0f;
                    PlacePiece();
                    board_changed = true;
                }
                break;
            }
            case STATE::EXPLODING:
            {
                if(timer >= EXPLOSION_STEP_TIME)
                {
                    timer = 0.0f;
                    StepExplosion();
                    board_changed = true;
                }
                break;
            }
            case STATE::TETRIS:
            {
                if(timer >= TETRIS_STEP_TIME)
                {
                    timer = 0.0f;
                    StepTestris();
                    board_changed = true;
                }
            }
            case STATE::REMOVING_LINES:
            {
                if(timer >= REMOVE_LINE_STEP_TIME)
                {
                    timer = 0.0f;
                    StepRemoveLine();
                    board_changed = true;
                }
                break;
            }
            case STATE::GAME_OVER:
            {
                break;
            }
            
        }
        if(board_changed)
        {
            std::vector<vec2<int>> positions[BLOCK_TYPES];
            for(int y = 0; y < BOARD_Y; y++)
            {
                for(int x = 0; x < BOARD_X; x++)
                {
                    auto b = GetBoard(x, y);
                    if(!b.IsEmpty())
                    {
                        auto pos = vec2<int>{x, y} * vec2<int>{BLOCK_SIZE_X, BLOCK_SIZE_Y};
                        positions[b.GetBlockIndex()].push_back(pos);
                        if(b.exploding)
                            positions[BLOCK_TYPES-1].push_back(pos);
                    }
                }
            }
            if(state == STATE::PLAYING)
            {
                auto blocks = piece.block_positions[rotation];
                for(int i = 0; i < blocks.size(); i++)
                {
                    positions[piece.type.GetBlockIndex()].push_back((blocks[i] + position) * vec2<int>{BLOCK_SIZE_X, BLOCK_SIZE_Y});
                }
            }
            if(state != STATE::GAME_OVER)
            {
                auto blocks = next_piece.block_positions[0];
                for(int i = 0; i < blocks.size(); i++)
                {
                    positions[next_piece.type.GetBlockIndex()].push_back(next_piece_center + blocks[i] * vec2<int>{BLOCK_SIZE_X, BLOCK_SIZE_Y} - BOARD_TOP_LEFT);
                }
            }
            for(int i = 0; i < BLOCK_TYPES; i++)
            {
                if(positions[i].size() == 0)
                {
                    block_sprites[i]->SetVisible(false);
                }
                else
                {
                    block_positions[i]->SetPoints(positions[i]);
                    block_sprites[i]->SetVisible(true);
                }
            }
        }
        return true;
    }
    virtual void Initialize() override
    {
        time_frame_circle = std::make_shared<EmptyCircle>(entity_manager, 255, EMPTY_CIRCLE_MODE::FULL, true, true, TIME_CIRCLE_FRAME_LAYER, time_circle_center, vec2<int>{TIME_CIRCLE_RADIUS, TIME_CIRCLE_RADIUS});
        time_circle = std::make_shared<Circle>(entity_manager, 255, 255, true, true, TIME_CIRCLE_LAYER, 0, time_circle_center, vec2<int>{TIME_CIRCLE_RADIUS, TIME_CIRCLE_RADIUS});
        for(int i = 0; i < ArraySize(time_triangles); i++)
            time_triangles[i] = std::make_shared<Triangle>(entity_manager, 0, vec2<int>{0, 0}, vec2<int>{0, 0}, 
                vec2<int>{0, 0}, false, true, TIME_TRIANGLE_LAYER, 0, time_circle_center, vec2<int>{1, 1});

        score_title_text = std::make_shared<Text>(entity_manager, text_manager, "SCORE", FONT::FIXED_10_20, TEXT_ALIGNMENT::CENTER, true, true, NORMAL_UI_LAYER, score_title_text_center);
        score_text = std::make_shared<Text>(entity_manager, text_manager, "0000000", FONT::FIXED_10_20, TEXT_ALIGNMENT::CENTER, true, true, NORMAL_UI_LAYER, score_text_center);
        score_last_added_text = std::make_shared<Text>(entity_manager, text_manager, "+0", FONT::FIXED_10_20, TEXT_ALIGNMENT::CENTER, true, false, NORMAL_UI_LAYER, score_added_text_center);

        tetris_text = std::make_shared<Text>(entity_manager, text_manager, "TETRIS", FONT::DEJAVUSERIF_F_16, TEXT_ALIGNMENT::CENTER, true, false, TETRIS_LAYER, vec2<int>{0, 0});
        tetris_rectangle = std::make_shared<Rectangle>(entity_manager, 255, 0, true, false, TETRIS_SQUARE_LAYER, 0, vec2<int>{0, 0}, BLOCK_SIZE * vec2<int>{BOARD_SIZE.x/2, 3});

        for(int i = 0; i < BLOCK_TYPES; i++)
        {
            block_textures[i] = texture_manager->CreateTextureFromTGA(texture_datas[i], texture_lens[i]);
            block_positions[i] = geometry_manager->AllocateGeomentry({vec2<int>{0, 0}});
            block_sprites[i] = std::shared_ptr<MultiSprite>(new MultiSprite(entity_manager, geometry_manager, block_textures[i], 
                block_positions[i], false, false, 0, BOARD_TOP_LEFT, false, NORMAL_BLOCK_LAYER));
        }
        block_sprites[BLOCK_TYPES-1]->SetLayer(EXPLOSION_BLOCK_LAYER);
        block_sprites[BLOCK_TYPES-1]->SetUseTransparency(true);
        block_sprites[BLOCK_TYPES-1]->SetTransparencyValue(0);
        std::vector<vec2<int>> board_lines;
        for(int i = 0; i < BOARD_Y+1; i++)
        {
            board_lines.push_back({0, BLOCK_SIZE_Y * i});
            board_lines.push_back({BLOCK_SIZE_X * BOARD_X, BLOCK_SIZE_Y * i});
        }
        for(int i = 0; i < BOARD_X+1; i++)
        {
            board_lines.push_back({BLOCK_SIZE_X * i, 0});
            board_lines.push_back({BLOCK_SIZE_X * i, BLOCK_SIZE_Y * BOARD_Y});
        }
        game_grid_lines = std::make_shared<MultiLines>(entity_manager, geometry_manager, 
            128, geometry_manager->AllocateGeomentry(board_lines), true, 0, 0, BOARD_TOP_LEFT);

        for(int i = 0; i < ArraySize(block_landing_indicators); i++)
        {
            block_landing_indicators[i] = std::make_shared<EmptyRectangle>(entity_manager, 255, false, false, INDICATOR_LAYER, 0, vec2<int>{0, 0}, vec2<int>{BLOCK_SIZE_X, BLOCK_SIZE_Y});
        }

        auto gamepads = gamepad_manager.GetGamepads();
        if(gamepads.size() > 0)
        {
            SetGamepad(gamepads.begin()->second);
        }
        gamepad_manager.AddOnGamepadAddedHandler([&](shared_ptr<Gamepad> gamepad)
        {
            SetGamepad(gamepad);
        });
        gamepad_manager.AddOnGamepadRemovedHandler([&](shared_ptr<Gamepad> gamepad)
        {
            if(this->gamepad == gamepad)
            {
                gamepad = nullptr;
                auto gamepads = gamepad_manager.GetGamepads();
                if(gamepads.size() > 0)
                {
                    SetGamepad(gamepads.begin()->second);
                }
            }
        });
    }
};