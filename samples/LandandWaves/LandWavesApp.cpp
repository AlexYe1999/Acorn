#include"LandWavesApp.hpp"

Acorn::GraphicsParam g_GraphicsConfig;

LandWavesApp::LandWavesApp()
    : 
    m_pScene(nullptr),
    m_pWaves(nullptr),
    m_pGraphicsManager(nullptr)
{}

void LandWavesApp::InitApp(
    const HINSTANCE appInstance, int nCmdShow, 
    const int16_t width, const int16_t height,
    const std::string& wndClassName,
    const std::string& wndText
){
    InitWnd(appInstance, nCmdShow, width, height, wndClassName, wndText);

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

    m_pGraphicsManager.reset(Acorn::D3D12GraphicsManager::GetInstance());

    BuildScene();

    m_pGraphicsManager->Initialize(m_pScene.get(), &m_Timer);

    m_bIsInit = true;
}

void LandWavesApp::RunApp(){
    if(!m_bIsInit){
        throw std::runtime_error("app is not init");
    }

    uint64_t frameCount = 0;
    float totalTime = 0.0f;
    m_Timer.Reset();

    while(!m_bIsQuit){
        MSG msg = {};

        if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        m_Timer.Tick();

        UpdateInput();        
        UpdateScene();

        m_pGraphicsManager->Tick();

        totalTime += m_Timer.DeltaTime();
        frameCount++;

        std::string text = "Land and Waves";
        text += "  " + std::to_string(static_cast<int>(frameCount / totalTime)) + " fps ";

        SetWindowText(m_hWnd, text.c_str());

        if(frameCount > 1000){
            frameCount = 0;
            totalTime = 0.0f;
        }
    }
}

void LandWavesApp::BuildScene(){

    m_pScene = std::make_unique<Acorn::Scene>();
    m_pScene->MainCamera.LookAt(
        Acorn::Vector3f(-49.0f, 40.0f, 50.0f),
        Acorn::Vector3f(0.0f, 0.0f, 0.0f),
        Acorn::Vector3f(0.0f, 1.0f, 0.0f)
    );
    CreateMaterial();
    CreateMesh();
    CreateRenderItem();
}

inline Acorn::Vector3f GetHillsNormal(float x, float z){

    Acorn::Vector3f n(
        -0.03f*z*cosf(0.1f*x) - 0.3f*cosf(0.1f*z),
        1.0f,
        -0.3f*sinf(0.1f*x) + 0.03f*x*sinf(0.1f*z));

    DirectX::XMVECTOR unitNormal = Acorn::XMVector3Normalize(XMLoadFloat3(&n));
    XMStoreFloat3(&n, unitNormal);

    return n;
}

float GetHillsHeight(float x, float z){
    return 0.3f*(z*sinf(0.1f*x) + x*cosf(0.1f*z));
}

