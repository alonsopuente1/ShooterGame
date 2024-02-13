#include "TextureManager.h"

namespace TextureManager
{
    std::array<Texture, 8> gTextures;
}

void TextureManager::LoadTextures(SDL_Renderer* render)
{
    if(!gTextures[Spritesheet1].LoadTexture(render, "res/spritesheets/textures1.png"))
    {
        std::cerr << "failed to load spritesheet texture" << std::endl;
    }

    if(!gTextures[MainPlayer].LoadTexture(render, "res/textures/player.png"))
    {
        std::cerr << "failed to load player texture" << std::endl;
    }

    if (!gTextures[Zombie].LoadTexture(render, "res/textures/zombie.png"))
    {
        std::cerr << "failed to load zombie texture" << std::endl;
    }

    if (!gTextures[Bullet].LoadTexture(render, "res/textures/bullet.png"))
    {
        std::cerr << "failed to load bullet texture" << std::endl;
    }
}

void TextureManager::CleanUp()
{
    for (Texture& tex : gTextures)
    {
        tex.Free();
    }
}
