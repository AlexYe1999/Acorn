#include"Win32App.hpp"

Win32App::Win32App() 
    : 
    m_bIsInit(false), m_bIsQuit(false),
    m_pAppInstance(),
    m_nWndWidth(0), m_nWndHeight(0),
    m_hWnd(nullptr), m_Mouse(),
    m_pGraphicsManager(nullptr)
{};

AcornEngine::GraphicsParam g_GraphicsConfig;

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

    m_hWnd = CreateWindow(
        CLASS_NAME.c_str(), "Win32App Sample", WS_OVERLAPPEDWINDOW,            
        CW_USEDEFAULT, CW_USEDEFAULT, m_nWndWidth, m_nWndHeight,
        0, 0, appInstance, 0
    );

    if (m_hWnd == nullptr){
        return;
    }

    ShowWindow(m_hWnd, nCmdShow);

    m_pGraphicsManager = AcornEngine::D3D12GraphicsManager::GetInstance();

    g_GraphicsConfig.MainWnd   = m_hWnd;
    g_GraphicsConfig.WndWidth  = m_nWndWidth;
    g_GraphicsConfig.WndHeight = m_nWndHeight;
    g_GraphicsConfig.ViewPort.Width  = m_nWndWidth;
    g_GraphicsConfig.ViewPort.Height = m_nWndHeight;
    g_GraphicsConfig.ViewPort.TopLeftX = 0;
    g_GraphicsConfig.ViewPort.TopLeftY = 0;
    g_GraphicsConfig.ViewPort.MinDepth = 0.0f;
    g_GraphicsConfig.ViewPort.MaxDepth = 1.0f;
    g_GraphicsConfig.ScissorRect.left  = 0;
    g_GraphicsConfig.ScissorRect.top   = 0;
    g_GraphicsConfig.ScissorRect.right = m_nWndWidth;
    g_GraphicsConfig.ScissorRect.bottom = m_nWndHeight;


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
        
        UpdateInput();
        m_pGraphicsManager->Render();
        
        m_Timer.Tick();
        totalTime += m_Timer.DeltaTime();
        frameCount++;

        std::string text = "Win32App Sample";
        text += "  " + std::to_string(static_cast<int>(frameCount / totalTime)) + " fps ";

        SetWindowText(m_hWnd, text.c_str());

        if(frameCount > 500){
            frameCount = 0;
            totalTime = 0.0f;
        }
    }
}

inline LRESULT Win32App::MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch (uMsg){
        case WM_LBUTTONDOWN:{
            SetCapture(hwnd);
            AcornEngine::Point2<int16_t> p;
            p.x = GET_X_LPARAM(lParam);
            p.y = GET_Y_LPARAM(lParam);
            m_Mouse.SetLastPosition(p);
            m_Mouse.KeyDown(0);
            break;
        }
        case WM_MBUTTONDOWN:{
            m_Mouse.KeyDown(1);
            break;
        }
        case WM_RBUTTONDOWN:{
            m_Mouse.KeyDown(2);
            break;
        }
        case WM_LBUTTONUP:{
            ReleaseCapture();
            m_Mouse.KeyUp(0);
            break;
        }
        case WM_MBUTTONUP:{
            m_Mouse.KeyUp(1);
            break;
        }
        case WM_RBUTTONUP:{
            m_Mouse.KeyUp(2);
            break;
        }
        case WM_MOUSEMOVE:{
            AcornEngine::Point2<int16_t> p;
            p.x = GET_X_LPARAM(lParam);
            p.y = GET_Y_LPARAM(lParam);
            m_Mouse.SetCurrPosition(p);
            break;
        }
        case WM_SIZE:{
            g_GraphicsConfig.WndWidth = LOWORD(lParam);
            g_GraphicsConfig.WndHeight = HIWORD(lParam);

            if(m_bIsInit) m_pGraphicsManager->ResetRtAndDs();
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

inline void Win32App::UpdateInput(){
    static int x = 0;
    static int y = 0;
    static int x_t = 0;
    static int y_t = 0;

    if(m_Mouse.IsKeyDown(0)){
        AcornEngine::Point2<int16_t> p = m_Mouse.GetDeltaPosition();
        x_t = p.x;
        y_t = p.y;
    }
    else{
        x += x_t;
        y += y_t;
        x_t = 0;
        y_t = 0; 
    }

    float theta = 0.25f * DirectX::XMConvertToRadians(x+x_t);
    float phi = 0.25f * DirectX::XMConvertToRadians(y+y_t);
    m_pGraphicsManager->UpdataConstants(theta, phi);
}

LRESULT CALLBACK Win32App::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    return s_pInstance->MsgProc(hwnd, uMsg, wParam, lParam);
}

