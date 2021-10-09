#pragma once
#include<string>
#include<cassert>
#include<stdexcept>
#include<cstdint>
#include<windows.h>
#include<windowsx.h>

template<typename DerivedClass>
class AppFremework{
public:
    static DerivedClass& GetInstance();
    virtual void InitApp(
        const HINSTANCE appInstance, int nCmdShow, 
        const int16_t width, const int16_t height,
        const std::string wndClassName, const std::string wndText
    );

    virtual void RunApp();

protected:
    bool m_bIsInit;
    bool m_bIsQuit;

    HINSTANCE m_pAppInstance;
    int16_t m_nWndWidth = 0;
    int16_t m_nWndHeight = 0;
    HWND m_hWnd;

    virtual LRESULT MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

protected:
    AppFremework();
    AppFremework(DerivedClass&& app) = delete;
    AppFremework(const DerivedClass& app) = delete;
    AppFremework& operator =(const DerivedClass& app) = delete;
    virtual ~AppFremework() = default;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
        return GetInstance().MsgProc(hwnd, uMsg, wParam, lParam);
    }
};

template<typename DerivedClass>
AppFremework<DerivedClass>::AppFremework()
    :
    m_bIsInit(false), m_bIsQuit(false),
    m_pAppInstance(), m_hWnd(),
    m_nWndWidth(0), m_nWndHeight(0)
{}

template<typename DerivedClass>
inline DerivedClass& AppFremework<DerivedClass>::GetInstance(){
    static DerivedClass s_Instance;
    return s_Instance;
}

template<typename DerivedClass>
void AppFremework<DerivedClass>::InitApp(
        const HINSTANCE appInstance, int nCmdShow, 
        const int16_t width, const int16_t height,
        const std::string wndClassName,
        const std::string wndText
){
    assert(width > 0 && height > 0);
    m_nWndWidth = width;
    m_nWndHeight = height;
    m_pAppInstance = appInstance;
    
    WNDCLASS wc = {};
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = appInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = wndClassName.c_str();

    RegisterClass(&wc);

    m_hWnd = CreateWindow(
        wndClassName.c_str(), wndText.c_str(), WS_OVERLAPPEDWINDOW,            
        CW_USEDEFAULT, CW_USEDEFAULT, m_nWndWidth, m_nWndHeight,
        0, 0, appInstance, 0
    );

    if (m_hWnd == nullptr) 
        throw std::runtime_error("create window failed");

    ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);

}

template<typename DerivedClass>
void AppFremework<DerivedClass>::RunApp(){

    while(!m_bIsQuit){
        MSG msg = {};

        if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }   
}
