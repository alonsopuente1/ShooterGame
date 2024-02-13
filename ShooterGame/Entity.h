#pragma once

#include <SDL.h>

#include "vec3.h"
#include "Texture.h"
#include "Util.h"
#include "Map.h"
#include "PhysicsEngine.h"

class Entity 
{
   
public:

    Entity();
    ~Entity();

    void Draw(SDL_Renderer* render, SDL_FPoint p, float scale);
    void DrawHitbox(SDL_Renderer* render, SDL_FPoint p, float scale);
    void DrawHealth(SDL_Renderer* render, SDL_FPoint p, float scale);

    void Update(float dtS, Map* map);

    void AddPos(vec2 pos);
    void AddVel(vec2 vel);
    void AddAngle(float a);
    void AddHealth(float h);

    // setters

    void SetHealth(float h);

    void SetVel(vec2 a);
    void SetPos(vec2 a);

    void SetAngle(float a);

    void SetHitbox(vec2 in);
    
    void SetTexture(Texture* in);

    // getters

    vec2 GetPos() const;
    vec2 GetVel() const;
    float GetHealth();

    float GetAngle() const;

    vec2 GetHitbox() const;

    Texture* GetTexture();

    bool HasCollided() const;

protected:

    vec2 mAcc;
    vec2 mVel;
    vec2 mPos;

    vec2 mHitbox;

    float mAngle;

    Texture* mTex;

    float mHealth;
    float mMaxHealth;

    bool mCollided = false;
};
