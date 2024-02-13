#include "Application.h"

Application::Application() : mLoop(false),
                            mTitle("Clippy Crawler"),
                            mPlayer(10.0f, 10.0f),
                            mWPressed(false),
                            mSPressed(false),
                            mAPressed(false),
                            mDPressed(false),
                            mViewHitbox(true)
{
    srand(time(nullptr));

    if (SDL_Init(SDL_INIT_VIDEO) < 0)    
    {
        std::cerr << "failed to init sdl2. error: " << SDL_GetError() << std::endl;
        return;
    }

    if (!mWindow.Create(mTitle.c_str(), 1280, 720))
    {
        std::cerr << "failed to create window" << std::endl;
        return;
    }

    TextureManager::LoadTextures(mWindow.GetRenderer());

    if (!mSpritesheet.LoadSpritesheet(mWindow.GetRenderer(), "res/spritesheets/textures1.png", 8, 8))
    {
        std::cerr << "failed to load spritesheet" << std::endl;
        return;
    }

    MapInfo mi = { 0 };

    if (!mMap.LoadMap("level/map1.ssm", &mi))
    {
        std::cerr << "failed to load map" << std::endl;
        return;
    }

    mPlayer.SetPos(mi.playerStartPos);
    mPlayer.SetPlayerSpeed(mi.playerSpeed);
    mPlayer.SetHitbox(mi.playerHitbox);
    mPlayer.SetTexture(&TextureManager::gTextures[TextureManager::Textures::MainPlayer]);
    mPlayer.SetHealth(mi.playerHealth);

    mEntities.resize(mi.zombieSpawns.size());
    for (int i = 0; i < mi.zombieSpawns.size(); i++)
    {
        mEntities[i] = Entity();
        mEntities[i].SetPos(mi.zombieSpawns[i]);
        mEntities[i].SetHitbox(mi.zombieHitbox);
        mEntities[i].SetTexture(&TextureManager::gTextures[TextureManager::Textures::Zombie]);
        mEntities[i].SetHealth(mi.zombieHealth);
    }

    mLoop = true;
}

Application::~Application()
{
    mWindow.Free();
    TextureManager::CleanUp();

    SDL_Quit();
}

void Application::Run(int targetFPS)
{
    int oldTime = 0;
    while (mLoop)
    {
        SDL_Delay(1000 / targetFPS);

        int newTime = SDL_GetTicks();
        int dtMs = newTime - oldTime;
        oldTime = newTime;

        HandleEvents();
        Update(dtMs);
        Draw();
    }
}

void Application::HandleEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        mWindow.HandleEvent(event);
        
        switch (event.type)
        {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
        {

            const Uint8* keyState = SDL_GetKeyboardState(nullptr);

            if (keyState[SDL_SCANCODE_ESCAPE])
            {
                mWindow.HideWindow();
                return;
            }

            mWPressed = keyState[SDL_SCANCODE_W];
            mAPressed = keyState[SDL_SCANCODE_A];
            mSPressed = keyState[SDL_SCANCODE_S];
            mDPressed = keyState[SDL_SCANCODE_D];

            if (keyState[SDL_SCANCODE_H])
                mViewHitbox = !mViewHitbox;

            break;
        }

        case SDL_MOUSEWHEEL:
        {
            mZoomMag += event.wheel.y * 0.25f;

            clamp<float>(mZoomMag, 1.f, 2.f);

            break;
        }

        case SDL_MOUSEBUTTONDOWN:
        {
            mBullets.push_back(Bullet(mPlayer.GetPos(), vec2::angtovec(mPlayer.GetAngle(), 10.f)));
            mBullets[mBullets.size() - 1].SetTexture(&TextureManager::gTextures[TextureManager::Textures::Bullet]);
            mBullets[mBullets.size() - 1].SetAngle(mPlayer.GetAngle());

            break;
        }

        }
    }
}

