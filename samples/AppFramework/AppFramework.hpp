#pragma once
#include"App.hpp"
#include<windows.h>

class AppFramework{
public:
    static int Run(App* pApp, HINSTANCE hInstance, int nCmdShow);
    static HWND GetHwnd() { return m_hWnd; }

protected:
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    static inline HWND m_hWnd = nullptr;

};