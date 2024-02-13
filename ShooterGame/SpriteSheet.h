#pragma once

#include <SDL_image.h>
#include <iostream>

class SpriteSheet
{
public:
    SpriteSheet();
    SpriteSheet(SDL_Renderer* render, const char* path, unsigned int cols, unsigned int rows);
    ~SpriteSheet();

    void SelectSprite(int x, int y);
    void SelectSprite(int i);

    void DrawSprite(SDL_FRect* dst);

    void AttachRenderer(SDL_Renderer* render);

    // Returns true on success: loads spritesheet on renderer attached
    bool LoadSpritesheet(const char* path, unsigned int cols, unsigned int rows);
    
    // Returns true on success: loads spritesheet on renderer passed through
    bool LoadSpritesheet(SDL_Renderer* render, const char* path, unsigned int cols, unsigned int rows);

    void Free();


    // getters
    int GetNumSprites() const;

    int GetTexWidth() const;
    int GetTexHeight() const;

    int GetRows() const;
    int GetCols() const;

    SDL_Texture* GetTexture() const;

    std::string GetLastPathLoaded() const;

    SDL_Renderer* GetRendererAttached() const;

    bool Ready() const;

private:
    SDL_Renderer*& mRender;

    SDL_Texture* mSpritesheetTex;

    int mTexWidth;
    int mTexHeight;

    int mCols;
    int mRows;

    int mCurX;
    int mCurY;

    bool mInit;

    std::string mLastPathLoaded;
};

