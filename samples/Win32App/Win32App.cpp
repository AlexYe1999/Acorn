#include"Win32App.hpp"

Win32App::Win32App() 
    : 
    m_bIsInit(false), m_bIsQuit(false),
    m_pAppInstance(),
    m_nWndWidth(0), m_nWndHeight(0),
    m_hWnd(nullptr), m_Mouse(),
    m_pGraphicsManager(nullptr),
    m_pScene(nullptr)
{};

Acorn::GraphicsParam g_GraphicsConfig;

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
	UpdateWindow(m_hWnd);

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

    m_pGraphicsManager = Acorn::D3D12GraphicsManager::GetInstance();

    BuildScene();
    m_pGraphicsManager->Initialize(m_pScene.get());

    m_bIsInit = true;
}

void Win32App::RunApp(){

    assert(m_bIsInit == true);
    uint64_t frameCount = 0;
    float totalTime = 0.0f;
    m_Timer.Reset();

    while(!m_bIsQuit){
        MSG msg = {};

        if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        UpdateInput();

        m_Timer.Tick();
        m_pGraphicsManager->Tick();
        
        totalTime += m_Timer.DeltaTime();
        frameCount++;

        std::string text = "Win32App Sample";
        text += "  " + std::to_string(static_cast<int>(frameCount / totalTime)) + " fps ";

        SetWindowText(m_hWnd, text.c_str());

        if(frameCount > 1000){
            frameCount = 0;
            totalTime = 0.0f;
        }
    }
}

 inline LRESULT Win32App::MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch (uMsg){
        case WM_LBUTTONDOWN:{
            SetCapture(hwnd);
            Acorn::Point2<int16_t> p;
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
            Acorn::Point2<int16_t> p;
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
                return 0;
            }
        }
        case WM_DESTROY:{
            PostQuitMessage(0);
            return 0;
        }

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Win32App::BuildScene(){
    m_pScene->MainCamera.LookAt(
        Acorn::Vector3f(0.0f, 10.0f, 20.0f),
        Acorn::Vector3f(0.0f, 0.0f, 0.0f),
        Acorn::Vector3f(0.0f, 1.0f, 0.0f)
    );
    CreateMesh();
    CreateRenderItem();
}

