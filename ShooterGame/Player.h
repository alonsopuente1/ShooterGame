#pragma once

#include "Texture.h"
#include "Util.h"
#include "vec3.h"

#include "Entity.h"

class Player : public Entity
{

public:

    Player();
    Player(float x, float y);

    ~Player();

    void LoadTextures(SDL_Renderer* render);

    // setters
    void SetPlayerSpeed(float s);

    // getters
    float GetPlayerSpeed() const;


private:

    float mPlayerSpeed = 10.f;

    float mAngle = 0.0;
};