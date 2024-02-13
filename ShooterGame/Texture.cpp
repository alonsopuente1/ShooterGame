#include "Texture.h"

Texture::Texture() : mWidth(0), mHeight(0), mTex(nullptr)
{
}

Texture::~Texture()
{
    Free();
}

bool Texture::LoadTexture(SDL_Renderer* render, const char* path)
{
    if (render == nullptr)
    {
        std::cerr << "failed to load texture: " << std::endl;    
        std::cerr << "\terror: no renderer passed" << std::endl;
        return false;
    }

    Free();

    mTex = IMG_LoadTexture(render, path);

    if (!mTex)
    {
        std::cerr << "failed to load texture: " << std::endl;
        std::cerr << "\tpath: " << path << std::endl;
        std::cerr << "\tSDL_image error: " << IMG_GetError() << std::endl;
        return false;
    }

    if (SDL_QueryTexture(mTex, nullptr, nullptr, &mWidth, &mHeight) < 0)
    {
        std::cerr << "failed to load texture: " << std::endl;
        std::cerr << "\terror: couldnt query texture" << std::endl;
        std::cerr << "\tSDL error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void Texture::Free()
{
    if(mTex)
        SDL_DestroyTexture(mTex);
}

int Texture::GetWidth() const
{
    return mWidth;
}

int Texture::GetHeight() const
{
    return mHeight;
}

SDL_Texture* Texture::GetTexture()
{
    return mTex;
}
