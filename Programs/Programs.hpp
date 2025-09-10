#pragma once

template<typename P>
Program* CreateProgram()
{
    return new P();
}

struct ProgramDescription
{
    std::string name;
    std::string description;
    std::function<Program*()> generator;
};

inline ProgramDescription programs[] = 
{
    {
        "Basic Test",
        "A basic rendering test",
        CreateProgram<BasicTestProgram>
    },
    {
        "Pong",
        "A game of Pong",
        CreateProgram<PongProgram>
    },
    {
        "Tetris",
        "A game of Tetris",
        CreateProgram<TetrisProgram>
    }
};