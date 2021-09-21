#include"Win32App.hpp"

Win32App::Win32App() 
    : 
    m_bIsInit(false), m_bIsQuit(false),
    m_pAppInstance(),
    m_nWndWidth(0), m_nWndHeight(0),
    m_hWnd(nullptr), m_mouse(),
    m_pGraphicsManager(nullptr)
{};

void Win32App::InitApp(
    const HINSTANCE appInstance, int nCmdShow,
    const int16_t width, const int16_t height
){
    m_nWndWidth = width;
    m_nWndHeight = height;
    m_pAppInstance = appInstance;

    const std::string CLASS_NAME  = "Sample Window Class";
    
    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = m_pAppInstance;
    wc.lpszClassName = CLASS_NAME.c_str();

    RegisterClass(&wc);

    m_hWnd = CreateWindowEx(
        0, CLASS_NAME.c_str(),             
        "Win32App Sample", WS_OVERLAPPEDWINDOW,            
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, appInstance, NULL
    );

    if (m_hWnd == nullptr){
        return;
    }

    ShowWindow(m_hWnd, nCmdShow);

    m_pGraphicsManager = AcornEngine::GraphicsManager::GetInstance();



    m_bIsInit = true;
}

void Win32App::RunApp(){
    assert(m_bIsInit == true);

    while(m_bIsQuit){
        MSG msg = {};
        while(GetMessageA(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

LRESULT Win32App::MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch (uMsg){

        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:{
            break;
        }
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP:{

            break;
        }
        case WM_SIZE:{
            int width = LOWORD(lParam);  // Macro to get the low-order word.
            int height = HIWORD(lParam); // Macro to get the high-order word.       

            break;
        }
        case WM_PAINT:{
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

            EndPaint(hwnd, &ps);
            break;
        }
        case WM_CLOSE:{
            if(MessageBox(hwnd, "Really quit?", "My application", MB_OKCANCEL) == IDOK){
                DestroyWindow(hwnd);
            }
            break;
        }
        case WM_DESTROY:{
            PostQuitMessage(0);
            return 0;
        }

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK Win32App::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    return s_pInstance->MsgProc(hwnd, uMsg, wParam, lParam);
}