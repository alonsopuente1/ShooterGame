#pragma once

#include <SDL.h>

namespace Physics
{
    bool RectCollision(SDL_Rect a, SDL_Rect b);
    bool RectCollision(SDL_FRect a, SDL_FRect b);


}