void LandWavesApp::CreateMesh(){
    using GeoGenerator = LemonCube::GeometryGenerator;
    m_pWaves = std::make_unique<Waves>(105.0f, 105.0f, 1.0f, 0.03f, 4.0f, 0.2f);

    GeoGenerator geoGen;
    GeoGenerator::MeshData grid = geoGen.CreateGrid(105.0f, 105.0f, 50, 50);
    std::vector<Acorn::VertexPNC> landVertices(grid.vertices.size());

    for(uint16_t index = 0; index < landVertices.size(); index++){
        auto& p = grid.vertices[index].position;

        landVertices[index].Position = p;
        landVertices[index].Normal = GetHillsNormal(p.x, p.z);
        landVertices[index].Position.y = GetHillsHeight(p.x, p.z);

        if(landVertices[index].Position.y < -10.0f){
            landVertices[index].Color = 
                std::move(Acorn::Vector4f(1.0f, 0.96f, 0.62f, 1.0f));
        }
        else if(landVertices[index].Position.y < 5.0f){
            landVertices[index].Color = 
                std::move(Acorn::Vector4f(0.48f, 0.77f, 0.46f, 1.0f));
        }
        else if(landVertices[index].Position.y < 12.0f){
            landVertices[index].Color = 
                std::move(Acorn::Vector4f(0.1f, 0.48f, 0.19f, 1.0f));
        }
        else if(landVertices[index].Position.y < 20.0f){
            landVertices[index].Color = 
                std::move(Acorn::Vector4f(0.45f, 0.39f, 0.34f, 1.0f));
        }
        else{
            landVertices[index].Color = 
                std::move(Acorn::Vector4f(1.0f, 1.0f, 1.0f, 1.0f));
        }

        std::unique_ptr<Acorn::Mesh> land = std::make_unique<Acorn::Mesh>();
        land->Name = "LandGeo";

        const uint32_t landVbByteSize = grid.vertices.size() * sizeof(Acorn::VertexPNC);
        const uint32_t landIbByteSize = grid.indices.size() * sizeof(uint16_t);

        D3DCreateBlob(landVbByteSize, land->VertexBufferCPU.GetAddressOf());
        D3DCreateBlob(landIbByteSize, land->IndexBufferCPU.GetAddressOf());

        CopyMemory(land->VertexBufferCPU->GetBufferPointer(), landVertices.data(), landVbByteSize);
        CopyMemory(land->IndexBufferCPU->GetBufferPointer(), grid.indices.data(), landIbByteSize);

        land->VertexByteStride = sizeof(Acorn::VertexPNC);
        land->IndexFormat = DXGI_FORMAT_R16_UINT;
        land->VertexBufferByteSize = landVbByteSize;
        land->IndexBufferByteSize = landIbByteSize;

        Acorn::SubMesh landSubMesh;
        landSubMesh.StartVertexLocation = 0;
        landSubMesh.StartIndexLocation = 0;
        landSubMesh.IndexCount = grid.indices.size();
        land->SubMesh["Land"] = std::move(landSubMesh);

        m_pScene->Meshes["LandGeo"] = std::move(land);

        std::vector<std::uint16_t> indices(3 * m_pWaves->TriangleCount());

        uint16_t m = m_pWaves->RowCount();
        uint16_t n = m_pWaves->ColumnCount();
        uint16_t k = 0;
        for(uint16_t i = 0; i < m - 1; ++i){

            for(uint16_t j = 0; j < n - 1; ++j){
                indices[k] = i * n + j;
                indices[k + 1] = i * n + j + 1;
                indices[k + 2] = (i + 1) * n + j;

                indices[k + 3] = (i + 1) * n + j;
                indices[k + 4] = i * n + j + 1;
                indices[k + 5] = (i + 1) * n + j + 1;

                k += 6;
            }
        }

        const uint32_t waveVbByteSize = m_pWaves->VertexCount() * sizeof(Acorn::VertexPNC);
        const uint32_t waveIbByteSize = indices.size() * sizeof(uint16_t);

        auto wave = std::make_unique<Acorn::Mesh>();
        wave->Name = "WaveGeo";

        D3DCreateBlob(waveVbByteSize, wave->VertexBufferCPU.GetAddressOf());
        D3DCreateBlob(waveIbByteSize, wave->IndexBufferCPU.GetAddressOf());

        CopyMemory(wave->IndexBufferCPU->GetBufferPointer(), indices.data(), waveIbByteSize);

        wave->VertexByteStride = sizeof(Acorn::VertexPNC);
        wave->IndexFormat = DXGI_FORMAT_R16_UINT;
        wave->VertexBufferByteSize = waveVbByteSize;
        wave->IndexBufferByteSize = waveIbByteSize;

        Acorn::SubMesh waveSubMesh;
        waveSubMesh.StartVertexLocation = 0;
        waveSubMesh.StartIndexLocation = 0;
        waveSubMesh.IndexCount = indices.size();
        wave->SubMesh["Wave"] = std::move(waveSubMesh);

        m_pScene->DynamicMeshes["WaveGeo"] = std::move(wave);
    }
}

void LandWavesApp::CreateMaterial(){

    auto grass = std::make_unique<Acorn::Material>();
    grass->Name = "grass";
    grass->MatCBIndex = 0;
    grass->DiffuseAlbedo = Acorn::Vector4f(0.2f, 0.6f, 0.2f, 1.0f);
    grass->FresnelR0 = Acorn::Vector3f(0.01f, 0.01f, 0.01f);
    grass->Roughness = 0.125f;
    grass->NumFramesDirty = g_GraphicsConfig.FrameResourceCount;

    auto water = std::make_unique<Acorn::Material>();
    water->Name = "water";
    water->MatCBIndex = 1;
    water->DiffuseAlbedo = Acorn::Vector4f(0.0f, 0.2f, 0.6f, 1.0f);
    water->FresnelR0 = Acorn::Vector3f(0.1f, 0.1f, 0.1f);
    water->Roughness = 0.0f;
    water->NumFramesDirty = g_GraphicsConfig.FrameResourceCount;

    m_pScene->Materials["grass"] = std::move(grass);
    m_pScene->Materials["water"] = std::move(water);
}

