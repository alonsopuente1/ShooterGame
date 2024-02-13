#include "Bullet.h"

Bullet::Bullet(vec2 pos, vec2 vel) : mDamage(10.f)
{
    mPos = pos; mVel = vel;
    mHitbox = vec2(0.2f, 0.2f);
}

Bullet::~Bullet()
{
}

bool Bullet::CheckCollision(const Entity& e)
{
    SDL_FRect entityHitbox = {e.GetPos().x - e.GetHitbox().x / 2.f, e.GetPos().y - e.GetHitbox().y / 2.f, e.GetHitbox().x, e.GetHitbox().y};

    SDL_FRect thisHitbox = { mPos.x - mHitbox.x / 2.f, mPos.y - mHitbox.y / 2.f, mHitbox.x, mHitbox.y };

    if (Physics::RectCollision(entityHitbox, thisHitbox))
    {
        return true;
    }
    return false;
}

void Bullet::Damage(Entity& e)
{
    e.AddHealth(-mDamage);
}
