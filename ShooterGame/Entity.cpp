#include "Entity.h"

Entity::Entity()
{
}

Entity::~Entity()
{
}

void Entity::Draw(SDL_Renderer* render, SDL_FPoint p, float scale)
{
    if (!mTex)
    {
        std::cout << "warning: trying to draw entity without texture" << std::endl;
        return;
    }

    float texWidth = mTex->GetWidth() * scale;
    float texHeight = mTex->GetHeight() * scale;


    SDL_FRect dst = { p.x - texWidth / 2.f, p.y - texHeight / 2.f, texWidth, texHeight };

    if (SDL_RenderCopyExF(render, mTex->GetTexture(), nullptr, &dst, radstodeg(mAngle), nullptr, SDL_FLIP_NONE) < 0)
    {
        std::cerr << "failed to render entity: " << std::endl;
        std::cerr << "\tSDL error: " << SDL_GetError() << std::endl;
    }
}

void Entity::DrawHitbox(SDL_Renderer* render, SDL_FPoint p, float scale)
{
    SDL_SetRenderDrawColor(render, 0xff, 0x00, 0x00, 0xff);

    if (!mHitbox.mag())
    {
        return;
    }

    SDL_Rect dst = { p.x - mHitbox.x / 2.f * scale, p.y - mHitbox.y / 2.f * scale, mHitbox.x * scale, mHitbox.y * scale };

    if (SDL_RenderDrawRect(render, &dst) < 0)
    {
        std::cerr << "failed to render hitbox: " << std::endl;
        std::cerr << "\tSDL error: " << SDL_GetError() << std::endl;
    }
}

void Entity::DrawHealth(SDL_Renderer* render, SDL_FPoint p, float scale)
{
    SDL_SetRenderDrawColor(render, 0xff, 0x00, 0x00, 0xff);

    float healthProp = mHealth / mMaxHealth;
    float healthWidth = 50.f * scale;
    float healthHeight = 5.f * scale;

    SDL_FRect dst = { p.x - healthWidth / 2.f, p.y - healthHeight / 2.f, healthWidth, healthHeight };

    SDL_RenderFillRectF(render, &dst);
    SDL_SetRenderDrawColor(render, 0x00, 0xff, 0x00, 0xff);
    dst.w = healthProp * healthWidth;
    SDL_RenderFillRectF(render, &dst);
}

void Entity::Update(float dtS, Map* map)
{
    mCollided = false;
    mVel += mAcc * dtS;

    if (!map)
    {
        mPos += mVel * dtS;
        return;
    }
    
    vec2 deltaPos = mVel * dtS;
    
    for (int i = 0; i < map->GetRows(); i++)
    {
        for (int j = 0; j < map->GetCols(); j++)
        {
            if (deltaPos.x == 0 && deltaPos.y == 0)
                goto outfor;

            int sprite = (*map)[j][i];

            if (sprite != 0)
                continue;

            SDL_FRect entityHitbox = { mPos.x - mHitbox.x / 2.f, mPos.y - mHitbox.y / 2.f, mHitbox.x, mHitbox.y };
            SDL_FRect mapHitbox = { j, i, 1.f, 1.f };
            vec2 cpydeltaPos = deltaPos;

            SDL_FRect newEntityHitbox = entityHitbox;
            newEntityHitbox.x += deltaPos.x;
            
            if (Physics::RectCollision(newEntityHitbox, mapHitbox))
            {
                deltaPos.x = 0.f;
                mCollided = true;
            }

            newEntityHitbox.x -= cpydeltaPos.x;

            newEntityHitbox.y += cpydeltaPos.y;

            if (Physics::RectCollision(newEntityHitbox, mapHitbox))
            {
                deltaPos.y = 0.f;
                mCollided = true;
            }

        }
    }
outfor:

    mPos += deltaPos;
}

void Entity::AddPos(vec2 pos)
{
    mPos += pos;
}

void Entity::AddVel(vec2 vel)
{
    mVel += vel;
}

void Entity::AddAngle(float a)
{
    mAngle += a;
}

void Entity::AddHealth(float h)
{
    mHealth += h;
}

void Entity::SetHealth(float h)
{
    mHealth = h;
    mMaxHealth = h;
}

void Entity::SetVel(vec2 a)
{
    mVel = a;
}

void Entity::SetPos(vec2 a)
{
    mPos = a;
}

void Entity::SetAngle(float a)
{
    mAngle = a;
}

void Entity::SetHitbox(vec2 in)
{
    mHitbox = in;
}

void Entity::SetTexture(Texture* in)
{
    mTex = in;
}

vec2 Entity::GetPos() const
{
    return mPos;
}

vec2 Entity::GetVel() const
{
    return mVel;
}

float Entity::GetHealth()
{
    return mHealth;
}

float Entity::GetAngle() const
{
    return mAngle;
}

vec2 Entity::GetHitbox() const
{
    return mHitbox;
}

Texture* Entity::GetTexture()
{
    return mTex;
}

bool Entity::HasCollided() const
{
    return mCollided;
}



