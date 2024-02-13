#include "SpriteSheet.h"

SDL_Renderer* nothing = nullptr;

SpriteSheet::SpriteSheet() : mLastPathLoaded(""), mInit(false), mRender(nothing)
{
}

SpriteSheet::SpriteSheet(SDL_Renderer* render, const char* path, unsigned int cols, unsigned int rows) : mRender(render)
{
    LoadSpritesheet(path, cols, rows);
}

SpriteSheet::~SpriteSheet() 
{
    Free();
}

void SpriteSheet::SelectSprite(int x, int y)
{
    if (x < 0 || x > mCols - 1 || y < 0 || y > mRows - 1)
    {
        std::cerr << "tryingto select outside of spirtesheet range" << std::endl;
    }

    mCurX = x;
    mCurY = y;
}

void SpriteSheet::SelectSprite(int i)
{
    mCurX = i % mCols;
    mCurY = i / mCols;
}

void SpriteSheet::DrawSprite(SDL_FRect* dst)
{
    if (mRender == nullptr)
    {
        std::cerr << "failed to draw sprite: no renderer attached" << std::endl;
        return;
    }

    SDL_Rect src = { mCurX * (mTexWidth / mCols), mCurY * (mTexHeight / mRows), (mTexWidth / mCols), (mTexHeight / mRows) };
    if (SDL_RenderCopyF(mRender, mSpritesheetTex, &src, dst) < 0)
    {
        std::cerr << "failed to draw sprite: error " << SDL_GetError() << std::endl;
    }
}

void SpriteSheet::AttachRenderer(SDL_Renderer* render)
{
    mRender = render;

    if (mLastPathLoaded == "")
    {
        std::cout << "no texture ws lodaed previusly before switching renderers. make sure you load texture before use" << std::endl;
        return;
    }

    LoadSpritesheet(mLastPathLoaded.c_str(), mCols, mRows);
}

bool SpriteSheet::LoadSpritesheet(const char* path, unsigned int cols, unsigned int rows)
{
    if (cols == 0 || rows == 0)
    {
        std::cerr << "failed to load spritesheet: cols or rows equaal to 0" << std::endl;
        return false;
    }

    if (mRender == nothing)
    {
        std::cerr << "failed to load spritesheet: no renderer to load spritesheet on" << std::endl;
        return false;
    }

    SDL_Renderer* temp = mRender;

    Free();

    mRender = temp;

    mCols = cols;
    mRows = rows;

    mSpritesheetTex = IMG_LoadTexture(mRender, path);
    if (!mSpritesheetTex)
    {
        std::cerr << "error loading tex at path '" << path << "'. error: " << IMG_GetError() << std::endl;
        return false;
    }

    if (SDL_QueryTexture(mSpritesheetTex, nullptr, nullptr, &mTexWidth, &mTexHeight) < 0)
    {
        std::cout << "faialed to query spritesheet tex. error: " << SDL_GetError() << std::endl;
        return false;
    }

    mLastPathLoaded = path;

    mInit = true;
    return true;
}

bool SpriteSheet::LoadSpritesheet(SDL_Renderer* render, const char* path, unsigned int cols, unsigned int rows)
{
    if (cols == 0 || rows == 0)
    {
        std::cerr << "cols or rows equaal to 0. spritesheet err" << std::endl;
        return false;
    }
    
    Free();

    mRender = render;

    mCols = cols;
    mRows = rows;

    mSpritesheetTex = IMG_LoadTexture(mRender, path);
    if (!mSpritesheetTex)
    {
        std::cerr << "error loading tex at path '" << path << "'. error: " << IMG_GetError() << std::endl;
        return false;
    }

    if (SDL_QueryTexture(mSpritesheetTex, nullptr, nullptr, &mTexWidth, &mTexHeight) < 0)
    {
        std::cout << "faialed to query spritesheet tex. error: " << SDL_GetError() << std::endl;
        return false;
    }

    mLastPathLoaded = path;

    mInit = true;
    return true;
}

void SpriteSheet::Free()
{
    mInit = false;
    mCols = 0;
    mRows = 0;
    mTexWidth = 0;
    mTexHeight = 0;
    mCurX = 0;
    mCurY = 0;
    mRender = nothing;
    SDL_DestroyTexture(mSpritesheetTex);
}

int SpriteSheet::GetNumSprites() const
{
    return mCols * mRows;
}

int SpriteSheet::GetTexWidth() const
{
    return mTexWidth;
}

int SpriteSheet::GetTexHeight() const
{
    return mTexHeight;
}

int SpriteSheet::GetRows() const
{
    return mRows;
}

int SpriteSheet::GetCols() const
{
    return mCols;
}

SDL_Texture* SpriteSheet::GetTexture() const
{
    return mSpritesheetTex;
}

std::string SpriteSheet::GetLastPathLoaded() const
{
    return mLastPathLoaded;
}

SDL_Renderer* SpriteSheet::GetRendererAttached() const
{
    return mRender;
}

bool SpriteSheet::Ready() const
{
    return mInit;
}
