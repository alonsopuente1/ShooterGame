#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class Texture
{

public:

    Texture();
    ~Texture();

    bool LoadTexture(SDL_Renderer* render, const char* path);

    void Free();

    // getters
    int GetWidth() const;
    int GetHeight() const;

    SDL_Texture* GetTexture();

private:

    SDL_Texture* mTex;

    int mWidth;
    int mHeight;

};