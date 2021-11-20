#pragma once

#ifndef ENGINE_HPP

#ifndef OLC_PGE_APPLICATION
#define OLC_PGE_APPLICATION
#endif

#ifndef OLC_INPUTMAP
#define OLC_INPUTMAP
#endif

#include "olcPixelGameEngine.h"
#include "olcPGEX_InputMap.h"

#include "Entity.hpp"

class Engine : public olc::PixelGameEngine
{
public:
    Engine()
    {
        PixelScale = std::max(ActualWindowSize.x / CanvasSize.x, ActualWindowSize.y / CanvasSize.y);
    }
    ~Engine() {}

private:
    bool OnUserCreate() override { return true; }
    bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(olc::BLANK);

        e.Update(fElapsedTime);
        e.Draw(this);

        return true;
    }

public:
    olc::vf2d CanvasSize = {320, 180}, ActualWindowSize = {1024, 576};
    float PixelScale = 0.0f;
    Entity e;
};

#endif