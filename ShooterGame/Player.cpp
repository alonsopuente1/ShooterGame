#include "Player.h"

Player::Player()
{
}

Player::Player(float x, float y)
{
    mPos.x = x;
    mPos.y = y;
}

Player::~Player()
{
}

void Player::SetPlayerSpeed(float s)
{
    mPlayerSpeed = s;
}

float Player::GetPlayerSpeed() const
{
    return mPlayerSpeed;
}
