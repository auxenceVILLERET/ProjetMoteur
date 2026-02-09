#ifndef WINDOW_CPP_INCLUDED
#define WINDOW_CPP_INCLUDED

#include "Window.h"

Window* Window::s_pInstance = nullptr;

Window::Window(int width, int height, const wchar_t* title)
{
    if (s_pInstance != nullptr)
        throw std::runtime_error("Window: une instance existe déjà.");

    s_pInstance = this;

    m_width = width;
    m_height = height;

    HINSTANCE hInst = GetModuleHandle(nullptr);

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = StaticWindowProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"Window";
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    RegisterClassEx(&wc);

    RECT r{ 0, 0, m_width, m_height };
    AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);

    m_hWindow = CreateWindowEx(
        0,
        wc.lpszClassName,
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        r.right - r.left,
        r.bottom - r.top,
        nullptr, nullptr,
        hInst,
        nullptr
    );

    ShowWindow(m_hWindow, SW_SHOW);
}

Window::~Window()
{
    if (m_hWindow)
        DestroyWindow(m_hWindow);

    s_pInstance = nullptr;
}

bool Window::ProcessMessages()
{
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            return false;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return true;
}

Window* Window::GetInstance()
{
    return s_pInstance;
}

HWND Window::GetHandle() const
{
    return m_hWindow;
}

LRESULT Window::StaticWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (s_pInstance == nullptr)
        return DefWindowProc(hwnd, msg, wParam, lParam);

    return s_pInstance->WindowProc(hwnd, msg, wParam, lParam);
}

LRESULT Window::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
	case WM_SIZE:
        if (wParam != SIZE_MINIMIZED)
        {
            m_width = LOWORD(lParam);
            m_height = HIWORD(lParam);
            m_resizing = true;
        }
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}


#endif // !WINDOW_CPP_INCLUDED