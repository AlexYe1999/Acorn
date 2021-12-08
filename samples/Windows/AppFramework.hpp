#pragma once
#include"IApplication.hpp"
#include<windows.h>

class __declspec(dllexport) AppFramework{
public:
    static int Run(App* pApp, HINSTANCE hInstance, int nCmdShow);
    static HWND GetHwnd() { return m_hWnd; }
    static App* GetApp()  { return m_pApp; }

protected:
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    static inline HWND m_hWnd = nullptr;
    static inline App* m_pApp = nullptr;
};