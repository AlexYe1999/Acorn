#pragma once
#include"Timer.hpp"
#include"BasicMouse.hpp"
#include"D3D12GraphicsManager.hpp"
#include<string>
#include<cstdint>
#include<cassert>
#include<windows.h>

class Win32App{

public:
    static Win32App* GetInstance(){
        return s_pInstance != nullptr ? s_pInstance 
            : s_pInstance = new Win32App;
    }

    virtual void InitApp(const HINSTANCE appInstance, int nCmdShow,const int16_t width, const int16_t height);
    virtual void RunApp();

protected:
    bool m_bIsInit;
    bool m_bIsQuit;

    HINSTANCE m_pAppInstance;
    int16_t m_nWndWidth;
    int16_t m_nWndHeight;
    HWND m_hWnd;

    AcornEngine::Timer m_Timer;
    AcornEngine::BasicMouse m_Mouse;

    AcornEngine::D3D12GraphicsManager* m_pGraphicsManager;

protected:
    virtual LRESULT MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
    Win32App();
    Win32App(const Win32App &app) = delete;
    Win32App& operator =(const Win32App &app) = delete;
    static inline Win32App* s_pInstance = nullptr;
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};