void Win32App::CreateMesh(){
    using GeoGenerator = LemonCube::GeometryGenerator;

    m_pScene = std::make_unique<Acorn::Scene>();
    m_pScene->MainCamera.LookAt(
        Acorn::Vector3f(0.0f, 1.0f, 3.0f),
        Acorn::Vector3f(0.0f, 0.0f, 3.0f),
        Acorn::Vector3f(0.0f, 1.0f, 0.0f)
    );

    std::unique_ptr<Acorn::Mesh> mesh = std::make_unique<Acorn::Mesh>();
    mesh->Name = "shapeGeo";

    GeoGenerator geoGen;
    GeoGenerator::MeshData box = geoGen.CreateBox(1.5f, 0.5f, 1.5f, 3);
    GeoGenerator::MeshData grid = geoGen.CreateGrid(20.0f, 30.0f, 60, 40);
    GeoGenerator::MeshData sphere = geoGen.CreateSphere(0.5f, 20, 20);
    GeoGenerator::MeshData cylinder = geoGen.CreateCylinder(0.5f, 0.3f, 3.0f, 20, 20);

    uint32_t totalVertexCount = box.vertices.size()
        + box.vertices.size() + box.vertices.size() + cylinder.vertices.size();
    uint32_t totalIndexCount = box.indices.size()
        + box.indices.size() + box.indices.size() + cylinder.indices.size();

    std::vector<Acorn::VertexP3C4> vertices(totalVertexCount);
    std::vector<uint16_t> indices;

    uint32_t VertexIndex  = 0;
    uint32_t indexOffset  = 0;
    uint32_t vertexOffset = 0;

    Acorn::SubMesh boxSubmesh;
    boxSubmesh.IndexCount = box.indices.size();
    boxSubmesh.StartIndexLocation = indexOffset;
    boxSubmesh.StartIndexLocation = vertexOffset;
    indexOffset  += box.indices.size();
    vertexOffset += box.vertices.size();
    mesh->SubMesh["box"] = std::move(boxSubmesh);
    for(const auto& vertex : box.vertices){
        vertices[VertexIndex].Position = vertex.position;
        vertices[VertexIndex++].Color = Acorn::Vector4f(1.0f);
    }
    indices.insert(indices.end(), box.indices.begin(), box.indices.end());

#if defined (_DEBUG)
    std::cout << "box index  count : " << box.indices.size() << std::endl;
    std::cout << "box vertex count : " << box.vertices.size() << std::endl;
    std::cout << "total index  count : " << vertices.size() << std::endl;
    std::cout << "total vertex count : " << indices.size() << std::endl;
    std::cout << std::endl;
#endif

    Acorn::SubMesh gridSubmesh;
    gridSubmesh.IndexCount = grid.indices.size();
    gridSubmesh.StartIndexLocation = indexOffset;
    gridSubmesh.StartIndexLocation = vertexOffset;
    indexOffset  += grid.indices.size();
    vertexOffset += grid.vertices.size();
    mesh->SubMesh["grid"] = std::move(gridSubmesh);
    for(const auto& vertex : grid.vertices){
        vertices[VertexIndex].Position = vertex.position;
        vertices[VertexIndex++].Color = Acorn::Vector4f(1.0f, 1.0f, 0.0f, 1.0f);
    }
    indices.insert(indices.end(), grid.indices.begin(), grid.indices.end());

#if defined (_DEBUG)
    std::cout << "grid index  count : " << grid.indices.size() << std::endl;
    std::cout << "grid vertex count : " << grid.vertices.size() << std::endl;
    std::cout << "total index  count : " << vertices.size() << std::endl;
    std::cout << "total vertex count : " << indices.size() << std::endl;
    std::cout << std::endl;
#endif

    Acorn::SubMesh sphereSubmesh;
    sphereSubmesh.IndexCount = sphere.indices.size();
    sphereSubmesh.StartIndexLocation = indexOffset;
    sphereSubmesh.StartIndexLocation = vertexOffset;
    indexOffset  += sphere.indices.size();
    vertexOffset += sphere.vertices.size();
    mesh->SubMesh["sphere"] = std::move(sphereSubmesh);
    for(const auto& vertex : sphere.vertices){
        vertices[VertexIndex].Position = vertex.position;
        vertices[VertexIndex++].Color = Acorn::Vector4f(1.0f, 0.0f, 1.0f, 1.0f);
    }
    indices.insert(indices.end(), sphere.indices.begin(), sphere.indices.end());

#if defined (_DEBUG)
    std::cout << "sphere index  count : " << sphere.indices.size() << std::endl;
    std::cout << "sphere vertex count : " << sphere.vertices.size() << std::endl;
    std::cout << "total index  count : " << vertices.size() << std::endl;
    std::cout << "total vertex count : " << indices.size() << std::endl;
    std::cout << std::endl;
#endif

    Acorn::SubMesh cylinderSubmesh;
    cylinderSubmesh.IndexCount = cylinder.indices.size();
    cylinderSubmesh.StartIndexLocation = indexOffset;
    cylinderSubmesh.StartIndexLocation = vertexOffset;
    indexOffset  += cylinder.indices.size();
    vertexOffset += cylinder.vertices.size();
    mesh->SubMesh["cylinder"] = std::move(cylinderSubmesh);
    for(const auto& vertex : cylinder.vertices){
        vertices[VertexIndex].Position = vertex.position;
        vertices[VertexIndex++].Color = Acorn::Vector4f(0.0f, 1.0f, 1.0f, 1.0f);
    }
    indices.insert(indices.end(), cylinder.indices.begin(), cylinder.indices.end());

#if defined (_DEBUG)
    std::cout << "cylinder index  count : " << cylinder.indices.size() << std::endl;
    std::cout << "cylinder vertex count : " << cylinder.vertices.size() << std::endl;
    std::cout << "total index  count : " << vertices.size() << std::endl;
    std::cout << "total vertex count : " << indices.size() << std::endl;
    std::cout << std::endl;
#endif

    const uint32_t vertexByteSize = sizeof(Acorn::VertexP3C4) * vertices.size();
    const uint32_t indexByteSize  = sizeof(uint16_t) * indices.size();

    D3DCreateBlob(vertexByteSize, mesh->VertexBufferCPU.GetAddressOf());
    D3DCreateBlob(indexByteSize, mesh->IndexBufferCPU.GetAddressOf());

    CopyMemory(mesh->VertexBufferCPU->GetBufferPointer(), vertices.data(), vertexByteSize);
    CopyMemory(mesh->IndexBufferCPU->GetBufferPointer(), indices.data(), indexByteSize);

    mesh->VertexByteStride = sizeof(Acorn::VertexP3C4);
    mesh->VertexBufferByteSize = vertexByteSize;
    mesh->IndexFormat = DXGI_FORMAT_R16_UINT;
    mesh->IndexBufferByteSize  = indexByteSize;

    m_pScene->Meshes[mesh->Name] = std::move(mesh);
}

