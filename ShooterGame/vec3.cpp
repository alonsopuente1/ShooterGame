#include "vec3.h"

vec3::vec3() : x(0.f), y(0.f), z(0.f)
{
}

vec3::vec3(float a) : x(a), y(a), z(a)
{
}

vec3::vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
{
}

vec3& vec3::operator+(const vec3& other)
{
    vec3 out(other.x + x, other.y + y, other.z + z);
    return out;
}

vec3& vec3::operator-(const vec3& other)
{
    vec3 out(x - other.x, y - other.y, z - other.z);
    return out;
}

vec3& vec3::operator*(const float& other)
{
    vec3 out(x * other, y * other, z * other);
    return out;
}

vec3& vec3::operator/(const float& other)
{
    vec3 out(x / other, y / other, z / other);
    return out;
}

vec3& vec3::operator+=(const vec3& other)
{
    *this = *this + other;
    return *this;
}

vec3& vec3::operator-=(const vec3& other)
{
    *this = *this - other;
    return *this;
}

vec3& vec3::operator*=(const float& other)
{
    *this = *this * other;
    return *this;
}

vec3& vec3::operator/=(const float& other)
{
    *this = *this / other;
    return *this;
}

void vec3::operator=(const vec3& other)
{
    x = other.x;
    y = other.y;
    z = other.z;
}

float vec3::mag() const
{
    return sqrtf(x * x + y * y + z*z);
}

void vec3::normalise()
{
    float m = mag();

    if (m == 0)
        return;

    x /= m;
    y /= m;
}


vec2::vec2() : x(0.f), y(0.f)
{
}

vec2::vec2(float a) : x(a), y(a)
{
}

vec2::vec2(float _x, float _y) : x(_x), y(_y)
{
}

vec2 vec2::operator+(const vec2& other)
{
    vec2 out(other.x + x, other.y + y);
    return out;
}

vec2 vec2::operator-(const vec2& other)
{
    vec2 out(x - other.x, y - other.y);
    return out;
}

vec2 vec2::operator*(const float& other)
{
    vec2 out(x * other, y * other);
    return out;
}

vec2 vec2::operator/(const float& other)
{
    vec2 out(x / other, y / other);
    return out;
}

vec2& vec2::operator+=(const vec2& other)
{
    *this = *this + other;
    return *this;
}

vec2& vec2::operator-=(const vec2& other)
{
    *this = *this - other;
    return *this;
}

vec2& vec2::operator*=(const float& other)
{
    *this = *this * other;
    return *this;
}

vec2& vec2::operator/=(const float& other)
{
    *this = *this / other;
    return *this;
}

void vec2::operator=(const vec2& other)
{
    x = other.x;
    y = other.y;
}

float vec2::mag() const
{
    return sqrtf(x * x + y * y);
}

void vec2::setmag(float mag)
{
    normalise();
    *this *= mag;
}

void vec2::normalise()
{
    float m = mag();

    if (m == 0)
        return;

    x /= m;
    y /= m;
}

vec2 vec2::angtovec(float a)
{
    return vec2(cosf(a), sinf(a));
}

vec2 vec2::angtovec(float a, float mag)
{
    vec2 out = angtovec(a);
    out.setmag(mag);
    return out;
}



