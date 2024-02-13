#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <array>
#include <iostream>

#include "Texture.h"

namespace TextureManager
{
    enum Textures : int
    {
        Spritesheet1,
        MainPlayer,
        Zombie,
        Bullet
    };

    extern std::array<Texture, 8> gTextures;
    
    void LoadTextures(SDL_Renderer* render);
    void CleanUp();
}