#pragma once

#include <iostream>
#include <vector>

#include "vec3.h"

void trim(std::string& string);

void trim(std::vector<std::string>& strings);

std::vector<std::string> split(const std::string& in, const std::string& token);
std::vector<std::string> split(const std::string& in, const std::vector<std::string>& tokens);

std::string resolvefilepath(std::string a, std::string b);

template <typename T>
void clamp(T& val, T min, T max)
{
    if (val < min)
        val = min;
    else if (val > max)
        val = max;
}

void normalise(float& x, float& y);

float radstodeg(float rads);

vec2 randomvec();
