#include"MultiInsApp.hpp"

Acorn::GraphicsParam g_GraphicsConfig;

MultiInsApp::MultiInsApp()
    : 
    m_pScene(nullptr),
    m_pWaves(nullptr),
    m_pGraphicsManager(nullptr)
{}

void MultiInsApp::InitApp(
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

void MultiInsApp::RunApp(){
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

void MultiInsApp::BuildScene(){

    m_pScene = std::make_unique<Acorn::Scene>();
    m_pScene->MainCamera.LookAt(
        Acorn::Vector3f(-98.0f, 40.0f, 100.0f),
        Acorn::Vector3f(0.0f, 0.0f, 0.0f),
        Acorn::Vector3f(0.0f, 1.0f, 0.0f)
    );
    CreateTexture();
    CreateMaterial();
    CreateMesh();
    CreateRenderItem();
}

void MultiInsApp::CreateTexture(){

    auto Bricks0Tex = std::make_unique<Acorn::Texture>();
    Bricks0Tex->FileName = L"E:/Code/Acorn/samples/MultiInsApp/texture/bricks0.dds";
    Bricks0Tex->Name = "Bricks0";

    auto Bricks1Tex = std::make_unique<Acorn::Texture>();
    Bricks1Tex->FileName = L"E:/Code/Acorn/samples/MultiInsApp/texture/bricks1.dds";
    Bricks1Tex->Name = "Bricks1";

    auto Bricks2Tex = std::make_unique<Acorn::Texture>();
    Bricks2Tex->FileName = L"E:/Code/Acorn/samples/MultiInsApp/texture/bricks2.dds";
    Bricks2Tex->Name = "Bricks2";

    auto GrassTex = std::make_unique<Acorn::Texture>();
    GrassTex->FileName = L"E:/Code/Acorn/samples/MultiInsApp/texture/grass.dds";
    GrassTex->Name = "Grass";

    auto IceTex = std::make_unique<Acorn::Texture>();
    IceTex->FileName = L"E:/Code/Acorn/samples/MultiInsApp/texture/ice.dds";
    IceTex->Name = "Ice";

    auto WaterTex = std::make_unique<Acorn::Texture>();
    WaterTex->FileName = L"E:/Code/Acorn/samples/MultiInsApp/texture/water.dds";
    WaterTex->Name = "Water";

    auto Wood1Tex = std::make_unique<Acorn::Texture>();
    Wood1Tex->FileName = L"E:/Code/Acorn/samples/MultiInsApp/texture/WoodCrate1.dds";
    Wood1Tex->Name = "Wood1";

    auto Wood2Tex = std::make_unique<Acorn::Texture>();
    Wood2Tex->FileName = L"E:/Code/Acorn/samples/MultiInsApp/texture/WoodCrate2.dds";
    Wood2Tex->Name = "Wood2";

    m_pScene->Textures[Bricks0Tex->Name] = std::move(Bricks0Tex);
    m_pScene->Textures[Bricks1Tex->Name] = std::move(Bricks1Tex);
    m_pScene->Textures[Bricks2Tex->Name] = std::move(Bricks2Tex);
    m_pScene->Textures[GrassTex->Name] = std::move(GrassTex);
    m_pScene->Textures[IceTex->Name] = std::move(IceTex);
    m_pScene->Textures[WaterTex->Name] = std::move(WaterTex);
    m_pScene->Textures[Wood1Tex->Name] = std::move(Wood1Tex);
    m_pScene->Textures[Wood2Tex->Name] = std::move(Wood2Tex);
}

void MultiInsApp::CreateMaterial(){

    uint16_t textureIndex = 0;

    auto brick0 = std::make_unique<Acorn::Material>("Brick0");
	brick0->MatCBIndex = textureIndex;
	brick0->DiffuseSrvHeapIndex = textureIndex++;
	brick0->DiffuseAlbedo = Acorn::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
	brick0->FresnelR0 = Acorn::Vector3f(0.02f, 0.02f, 0.02f);
	brick0->Roughness = 0.1f;
    brick0->NumFramesDirty = g_GraphicsConfig.FrameResourceCount;

    auto brick1 = std::make_unique<Acorn::Material>("Brick1");
	brick1->MatCBIndex = textureIndex;
	brick1->DiffuseSrvHeapIndex = textureIndex++;
	brick1->DiffuseAlbedo = Acorn::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
	brick1->FresnelR0 = Acorn::Vector3f(0.02f, 0.02f, 0.02f);
	brick1->Roughness = 0.1f;
    brick1->NumFramesDirty = g_GraphicsConfig.FrameResourceCount;

    auto brick2 = std::make_unique<Acorn::Material>("Brick2");
	brick2->MatCBIndex = textureIndex;
	brick2->DiffuseSrvHeapIndex = textureIndex++;
	brick2->DiffuseAlbedo = Acorn::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
	brick2->FresnelR0 = Acorn::Vector3f(0.02f, 0.02f, 0.02f);
	brick2->Roughness = 0.1f;
    brick2->NumFramesDirty = g_GraphicsConfig.FrameResourceCount;

    auto grass = std::make_unique<Acorn::Material>("Grass");
    grass->MatTransform =  DirectX::XMMatrixScaling(5.0f, 5.0f, 1.0f);
    grass->MatCBIndex = textureIndex;
    grass->DiffuseSrvHeapIndex = textureIndex++;
    grass->DiffuseAlbedo = Acorn::Vector4f(0.6f, 0.6f, 0.6f, 1.0f);
    grass->FresnelR0 = Acorn::Vector3f(0.01f, 0.01f, 0.01f);
    grass->Roughness = 0.125f;
    grass->NumFramesDirty = g_GraphicsConfig.FrameResourceCount;

	auto ice0 = std::make_unique<Acorn::Material>("Ice");
	ice0->MatCBIndex = textureIndex;
	ice0->DiffuseSrvHeapIndex = textureIndex++;
	ice0->DiffuseAlbedo = Acorn::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
	ice0->FresnelR0 = Acorn::Vector3f(0.1f, 0.1f, 0.1f);
	ice0->Roughness = 0.0f;

    auto water = std::make_unique<Acorn::Material>("Water");
    water->MatCBIndex = textureIndex;
    water->DiffuseSrvHeapIndex = textureIndex++;
    water->DiffuseAlbedo = Acorn::Vector4f(0.0f, 0.2f, 0.6f, 0.5f);
    water->FresnelR0 = Acorn::Vector3f(0.1f, 0.1f, 0.1f);
    water->Roughness = 0.0f;
    water->NumFramesDirty = g_GraphicsConfig.FrameResourceCount;

    auto crate1 = std::make_unique<Acorn::Material>("Crate1");
	crate1->MatCBIndex = textureIndex;
	crate1->DiffuseSrvHeapIndex = textureIndex++;
	crate1->DiffuseAlbedo = Acorn::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
	crate1->FresnelR0 = Acorn::Vector3f(0.05f, 0.05f, 0.05f);
	crate1->Roughness = 0.2f;
    crate1->NumFramesDirty = g_GraphicsConfig.FrameResourceCount;

    auto crate2 = std::make_unique<Acorn::Material>("Crate2");
	crate2->MatCBIndex = textureIndex;
	crate2->DiffuseSrvHeapIndex = textureIndex++;
	crate2->DiffuseAlbedo = Acorn::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
	crate2->FresnelR0 = Acorn::Vector3f(0.05f, 0.05f, 0.05f);
	crate2->Roughness = 0.2f;
    crate2->NumFramesDirty = g_GraphicsConfig.FrameResourceCount;

    m_pScene->Materials[brick0->Name] = std::move(brick0);
    m_pScene->Materials[brick1->Name] = std::move(brick1);
    m_pScene->Materials[brick2->Name] = std::move(brick2);
    m_pScene->Materials[grass->Name]  = std::move(grass);
    m_pScene->Materials[ice0->Name]   = std::move(ice0);
    m_pScene->Materials[water->Name]  = std::move(water);
    m_pScene->Materials[crate1->Name] = std::move(crate1);
    m_pScene->Materials[crate2->Name] = std::move(crate2);
}

void MultiInsApp::CreateMesh(){
    using GeoGenerator = LemonCube::GeometryGenerator;

    GeoGenerator geoGen;

    // Mesh of box 
    GeoGenerator::MeshData boxGeo = geoGen.CreateBox(1.0f, 1.0f, 1.0f, 3);
    std::vector<Acorn::Vertex> boxVertices(boxGeo.vertices.size());

    for(uint16_t index = 0; index < boxVertices.size(); index++){
        boxVertices[index].Position = boxGeo.vertices[index].position;
        boxVertices[index].Normal = boxGeo.vertices[index].normal;
        boxVertices[index].TexC = boxGeo.vertices[index].uv;
    }
    
    std::unique_ptr<Acorn::Mesh> box = std::make_unique<Acorn::Mesh>("BoxGeo");

    const uint32_t boxVbByteSize = boxVertices.size() * sizeof(Acorn::Vertex);
    const uint32_t boxIbByteSize = boxGeo.indices.size() * sizeof(uint16_t);

    D3DCreateBlob(boxVbByteSize, box->VertexBufferCPU.GetAddressOf());
    D3DCreateBlob(boxIbByteSize, box->IndexBufferCPU.GetAddressOf());

    CopyMemory(box->VertexBufferCPU->GetBufferPointer(), boxVertices.data(), boxVbByteSize);
    CopyMemory(box->IndexBufferCPU->GetBufferPointer(), boxGeo.indices.data(), boxIbByteSize);

    box->VertexByteStride = sizeof(Acorn::Vertex);
    box->IndexFormat = DXGI_FORMAT_R16_UINT;
    box->VertexBufferByteSize = boxVbByteSize;
    box->IndexBufferByteSize = boxIbByteSize;

    Acorn::SubMesh boxSubMesh;
    boxSubMesh.StartVertexLocation = 0;
    boxSubMesh.StartIndexLocation = 0;
    boxSubMesh.IndexCount = boxGeo.indices.size();
    DirectX::BoundingBox::CreateFromPoints(
        boxSubMesh.BBox, boxVertices.size(), 
        &boxVertices[0].Position, sizeof(Acorn::Vertex)
    );

    box->SubMesh["Box"] = std::move(boxSubMesh);
    m_pScene->Meshes[box->Name] = std::move(box);

}


void MultiInsApp::CreateRenderItem(){

    auto boxes = std::make_unique<Acorn::RenderInstance>();
    boxes->Mesh = m_pScene->Meshes["BoxGeo"].get();
    boxes->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    boxes->IndexCount = boxes->Mesh->SubMesh["Box"].IndexCount;
    boxes->StartVertexLocation = boxes->Mesh->SubMesh["Box"].StartVertexLocation;
    boxes->StartIndexLocation = boxes->Mesh->SubMesh["Box"].StartIndexLocation;
    boxes->DirtyCount = g_GraphicsConfig.FrameResourceCount;
    boxes->SubMesh = &boxes->Mesh->SubMesh["Box"];

    for(int x = -50; x <= 50; x++){
        for(int y = -50; y <= 50; y++){
            Acorn::InstanceData instanceData;

            DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(2.0f * x, 2.0f * y, 0.0);
            DirectX::XMMATRIX worldInv = XMMatrixInverse(nullptr, world);

            instanceData.World = XMMatrixTranspose(world);
            instanceData.WorldIT = XMMatrixTranspose(worldInv);
            instanceData.MaterialIndex = (x + 500) % 8;
            
            boxes->InstanceDataArray.push_back(std::move(instanceData));
        }
    }

    m_pScene->Instances = std::move(boxes);
}

void MultiInsApp::UpdateInput(){
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

void MultiInsApp::UpdateScene(){

    Acorn::Vector3f forward(0.0f, 0.0f, 1.0f);
    if(m_Keyboard.IsKeyDown('W')){
        m_pScene->MainCamera.Walk(forward, 0.5f);
    }
    if(m_Keyboard.IsKeyDown('S')){
        m_pScene->MainCamera.Walk(forward*-1.0f, 0.5f);
    }
    Acorn::Vector3f right(1.0f, 0.0f, 0.0f);
    if(m_Keyboard.IsKeyDown('D')){
        m_pScene->MainCamera.Walk(right, 0.5f);
    }
    if(m_Keyboard.IsKeyDown('A')){
        m_pScene->MainCamera.Walk(right*-1.0f, 0.5f);
    }

}

LRESULT MultiInsApp::MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch (uMsg){
        case WM_KEYDOWN:{
            m_Keyboard.KeyDown(wParam);
            break;
        }
        case WM_KEYUP:{
            m_Keyboard.KeyUp(wParam);
            break;
        }
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