void Application::Update(unsigned int dtMs)
{
    mLoop = mWindow.GetShown();

    clamp<UINT32>(dtMs, (UINT32)1, MAXUINT32);

    int fps = 1000 / dtMs;

    std::stringstream newTitle;

    newTitle << mTitle << " FPS - " << fps;

    mWindow.SetWindowTitle(newTitle.str().c_str());

    float dtS = (float)dtMs / 1000.f;

    if ((mTimerMs += dtMs) > 3000.f)
    {
        for (Entity& e : mEntities)
            e.SetVel(randomvec());
        mTimerMs = 0.f;
    }

    vec2 newVel;

    if (mAPressed)
    {
        newVel += vec2{-1.f, 0.f };
    }
    if (mSPressed)
    {
        newVel += vec2{ 0.f, 1.f };
    }
    if (mWPressed)
    {
        newVel += vec2{ 0.f, -1.f };
    }
    if (mDPressed)
    {
        newVel += vec2{ 1.f, 0.f };
    }

    newVel.normalise();
    newVel.setmag(mPlayer.GetPlayerSpeed());
    mPlayer.SetVel(newVel);

    mPlayer.Update(dtS, &mMap);

    for (Entity& e : mEntities)
    {
        e.Update(dtS, &mMap);
    }

    for (int i = 0; i < mBullets.size(); i++)
    {
        mBullets[i].Update(dtS, &mMap);
        if (mBullets[i].HasCollided())
        {
            mBullets.erase(mBullets.begin() + i);
            continue;
        }
        for(Entity& e : mEntities)
            if (mBullets[i].CheckCollision(e))
            {
                mBullets[i].Damage(e);
                mBullets.erase(mBullets.begin() + i);
                break;
            }
    }

    for (int i = 0; i < mEntities.size(); i++)
    {
        if (mEntities[i].GetHealth() <= 0)
        {
            mEntities.erase(mEntities.begin() + i);
        }
    }

    int x, y;
    SDL_GetMouseState(&x, &y);

    x -= mWindow.GetWidth() / 2;
    y -= mWindow.GetHeight() / 2;

    mPlayer.SetAngle(atan2f((float)y, (float)x));
}

void Application::Draw()
{

    SDL_SetRenderDrawColor(mWindow.GetRenderer(), 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(mWindow.GetRenderer());

    int width = mSquareWidth * mZoomMag;
    int height = mSquareWidth * mZoomMag;

    if (mMap.Ready() && mSpritesheet.Ready())
    {
        for (int i = 0; i < mMap.GetRows(); i++)
        {
            for (int j = 0; j < mMap.GetCols(); j++)
            {
                SDL_FRect dst = GetMapToScreenRect(j, i);

                mSpritesheet.SelectSprite(mMap[j][i]);
                mSpritesheet.DrawSprite(&dst);
            }
        }
    }

    float playerScale = 0.2f;
    float zombieScale = 0.2f;
    float bulletScale = 0.03f;
    
    int scrWidth = mWindow.GetWidth();
    int scrHeight = mWindow.GetHeight();

    mPlayer.Draw(mWindow.GetRenderer(), SDL_FPoint {(float)scrWidth / 2.f, (float)scrHeight / 2.f}, playerScale * mZoomMag);
    mPlayer.DrawHealth(mWindow.GetRenderer(), SDL_FPoint{ (float)scrWidth / 2.f, (float)scrHeight / 2.f + 20.f * mZoomMag}, mZoomMag);

    for(Entity& e : mEntities)
        e.Draw(mWindow.GetRenderer(), SDL_FPoint{ e.GetPos().x * (float)width - mPlayer.GetPos().x * width + mWindow.GetWidth() / 2.f, e.GetPos().y * (float)height - mPlayer.GetPos().y * height + mWindow.GetHeight() / 2}, zombieScale * mZoomMag);
    for (Bullet& b : mBullets)
        b.Draw(mWindow.GetRenderer(), SDL_FPoint{ b.GetPos().x * (float)width - mPlayer.GetPos().x * width + mWindow.GetWidth() / 2.f, b.GetPos().y * (float)height - mPlayer.GetPos().y * height + mWindow.GetHeight() / 2.f }, bulletScale * mZoomMag);
    for (Entity& e : mEntities)
        e.DrawHealth(mWindow.GetRenderer(), SDL_FPoint{ e.GetPos().x * (float)width - mPlayer.GetPos().x * width + mWindow.GetWidth() / 2.f, e.GetPos().y * (float)height - mPlayer.GetPos().y * height + mWindow.GetHeight() / 2.f + 25.f * mZoomMag }, mZoomMag);

    if (mViewHitbox)
    {
        mPlayer.DrawHitbox(mWindow.GetRenderer(), SDL_FPoint {(float)scrWidth / 2.f, (float)scrHeight / 2.f}, width);
        for(Entity& e : mEntities)
            e.DrawHitbox(mWindow.GetRenderer(), SDL_FPoint{ e.GetPos().x * (float)width - mPlayer.GetPos().x * width + mWindow.GetWidth() / 2.f, e.GetPos().y * (float)height - mPlayer.GetPos().y * height + mWindow.GetHeight() / 2 }, width);
        for (Bullet& b : mBullets)
            b.DrawHitbox(mWindow.GetRenderer(), SDL_FPoint{ b.GetPos().x * (float)width - mPlayer.GetPos().x * width + mWindow.GetWidth() / 2.f, b.GetPos().y * (float)height - mPlayer.GetPos().y * height + mWindow.GetHeight() / 2 }, width);
    }

    SDL_RenderPresent(mWindow.GetRenderer());
}

SDL_FRect Application::GetMapToScreenRect(int x, int y) const
{
    SDL_FRect out;

    float scale = mZoomMag * mSquareWidth;

    out = { x * scale - mPlayer.GetPos().x * scale + mWindow.GetWidth() / 2, y * scale - mPlayer.GetPos().y * scale + mWindow.GetHeight() / 2, scale, scale };
    
    return out;
}
