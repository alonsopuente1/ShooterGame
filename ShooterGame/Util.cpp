#include "Util.h"

void trim(std::string& string)
{
    bool foundleft = false;
    bool foundright = false;

    int leftindex = -1;
    int rightindex = -1;

    for (int i = 0; i < string.size(); i++)
    {
        if (!foundleft)
        {
            if (string[i] != ' ' && string[i] != '\t')
            {
                foundleft = true;
                leftindex = i;
            }
        }

        if (!foundright)
        {
            if (string[string.size() - i - 1] != ' ' && string[string.size() - i - 1] != '\t')
            {
                foundright = true;
                rightindex = string.size() - i - 1;
            }
        }

    }

    if (leftindex < 0 || rightindex < 0)
    {
        string = "";
        return;
    }

    string = string.substr(leftindex, rightindex - leftindex + 1);
}

void trim(std::vector<std::string>& strings)
{
    for (std::string& str : strings)
    {
        trim(str);
    }
}

std::vector<std::string> split(const std::string& in, const std::vector<std::string>& tokens)
{
    std::vector<std::string> out;

    out.clear();

    std::string temp;

    bool addChar = true;

    for (int i = 0; i < int(in.size()); i++)
    {
        for (const std::string& token : tokens)
        {
            addChar = true;
            std::string test = in.substr(i, token.size());

            if (test == token)
            {
                if (!temp.empty())
                {
                    out.push_back(temp);
                    temp.clear();
                    i += (int)token.size() - 1;
                    addChar = false;
                    break;
                }
                else
                {
                    out.push_back("");
                }
            }
            else if (i + token.size() >= in.size())
            {
                temp += in.substr(i, token.size());
                out.push_back(temp);
                i = in.size();
                addChar = false;
                break;
            }
        }

        if (addChar)
        {
            temp += in[i];
        }
    }

    trim(out);

    return out;
}

std::vector<std::string> split(const std::string& in, const std::string& token)
{
    std::vector<std::string> out;

    out.clear();

    std::string temp;

    for (int i = 0; i < int(in.size()); i++)
    {
        std::string test = in.substr(i, token.size());

        if (test == token)
        {
            if (!temp.empty())
            {
                out.push_back(temp);
                temp.clear();
                i += (int)token.size() - 1;
            }
            else
            {
                out.push_back("");
            }
        }
        else if (i + token.size() >= in.size())
        {
            temp += in.substr(i, token.size());
            out.push_back(temp);
            break;
        }
        else
        {
            temp += in[i];
        }
    }

    trim(out);

    return out;
}

std::string resolvefilepath(std::string a, std::string b)
{
    std::vector<std::string> tokens = { "/", "\\" };
    std::vector<std::string> tokensA = split(a, tokens);
    std::vector<std::string> tokensB = split(b, tokens);

    std::vector<std::string> output = tokensA;



    return "";
}



void normalise(float& x, float& y)
{
    float mag = sqrtf(x * x + y * y);

    if (mag == 0)
    {
        return;
    }

    x /= mag;
    y /= mag;
}

float radstodeg(float rads)
{
    return rads * (180.f / acos(-1));
}

vec2 randomvec()
{
    vec2 out(((float)rand() / (float)RAND_MAX) * 2.f - 1.f, ((float)rand() / (float)RAND_MAX) * 2.f - 1.f);
    out.normalise();
    return out;
}
