#ifndef WINDOW_H_DEFINED
#define WINDOW_H_DEFINED

#include "Helpers/d3dUtil.h"
class Renderer;

class Window
{
public:
    Window(int width, int height, const wchar_t* title);
    ~Window();

    bool ProcessMessages();

    Window* GetInstance();
    HWND GetHandle() const;

private:
    static LRESULT CALLBACK StaticWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static Window* s_pInstance;

    HWND m_hWindow = nullptr;

    bool m_minimized = false;   // is the application minimized?
    bool m_maximized = false;   // is the application maximized?
    bool m_resizing = false;    // are the resize bars being dragged?
    bool m_fullscreen = false;  // fullscreen enabled
};

#endif // !ENGINE_WINDOW_H