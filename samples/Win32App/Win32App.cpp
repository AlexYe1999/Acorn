#include"Win32App.hpp"

Win32App::Win32App() 
    : 
    m_bIsInit(false), m_bIsQuit(false),
    m_pAppInstance(),
    m_nWndWidth(0), m_nWndHeight(0),
    m_hWnd(nullptr), m_Mouse(),
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
        CW_USEDEFAULT, CW_USEDEFAULT, m_nWndWidth, m_nWndHeight,
        NULL, NULL, appInstance, NULL
    );

    if (m_hWnd == nullptr){
        return;
    }

    ShowWindow(m_hWnd, nCmdShow);

    m_pGraphicsManager = AcornEngine::D3D12GraphicsManager::GetInstance();

    AcornEngine::GraphicsParam param;
    param.MainWnd   = m_hWnd;
    param.WndWidth  = m_nWndWidth;
    param.WndHeight = m_nWndHeight;
    param.ViewPort.Width = m_nWndWidth;
    param.ViewPort.Height = m_nWndHeight;
    param.ViewPort.TopLeftX = 0;
    param.ViewPort.TopLeftY = 0;
    param.ViewPort.MinDepth = 0.0f;
    param.ViewPort.MaxDepth = 1.0f;
    param.ScissorRect.left = 0;
    param.ScissorRect.top  = 0;
    param.ScissorRect.right = m_nWndWidth;
    param.ScissorRect.bottom = m_nWndHeight;

    m_pGraphicsManager->GetSettingParams(param);
    m_pGraphicsManager->Initialize();


    m_bIsInit = true;
}

void Win32App::RunApp(){

    assert(m_bIsInit == true);
    uint64_t frameCount = 0;
    float totalTime = 0.0f;
    m_Timer.Reset();

    while(!m_bIsQuit){
        MSG msg = {};

        if(PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        m_Timer.Tick();
        m_pGraphicsManager->Render();


        totalTime += m_Timer.DeltaTime();        
        frameCount++;
        
        std::string text = "Win32App Sample";
        text += "  " + std::to_string(frameCount / totalTime) + " fps ";
        SetWindowText(m_hWnd, text.c_str());

        if(frameCount > 500){
            frameCount = 0;
            totalTime = 0.0f;
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
                m_bIsQuit = true;
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