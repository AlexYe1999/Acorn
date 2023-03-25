#include "win32_window.hpp"
#include "runtime/engine.hpp"

namespace Acorn{

    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        Win32Window* window = nullptr;

        if(uMsg == WM_CREATE)
        {
            window = reinterpret_cast<Win32Window*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        }
        else
        {
            window = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }

        switch(uMsg)
        {
            case WM_CLOSE:
            {
                DestroyWindow(hwnd);
                return 0;
            }
            case WM_DESTROY:
            {
                PostQuitMessage(0);
                window->CloseWindow();
                return 0;
            }

        }

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    void Win32Window::InitSystem()
    {
        const auto& config = EngineRuntimeContext::ConfigSystem()->GetEngineConfig();

        m_width  = config.width;
        m_height = config.height;
        m_is_fullscreen = config.is_fullscreen;

        WNDCLASS wc = {};

        wc.lpfnWndProc   = WindowProc;
        wc.hInstance     = GetModuleHandleA(NULL);
        wc.lpszClassName = "Win32Window Class";

        RegisterClass(&wc);

        m_hwnd = ::CreateWindowEx(
            0,                            
            "Win32Window Class",      
            "Learn to Program Game Engine",
            WS_OVERLAPPEDWINDOW,          
            CW_USEDEFAULT, CW_USEDEFAULT, 
            m_width, m_height,
            NULL,
            NULL,      
            GetModuleHandleA(NULL),  
            this 
        );

        ShowWindow(m_hwnd, m_is_fullscreen == false ? SW_SHOW : SW_MAXIMIZE);
    }

    void Win32Window::ShutdownSystem()
    {
        ::UnregisterClassW(L"Win32Window Class", GetModuleHandleA(NULL));
    }

    void Win32Window::ProcessMessage()
    {
        MSG msg = {};
        while(PeekMessageA(&msg, m_hwnd, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

    }

    void Win32Window::SetTitle(char const* const text) const
    {
        SetWindowTextA(m_hwnd, text);
    }

}