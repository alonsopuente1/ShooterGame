#include "PhysicsEngine.h"

bool Physics::RectCollision(SDL_Rect a, SDL_Rect b)
{
    return (a.x + a.w > b.x && a.x < b.x + b.w && a.y < b.y + b.h && a.y + a.h > b.y);
}

bool Physics::RectCollision(SDL_FRect a, SDL_FRect b)
{
    return (a.x + a.w > b.x && a.x < b.x + b.w && a.y < b.y + b.h && a.y + a.h > b.y);
}
