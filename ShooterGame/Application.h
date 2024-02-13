#pragma once

#include <sstream>

#include "Window.h"
#include "Map.h"
#include "SpriteSheet.h"
#include "Player.h"
#include "Util.h"
#include "vec3.h"
#include "TextureManager.h"
#include "Bullet.h"

class Application
{

public:

    Application();
    ~Application();

    void Run(int targetFPS = 60);

private:

    void HandleEvents();
    void Update(unsigned int dtMs);
    void Draw();

    SDL_FRect GetMapToScreenRect(int x, int y) const;

    Window mWindow;

    std::string mTitle;

    SpriteSheet mSpritesheet;
    Map mMap;
    Player mPlayer;

    std::vector<Entity> mEntities;
    std::vector<Bullet> mBullets;

    bool mWPressed;
    bool mAPressed;
    bool mSPressed;
    bool mDPressed;

    float mZoomMag = 1.f;
    
    int mSquareWidth = 50.f;

    float mTimerMs;

    bool mLoop;
    bool mViewHitbox;
};