void LandWavesApp::CreateRenderItem(){

    uint16_t objIndex = 0;

    auto land = std::make_unique<Acorn::RenderItem>();
    land->World = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
    land->Mesh = m_pScene->Meshes["LandGeo"].get();
    land->Mat = m_pScene->Materials["grass"].get();
    land->ObjCBIndex = objIndex++;
    land->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    land->IndexCount = land->Mesh->SubMesh["Land"].IndexCount;
    land->StartVertexLocation = land->Mesh->SubMesh["Land"].StartVertexLocation;
    land->StartIndexLocation = land->Mesh->SubMesh["Land"].StartIndexLocation;
    land->DirtyCount = g_GraphicsConfig.FrameResourceCount;
    m_pScene->OpaqueRenderItems.push_back(std::move(land.get()));
    m_pScene->AllRenderItems.push_back(std::move(land));

    auto wave = std::make_unique<Acorn::RenderItem>();
    wave->World = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
    wave->Mesh = m_pScene->DynamicMeshes["WaveGeo"].get();
    wave->Mat = m_pScene->Materials["water"].get();
    wave->ObjCBIndex = objIndex++;
    wave->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    wave->IndexCount = wave->Mesh->SubMesh["Wave"].IndexCount;
    wave->StartVertexLocation = wave->Mesh->SubMesh["Wave"].StartVertexLocation;
    wave->StartIndexLocation = wave->Mesh->SubMesh["Wave"].StartIndexLocation;
    wave->DirtyCount = g_GraphicsConfig.FrameResourceCount;
    m_pScene->OpaqueRenderItems.push_back(std::move(wave.get()));
    m_pScene->AllRenderItems.push_back(std::move(wave));
}

void LandWavesApp::UpdateInput(){
    static Acorn::Point2<int16_t> deltaP(0, 0);
    if(m_Mouse.IsKeyDown(0)){
        deltaP = m_Mouse.GetDeltaPosition();
    }
    else{
        deltaP = Acorn::Point2<int16_t>(0, 0);
    }

    float theta = 0.002f * DirectX::XMConvertToRadians(deltaP.x);
    float phi = 0.002f * DirectX::XMConvertToRadians(deltaP.y);
    m_pScene->MainCamera.RotateY(theta);
    m_pScene->MainCamera.Pitch(phi);
}

void LandWavesApp::UpdateScene(){
    const uint32_t waveVbByteSize = m_pWaves->VertexCount() * sizeof(Acorn::VertexPNC);
    const auto& vertexBuffer = m_pScene->DynamicMeshes["WaveGeo"]->VertexBufferCPU->GetBufferPointer();
    std::vector<Acorn::VertexPNC> vertices(m_pWaves->VertexCount());

	static float t_base = 0.0f;
	if((m_Timer.TotalTime() - t_base) >= 0.25f)
	{
		t_base += 0.25f;

		int i = 4 + rand() % (m_pWaves->RowCount() - 8);
		int j = 4 + rand() % (m_pWaves->ColumnCount() - 8);

		float r = 0.2f + ((float)rand() / (float)RAND_MAX)*0.3f;

		m_pWaves->Disturb(i, j, r);
	}

	m_pWaves->Update(m_Timer.DeltaTime());


	for(int i = 0; i < m_pWaves->VertexCount(); ++i){
        vertices[i].Position = m_pWaves->Position(i);
        vertices[i].Normal = m_pWaves->Normal(i);
        vertices[i].Color = Acorn::Vector4f(1.0f, 0.2f, 0.2f, 1.0f);
	}

    CopyMemory(vertexBuffer, vertices.data(), waveVbByteSize);
}

LRESULT LandWavesApp::MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
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
                break;
            }
        }

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}