void Win32App::CreateRenderItem(){
    uint16_t objIndex = 0;

    auto box = std::make_unique<Acorn::RenderItem>();
    box->WorldMatrix = DirectX::XMMatrixScaling(2.0f, 2.0f, 2.0f)
        * DirectX::XMMatrixTranslation(2.0f, 2.0f, 2.0f);
    box->Mesh = m_pScene->Meshes["shapeGeo"].get();
    box->ObjCBIndex = objIndex++;
    box->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    box->IndexCount = box->Mesh->SubMesh["box"].IndexCount;
    box->StartVertexLocation = box->Mesh->SubMesh["box"].StartVertexLocation;
    box->StartIndexLocation = box->Mesh->SubMesh["box"].StartIndexLocation;
    box->DirtyCount = g_GraphicsConfig.FrameResorceCount;
    m_pScene->OpaqueRenderItems.push_back(std::move(box.get()));
    m_pScene->AllRenderItems.push_back(std::move(box));

    auto grid = std::make_unique<Acorn::RenderItem>();
    grid->Mesh = m_pScene->Meshes["shapGeo"].get();
    grid->ObjCBIndex = objIndex++;
    grid->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    grid->IndexCount = grid->Mesh->SubMesh["grid"].IndexCount;
    grid->StartVertexLocation = grid->Mesh->SubMesh["grid"].StartVertexLocation;
    grid->StartIndexLocation = grid->Mesh->SubMesh["grid"].StartIndexLocation;
    grid->DirtyCount = g_GraphicsConfig.FrameResorceCount;
    m_pScene->OpaqueRenderItems.push_back(std::move(grid.get()));
    m_pScene->AllRenderItems.push_back(std::move(grid));

    for(int i = 0; i < 5; i++){

        auto leftCylRitem = std::make_unique<Acorn::RenderItem>();
        DirectX::XMMATRIX leftCylWorld =
            DirectX::XMMatrixTranslation(-5.0f, 1.5f, -10.0f + i * 5.0f);

        leftCylRitem->WorldMatrix = leftCylWorld;
        leftCylRitem->Mesh = m_pScene->Meshes["shapeGeo"].get();
        leftCylRitem->ObjCBIndex = objIndex++;
        leftCylRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        leftCylRitem->IndexCount = 
            leftCylRitem->Mesh->SubMesh["cylinder"].IndexCount;
        leftCylRitem->StartVertexLocation =
            leftCylRitem->Mesh->SubMesh["cylinder"].StartVertexLocation;
        leftCylRitem->StartIndexLocation =
            leftCylRitem->Mesh->SubMesh["cylinder"].StartIndexLocation;
        leftCylRitem->DirtyCount = g_GraphicsConfig.FrameResorceCount;
        m_pScene->OpaqueRenderItems.push_back(std::move(leftCylRitem.get()));
        m_pScene->AllRenderItems.push_back(std::move(leftCylRitem));

        auto rightCylRitem = std::make_unique<Acorn::RenderItem>();
        DirectX::XMMATRIX RightCylWorld =
            DirectX::XMMatrixTranslation(-+5.0f, 1.5f, -10.0f + i * 5.0f);

        rightCylRitem->WorldMatrix = RightCylWorld;
        rightCylRitem->Mesh = m_pScene->Meshes["shapeGeo"].get();
        rightCylRitem->ObjCBIndex = objIndex++;
        rightCylRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        rightCylRitem->IndexCount = 
            rightCylRitem->Mesh->SubMesh["cylinder"].IndexCount;
        rightCylRitem->StartVertexLocation =
            rightCylRitem->Mesh->SubMesh["cylinder"].StartVertexLocation;
        rightCylRitem->StartIndexLocation =
            rightCylRitem->Mesh->SubMesh["cylinder"].StartIndexLocation;
        rightCylRitem->DirtyCount = g_GraphicsConfig.FrameResorceCount;
        m_pScene->OpaqueRenderItems.push_back(std::move(rightCylRitem.get()));
        m_pScene->AllRenderItems.push_back(std::move(rightCylRitem));

        auto leftSphereRitem = std::make_unique<Acorn::RenderItem>();
        DirectX::XMMATRIX leftSphereWorld =
            DirectX::XMMatrixTranslation(-5.0f, 3.5f, -10.0f + i * 5.0f);

        leftSphereRitem->WorldMatrix = leftSphereWorld;
        leftSphereRitem->Mesh = m_pScene->Meshes["shapeGeo"].get();
        leftSphereRitem->ObjCBIndex = objIndex++;
        leftSphereRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        leftSphereRitem->IndexCount = 
            leftSphereRitem->Mesh->SubMesh["sphere"].IndexCount;
        leftSphereRitem->StartVertexLocation =
            leftSphereRitem->Mesh->SubMesh["sphere"].StartVertexLocation;
        leftSphereRitem->StartIndexLocation =
            leftSphereRitem->Mesh->SubMesh["sphere"].StartIndexLocation;
        leftSphereRitem->DirtyCount = g_GraphicsConfig.FrameResorceCount;
        m_pScene->OpaqueRenderItems.push_back(std::move(leftSphereRitem.get()));
        m_pScene->AllRenderItems.push_back(std::move(leftSphereRitem));

        auto rightSphereRitem = std::make_unique<Acorn::RenderItem>();
        DirectX::XMMATRIX RightSphereWorld =
            DirectX::XMMatrixTranslation(-+5.0f, 3.5f, -10.0f + i * 5.0f);

        rightCylRitem->WorldMatrix = RightSphereWorld;
        rightCylRitem->Mesh = m_pScene->Meshes["shapeGeo"].get();
        rightCylRitem->ObjCBIndex = objIndex++;
        rightCylRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        rightCylRitem->IndexCount = 
            rightCylRitem->Mesh->SubMesh["sphere"].IndexCount;
        rightCylRitem->StartVertexLocation =
            rightCylRitem->Mesh->SubMesh["sphere"].StartVertexLocation;
        rightCylRitem->StartIndexLocation =
            rightCylRitem->Mesh->SubMesh["sphere"].StartIndexLocation;
        rightCylRitem->DirtyCount = g_GraphicsConfig.FrameResorceCount;
        m_pScene->OpaqueRenderItems.push_back(std::move(rightCylRitem.get()));
        m_pScene->AllRenderItems.push_back(std::move(rightCylRitem));
    }

}

inline void Win32App::UpdateInput(){
    static int x = 0;
    static int y = 0;
    static int x_t = 0;
    static int y_t = 0;

    if(m_Mouse.IsKeyDown(0)){
        Acorn::Point2<int16_t> p = m_Mouse.GetDeltaPosition();
        x_t = p.x;
        y_t = p.y;
    }
    else{
        x += x_t;
        y += y_t;
        x_t = 0;
        y_t = 0; 
    }

    float theta = 0.25f * DirectX::XMConvertToRadians(x + x_t);
    float phi = 0.25f * DirectX::XMConvertToRadians(y + y_t);
    m_pScene->MainCamera.RotateY(theta);
    m_pScene->MainCamera.Pitch(-phi);
}

LRESULT CALLBACK Win32App::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    return s_pInstance->MsgProc(hwnd, uMsg, wParam, lParam);
}

