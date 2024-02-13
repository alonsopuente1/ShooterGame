#pragma once

#include <math.h>

class vec2
{
public:

    vec2();
    vec2(float a);
    vec2(float _x, float _y);
    
    vec2 operator+(const vec2& other);
    vec2 operator-(const vec2& other);
    vec2 operator*(const float& other);
    vec2 operator/(const float& other);
       
    vec2& operator+=(const vec2& other);
    vec2& operator-=(const vec2& other);
    vec2& operator*=(const float& other);
    vec2& operator/=(const float& other);

    void operator=(const vec2& other);

    float mag() const;
    void setmag(float mag);

    void normalise();

    static vec2 angtovec(float a);
    static vec2 angtovec(float a, float mag);

    float x, y;

};

class vec3
{
public:

    vec3();
    vec3(float a);
    vec3(float _x, float _y, float _z);

    vec3& operator+(const vec3& other);
    vec3& operator-(const vec3& other);
    vec3& operator*(const float& other);
    vec3& operator/(const float& other);

    vec3& operator+=(const vec3& other);
    vec3& operator-=(const vec3& other);
    vec3& operator*=(const float& other);
    vec3& operator/=(const float& other);
    
    void operator=(const vec3& other);

    float mag() const;

    void normalise();

    float x, y, z;

};