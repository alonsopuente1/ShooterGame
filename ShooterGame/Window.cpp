#include "Window.h"

Window::Window() :  mWindow(nullptr), 
                    mRender(nullptr), 
                    mInit(false),
                    mMouseFocus(false),
                    mKeyboardFocus(false),
                    mFullscreen(false),
                    mMinimised(false),
                    mShown(false),
                    mWidth(false),
                    mHeight(false),
                    mWindowID(false),
                    mWindowHandle(nullptr)
{
}

Window::~Window()
{
    Free();
}

bool Window::Create(const char* title, int width, int height)
{
    if (width < 0 || height < 0 || width > 1900 || height > 1060)
    {
        std::cerr << "Width or height out of ranging when creating window" << std::endl;
        return false;
    }

    mWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
    if (!mWindow)
    {
        std::cerr << "Failed to create window. error: " << SDL_GetError() << std::endl;
        return false;
    }

    mRender = SDL_CreateRenderer(mWindow, -1, 0);
    if (!mRender)
    {
        std::cerr << "Failed to create renderer . error: " << SDL_GetError() << std::endl;
        return false;
    }

    mMouseFocus = true;
    mKeyboardFocus = true;
    mWidth = width;
    mHeight = height;
    mShown = true;

    mWindowID = SDL_GetWindowID(mWindow);
    if (mWindowID == 0)
    {
        std::cerr << "Failed to get window ID of window. error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    if (!SDL_GetWindowWMInfo(mWindow, &info))
    {
        std::cerr << "failed to get window handle to sdl window. error: " << SDL_GetError() << std::endl;
        return false;
    }

    mWindowHandle = info.info.win.window;

    return true;
}

void Window::HandleEvent(SDL_Event e)
{
    if (e.type == SDL_WINDOWEVENT && e.window.windowID == mWindowID)
    {
        switch (e.window.event)
        {
            //Window appeared
        case SDL_WINDOWEVENT_SHOWN:
            mShown = true;
            break;

            //Window disappeared
        case SDL_WINDOWEVENT_HIDDEN:
            mShown = false;
            break;

            //Get new dimensions and repaint
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            mWidth = e.window.data1;
            mHeight = e.window.data2;
            SDL_RenderPresent(mRender);
            break;

            //Repaint on expose
        case SDL_WINDOWEVENT_EXPOSED:
            SDL_RenderPresent(mRender);
            break;

            //Mouse enter
        case SDL_WINDOWEVENT_ENTER:
            mMouseFocus = true;
            break;

            //Mouse exit
        case SDL_WINDOWEVENT_LEAVE:
            mMouseFocus = false;
            break;

            //Keyboard focus gained
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            mKeyboardFocus = true;
            break;

            //Keyboard focus lost
        case SDL_WINDOWEVENT_FOCUS_LOST:
            mKeyboardFocus = false;
            break;

            //Window minimized
        case SDL_WINDOWEVENT_MINIMIZED:
            mMinimised = true;
            break;

            //Window maxized
        case SDL_WINDOWEVENT_MAXIMIZED:
            mMinimised = false;
            break;

            //Window restored
        case SDL_WINDOWEVENT_RESTORED:
            mMinimised = false;
            break;
            
            //Hide on close
        case SDL_WINDOWEVENT_CLOSE:
            SDL_HideWindow(mWindow);
            mShown = false;
            break;
        }
    }
}

void Window::Focus()
{
    if (!mShown)
    {
        ShowWindow();
    }

    SDL_RaiseWindow(mWindow);
}

void Window::Free()
{
    for (SDL_Texture*& tex : mTexturesLoaded)
    {
        SDL_DestroyTexture(tex);
    }

    SDL_DestroyRenderer(mRender);
    SDL_DestroyWindow(mWindow);

    mWidth = 0;
    mHeight = 0;

    mMouseFocus = false;
    mKeyboardFocus = false;
    mFullscreen = false;
    mMinimised = false;
    mShown = false;

    mInit = false;
}

void Window::ShowWindow()
{
    SDL_ShowWindow(mWindow);
    mShown = true;
}

void Window::HideWindow()
{
    SDL_HideWindow(mWindow);
    mShown = false;
}

SDL_Texture* Window::LoadTexture(const char* filepath)
{
    if (mRender == nullptr)
    {
        std::cerr << "failed to load texture: window not created" << std::endl;
        return nullptr;
    }

    SDL_Texture* out = IMG_LoadTexture(mRender, filepath);
    if (!out)
    {
        std::cerr << "failed to load texture at '" << filepath << "'. error: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    mTexturesLoaded.emplace_back(out);

    return out;
}

void Window::SetWindowTitle(const char* newTitle)
{
    SDL_SetWindowTitle(mWindow, newTitle);
}

bool Window::GetShown() const
{
    return mShown;
}

bool Window::GetMouseFocus() const
{
    return mMouseFocus;
}

SDL_Renderer* Window::GetRenderer() const
{
    return mRender;
}

int Window::GetWidth() const
{
    return mWidth;
}

int Window::GetHeight() const
{
    return mHeight;
}

HWND Window::GetWindowHandle() const
{
    return mWindowHandle;
}

SDL_Window* Window::GetWindow() const
{
    return mWindow;
}
