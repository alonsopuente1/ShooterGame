#pragma once

#include <SDL.h>
#include <iostream>
#include <SDL_syswm.h>
#include <SDL_image.h>
#include <vector>

class Window
{

public:
    Window();
    ~Window();

    bool Create(const char* title, int width, int height);

    void HandleEvent(SDL_Event e);

    void Focus();

    void Free();

    void ShowWindow();
    void HideWindow();

    // loads texture on window's renderer
    SDL_Texture* LoadTexture(const char* filepath);

    // setters
    void SetWindowTitle(const char* newTitle);


    // getters
    bool GetShown() const;
    bool GetMouseFocus() const;
    SDL_Renderer* GetRenderer() const;

    int GetWidth() const;
    int GetHeight() const;

    HWND GetWindowHandle() const;
    SDL_Window* GetWindow() const;
private:

    std::vector<SDL_Texture*> mTexturesLoaded;

    SDL_Window* mWindow;
    SDL_Renderer* mRender;
    int mWindowID;
    HWND mWindowHandle;

    int mWidth;
    int mHeight;

    bool mMouseFocus;
    bool mKeyboardFocus;
    bool mFullscreen;
    bool mMinimised;
    bool mShown;

    bool mInit;
};