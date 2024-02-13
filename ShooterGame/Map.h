#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "SpriteSheet.h"
#include "Util.h"
#include "vec3.h"

struct MapInfo
{
    vec2 playerStartPos;
    float playerSpeed;
    vec2 playerHitbox;
    float playerHealth;

    std::vector<vec2> zombieSpawns;
    vec2 zombieHitbox;
    float zombieSpeed;
    float zombieHealth;
};

class Map
{
public:
    Map();
    ~Map();

    void Create(unsigned int cols, unsigned int rows);
    void Set(int newItem, int col, int row);
    void Set(int newItem, int index);
    
    bool LoadMap(const char* filepath, MapInfo* mapinfo);

    // true if successful
    bool ExportMap(const char* dstpath, const char* sprsheetpath);

    bool Ready() const;

    int GetRows() const;
    int GetCols() const;

    void Free();

    std::vector<int>& operator[] (unsigned int col);
private:
    bool ReadFile(const char* path, MapInfo* mapinfo);

    int mCols, mRows;

    bool mInit;

    std::vector<std::vector<int>> mSpriteMap;
};

