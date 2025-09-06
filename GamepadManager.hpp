#pragma once
#include "hmain.hpp"

class GamepadManager
{
    protected:
    std::map<std::string, shared_ptr<Gamepad>> gamepads;
    std::vector<std::function<void(shared_ptr<Gamepad>)>> OnGamepadRemovedHandlers;
    std::vector<std::function<void(shared_ptr<Gamepad>)>> OnGamepadAddedHandlers;
    public:
    void AddOnGamepadRemovedHandler(std::function<void(shared_ptr<Gamepad>)> f)
    {
        OnGamepadRemovedHandlers.push_back(f);
    }
    void AddOnGamepadAddedHandler(std::function<void(shared_ptr<Gamepad>)> f)
    {
        OnGamepadAddedHandlers.push_back(f);
    }
    std::map<std::string, shared_ptr<Gamepad>> GetGamepads()
    {
        return gamepads;
    }
    shared_ptr<Gamepad> CreateGamepad(std::string id)
    {
        auto gamepad = std::make_shared<Gamepad>(id);
        gamepads.insert(std::make_pair(id, gamepad));
        for(auto it : OnGamepadAddedHandlers)
        {
            it(gamepad);
        }
        return gamepad;
    }
    shared_ptr<Gamepad> GetGamepad(std::string id)
    {
        assert(gamepads.find(id) != gamepads.end());
        return gamepads[id];
    }
    void DeleteGamepad(std::string id)
    {
        assert(gamepads.find(id) != gamepads.end());
        auto gamepad = gamepads[id];
        gamepads.erase(id);
        for(auto it : OnGamepadRemovedHandlers)
        {
            it(gamepad);
        }
    }
    bool HasGamepad(std::string id)
    {
        return gamepads.find(id) != gamepads.end();
    }
};

inline GamepadManager gamepad_manager = GamepadManager();