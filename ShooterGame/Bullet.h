#pragma once

#include "Entity.h"

class Bullet : public Entity
{

public:

    Bullet(vec2 pos, vec2 vel);
    ~Bullet();

    bool CheckCollision(const Entity& e);
    void Damage(Entity& e);
    

private:

    float mDamage;

};