#include"TextureApp.hpp"

Acorn::GraphicsParam g_GraphicsConfig;

TextureApp::TextureApp()
    : 
    m_pScene(nullptr),
    m_pWaves(nullptr),
    m_pGraphicsManager(nullptr)
{}

void TextureApp::InitApp(
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

void TextureApp::RunApp(){
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

void TextureApp::BuildScene(){

    m_pScene = std::make_unique<Acorn::Scene>();
    m_pScene->MainCamera.LookAt(
        Acorn::Vector3f(-98.0f, 80.0f, 100.0f),
        Acorn::Vector3f(0.0f, 0.0f, 0.0f),
        Acorn::Vector3f(0.0f, 1.0f, 0.0f)
    );
    CreateTexture();
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

void TextureApp::CreateTexture(){

    auto grassTex = std::make_unique<Acorn::Texture>();
    grassTex->FileName = L"E:/Code/Acorn/samples/TextureMapping/texture/grass.dds";
    grassTex->Name = "GrassTexture";

    auto waveTex = std::make_unique<Acorn::Texture>();
    waveTex->FileName = L"E:/Code/Acorn/samples/TextureMapping/texture/water.dds";
    waveTex->Name = "WaveTexture";

    auto crateTex = std::make_unique<Acorn::Texture>();
    crateTex->FileName = L"E:/Code/Acorn/samples/TextureMapping/texture/WireFence.dds";
    crateTex->Name = "CrateTexture";
    
    auto mirrorTex = std::make_unique<Acorn::Texture>();
    mirrorTex->FileName = L"E:/Code/Acorn/samples/TextureMapping/texture/ice.dds";
    mirrorTex->Name = "MirrorTexture";

    m_pScene->Textures[grassTex->Name] = std::move(grassTex);
    m_pScene->Textures[waveTex->Name] = std::move(waveTex);
    m_pScene->Textures[crateTex->Name] = std::move(crateTex);
    m_pScene->Textures[mirrorTex->Name] = std::move(mirrorTex);

}

void TextureApp::CreateMaterial(){

    auto grass = std::make_unique<Acorn::Material>();
    grass->Name = "grass";
    grass->MatTransform =  DirectX::XMMatrixScaling(5.0f, 5.0f, 1.0f);
    grass->MatCBIndex = 0;
    grass->DiffuseSrvHeapIndex = 0;
    grass->DiffuseAlbedo = Acorn::Vector4f(0.2f, 0.6f, 0.2f, 1.0f);
    grass->FresnelR0 = Acorn::Vector3f(0.01f, 0.01f, 0.01f);
    grass->Roughness = 0.125f;
    grass->NumFramesDirty = g_GraphicsConfig.FrameResourceCount;

    auto water = std::make_unique<Acorn::Material>();
    water->Name = "water";
    water->MatCBIndex = 1;
    water->DiffuseSrvHeapIndex = 1;
    water->DiffuseAlbedo = Acorn::Vector4f(0.0f, 0.2f, 0.6f, 0.5f);
    water->FresnelR0 = Acorn::Vector3f(0.1f, 0.1f, 0.1f);
    water->Roughness = 0.0f;
    water->NumFramesDirty = g_GraphicsConfig.FrameResourceCount;

    auto crate = std::make_unique<Acorn::Material>();
    crate->Name = "crate";
	crate->MatCBIndex = 2;
	crate->DiffuseSrvHeapIndex = 2;
	crate->DiffuseAlbedo = Acorn::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
	crate->FresnelR0 = Acorn::Vector3f(0.05f, 0.05f, 0.05f);
	crate->Roughness = 0.2f;
    crate->NumFramesDirty = g_GraphicsConfig.FrameResourceCount;

    auto mirror = std::make_unique<Acorn::Material>();
    mirror->Name = "mirror";
	mirror->MatCBIndex = 3;
	mirror->DiffuseSrvHeapIndex = 3;
	mirror->DiffuseAlbedo = Acorn::Vector4f(1.0f, 1.0f, 1.0f, 0.3f);
	mirror->FresnelR0 = Acorn::Vector3f(0.1f, 0.1f, 0.1f);
	mirror->Roughness = 0.5f;
    mirror->NumFramesDirty = g_GraphicsConfig.FrameResourceCount;

    auto shadow = std::make_unique<Acorn::Material>();
    shadow->Name = "shadow";
	shadow->MatCBIndex = 4;
	shadow->DiffuseSrvHeapIndex = 3;
	shadow->DiffuseAlbedo = Acorn::Vector4f(0.0f, 0.0f, 0.0f, 0.5f);
	shadow->FresnelR0 = Acorn::Vector3f(0.001f, 0.001f, 0.001f);
	shadow->Roughness = 0.0f;
    shadow->NumFramesDirty = g_GraphicsConfig.FrameResourceCount;

    m_pScene->Materials[grass->Name] = std::move(grass);
    m_pScene->Materials[water->Name] = std::move(water);
    m_pScene->Materials[crate->Name] = std::move(crate);
    m_pScene->Materials[mirror->Name] = std::move(mirror);
    m_pScene->Materials[shadow->Name] = std::move(shadow);
}

void TextureApp::CreateMesh(){
    using GeoGen = LemonCube::GeometryGenerator;
    GeoGen geoGen;

    GeoGen::MeshData boxGeo = geoGen.CreateBox(1.0f, 1.0f, 1.0f, 3);
    std::unique_ptr<Acorn::Mesh> box = std::make_unique<Acorn::Mesh>("BoxGeo");

    std::vector<Acorn::Vertex> boxVertices(boxGeo.vertices.size());

    for(uint16_t index = 0; index < boxVertices.size(); index++){
        boxVertices[index].Position = boxGeo.vertices[index].position;
        boxVertices[index].Normal = boxGeo.vertices[index].normal;
        boxVertices[index].TexC = boxGeo.vertices[index].uv;
    }
    
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

    box->SubMesh["Box"] = std::move(boxSubMesh);
    m_pScene->Meshes[box->Name] = std::move(box);


    GeoGen::MeshData grid = geoGen.CreateGrid(105.0f, 105.0f, 50, 50);
    std::unique_ptr<Acorn::Mesh> land = std::make_unique<Acorn::Mesh>("LandGeo");

    std::vector<Acorn::Vertex> landVertices(grid.vertices.size());

    for(uint16_t index = 0; index < landVertices.size(); index++){
        auto& p = grid.vertices[index].position;

        landVertices[index].Position = p;
        landVertices[index].Position.y = GetHillsHeight(p.x, p.z);
        landVertices[index].Normal = GetHillsNormal(p.x, p.z);
        landVertices[index].TexC = grid.vertices[index].uv;
    }

    const uint32_t landVbByteSize = grid.vertices.size() * sizeof(Acorn::Vertex);
    const uint32_t landIbByteSize = grid.indices.size() * sizeof(uint16_t);

    D3DCreateBlob(landVbByteSize, land->VertexBufferCPU.GetAddressOf());
    D3DCreateBlob(landIbByteSize, land->IndexBufferCPU.GetAddressOf());

    CopyMemory(land->VertexBufferCPU->GetBufferPointer(), landVertices.data(), landVbByteSize);
    CopyMemory(land->IndexBufferCPU->GetBufferPointer(), grid.indices.data(), landIbByteSize);

    land->VertexByteStride = sizeof(Acorn::Vertex);
    land->IndexFormat = DXGI_FORMAT_R16_UINT;
    land->VertexBufferByteSize = landVbByteSize;
    land->IndexBufferByteSize = landIbByteSize;

    Acorn::SubMesh landSubMesh;
    landSubMesh.StartVertexLocation = 0;
    landSubMesh.StartIndexLocation = 0;
    landSubMesh.IndexCount = grid.indices.size();

    land->SubMesh["Land"] = std::move(landSubMesh);
    m_pScene->Meshes[land->Name] = std::move(land);

    m_pWaves = std::make_unique<Waves>(105.0f, 105.0f, 1.0f, 0.03f, 4.0f, 0.2f);
    auto wave = std::make_unique<Acorn::Mesh>("WaveGeo");

    std::vector<std::uint16_t> waveIndices(3 * m_pWaves->TriangleCount());

    uint16_t m = m_pWaves->RowCount();
    uint16_t n = m_pWaves->ColumnCount();
    uint16_t k = 0;
    for(uint16_t i = 0; i < m - 1; ++i){

        for(uint16_t j = 0; j < n - 1; ++j){
            waveIndices[k] = i * n + j;
            waveIndices[k + 1] = i * n + j + 1;
            waveIndices[k + 2] = (i + 1) * n + j;

            waveIndices[k + 3] = (i + 1) * n + j;
            waveIndices[k + 4] = i * n + j + 1;
            waveIndices[k + 5] = (i + 1) * n + j + 1;

            k += 6;
        }
    }

    const uint32_t waveVbByteSize = m_pWaves->VertexCount() * sizeof(Acorn::Vertex);
    const uint32_t waveIbByteSize = waveIndices.size() * sizeof(uint16_t);

    D3DCreateBlob(waveVbByteSize, wave->VertexBufferCPU.GetAddressOf());
    D3DCreateBlob(waveIbByteSize, wave->IndexBufferCPU.GetAddressOf());

    CopyMemory(wave->IndexBufferCPU->GetBufferPointer(), waveIndices.data(), waveIbByteSize);

    wave->VertexByteStride = sizeof(Acorn::Vertex);
    wave->IndexFormat = DXGI_FORMAT_R16_UINT;
    wave->VertexBufferByteSize = waveVbByteSize;
    wave->IndexBufferByteSize = waveIbByteSize;

    Acorn::SubMesh waveSubMesh;
    waveSubMesh.StartVertexLocation = 0;
    waveSubMesh.StartIndexLocation = 0;
    waveSubMesh.IndexCount = waveIndices.size();

    wave->SubMesh["Wave"] = std::move(waveSubMesh);
    m_pScene->DynamicMeshes[wave->Name] = std::move(wave);

    std::unique_ptr<Acorn::Mesh> mirror = std::make_unique<Acorn::Mesh>("MirrorGeo");
    
    std::vector<Acorn::Vertex> mirrorVetices = {
        Acorn::Vertex(-4.0f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
        Acorn::Vertex(4.0f,  4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),
        Acorn::Vertex(4.0f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),
        Acorn::Vertex(-4.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f)
    };
    std::vector<std::uint16_t> mirrorIndices = {
        0, 1, 2,
        0, 2, 3,
        2, 1, 0,
        3, 2, 0
    };

    const uint32_t mirrorVbByteSize = mirrorVetices.size() * sizeof(Acorn::Vertex);
    const uint32_t mirrorIbByteSize = mirrorIndices.size() * sizeof(uint16_t);

    D3DCreateBlob(mirrorVbByteSize, mirror->VertexBufferCPU.GetAddressOf());
    D3DCreateBlob(mirrorIbByteSize, mirror->IndexBufferCPU.GetAddressOf());

    CopyMemory(mirror->VertexBufferCPU->GetBufferPointer(), mirrorVetices.data(), mirrorVbByteSize);
    CopyMemory(mirror->IndexBufferCPU->GetBufferPointer(), mirrorIndices.data(), mirrorIbByteSize);

    mirror->VertexByteStride = sizeof(Acorn::Vertex);
    mirror->IndexFormat = DXGI_FORMAT_R16_UINT;
    mirror->VertexBufferByteSize = mirrorVbByteSize;
    mirror->IndexBufferByteSize = mirrorIbByteSize;

    Acorn::SubMesh mirrorSubMesh;
    mirrorSubMesh.StartVertexLocation = 0;
    mirrorSubMesh.StartIndexLocation = 0;
    mirrorSubMesh.IndexCount = mirrorIndices.size();

    mirror->SubMesh["Mirror"] = std::move(mirrorSubMesh);
    m_pScene->Meshes[mirror->Name] = std::move(mirror);

}


void TextureApp::CreateRenderItem(){

    uint16_t objIndex = 0;

    // Mirror
    auto mirror = std::make_unique<Acorn::RenderItem>();
    mirror->World = DirectX::XMMatrixScaling(10.0f, 10.0f, 10.0f);
    mirror->Mesh = m_pScene->Meshes["MirrorGeo"].get();
    mirror->Mat = m_pScene->Materials["mirror"].get();
    mirror->ObjCBIndex = objIndex++;
    mirror->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    mirror->IndexCount = mirror->Mesh->SubMesh["Mirror"].IndexCount;
    mirror->StartVertexLocation = mirror->Mesh->SubMesh["Mirror"].StartVertexLocation;
    mirror->StartIndexLocation = mirror->Mesh->SubMesh["Mirror"].StartIndexLocation;
    mirror->DirtyCount = g_GraphicsConfig.FrameResourceCount;

    // Shadow of mirror
    auto Shadow = std::make_unique<Acorn::RenderItem>();
	DirectX::XMVECTOR shadowPlane = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // xz plane
	DirectX::XMVECTOR toMainLight = DirectX::XMVectorSet(0.5f, 0.5f, 0.5f, 0.0f);
	DirectX::XMMATRIX S = DirectX::XMMatrixShadow(shadowPlane, toMainLight);
	DirectX::XMMATRIX shadowOffsetY = DirectX::XMMatrixTranslation(0.0f, 0.001f, 0.0f);

    *Shadow = *mirror;
    Shadow->World *= S * shadowOffsetY;
    Shadow->Mat = m_pScene->Materials["shadow"].get();
    Shadow->ObjCBIndex = objIndex++;

    m_pScene->RenderLayers[static_cast<uint16_t>(Acorn::RenderLayer::Mirrors)].push_back(std::move(mirror.get()));
    m_pScene->RenderLayers[static_cast<uint16_t>(Acorn::RenderLayer::Shadow)].push_back(std::move(Shadow.get()));
    m_pScene->AllRenderItems.push_back(std::move(mirror));
    m_pScene->AllRenderItems.push_back(std::move(Shadow));

    Acorn::Matrix4f reflectedMatrix = DirectX::XMMatrixReflect(
         DirectX::XMLoadFloat4(&Acorn::Vector4f(0.0f, 0.0f, -1.0f, 0.0f))
    );

    // Land
    auto land = std::make_unique<Acorn::RenderItem>();
    land->World = DirectX::XMMatrixScaling(4.0f, 4.0f, 4.0f);
    land->Mesh = m_pScene->Meshes["LandGeo"].get();
    land->Mat = m_pScene->Materials["grass"].get();
    land->ObjCBIndex = objIndex++;
    land->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    land->IndexCount = land->Mesh->SubMesh["Land"].IndexCount;
    land->StartVertexLocation = land->Mesh->SubMesh["Land"].StartVertexLocation;
    land->StartIndexLocation = land->Mesh->SubMesh["Land"].StartIndexLocation;
    land->DirtyCount = g_GraphicsConfig.FrameResourceCount;
 
    // Reflected land
    auto reflectedLand = std::make_unique<Acorn::RenderItem>();
    *reflectedLand = *land;
    reflectedLand->World *= reflectedMatrix;
    reflectedLand->ObjCBIndex = objIndex++;

    m_pScene->RenderLayers[static_cast<uint16_t>(Acorn::RenderLayer::Opaque)].push_back(std::move(land.get()));
    m_pScene->RenderLayers[static_cast<uint16_t>(Acorn::RenderLayer::Reflected)].push_back(std::move(reflectedLand.get()));
    m_pScene->AllRenderItems.push_back(std::move(land));
    m_pScene->AllRenderItems.push_back(std::move(reflectedLand));

    // Box
    auto box = std::make_unique<Acorn::RenderItem>();
    box->World = DirectX::XMMatrixScaling(15.0f, 15.0f, 15.0f)
        * DirectX::XMMatrixTranslation(0.0f, 5.0f,20.0f);
    box->Mesh = m_pScene->Meshes["BoxGeo"].get();
    box->Mat = m_pScene->Materials["crate"].get();
    box->ObjCBIndex = objIndex++;
    box->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    box->IndexCount = box->Mesh->SubMesh["Box"].IndexCount;
    box->StartVertexLocation = box->Mesh->SubMesh["Box"].StartVertexLocation;
    box->StartIndexLocation = box->Mesh->SubMesh["Box"].StartIndexLocation;
    box->DirtyCount = g_GraphicsConfig.FrameResourceCount;

    // Reflected box
    auto reflectedBox = std::make_unique<Acorn::RenderItem>();
    *reflectedBox = *box;
    reflectedBox->World *= reflectedMatrix;
    reflectedBox->ObjCBIndex = objIndex++;
    
    m_pScene->RenderLayers[static_cast<uint16_t>(Acorn::RenderLayer::Transparent)].push_back(std::move(box.get()));
    m_pScene->RenderLayers[static_cast<uint16_t>(Acorn::RenderLayer::Reflected)].push_back(std::move(reflectedBox.get()));
    m_pScene->AllRenderItems.push_back(std::move(box));
    m_pScene->AllRenderItems.push_back(std::move(reflectedBox));
    
    // Wave
    auto wave = std::make_unique<Acorn::RenderItem>();
    wave->World = DirectX::XMMatrixScaling(4.0f, 4.0f, 4.0f);
    wave->Mesh = m_pScene->DynamicMeshes["WaveGeo"].get();
    wave->Mat = m_pScene->Materials["water"].get();
    wave->ObjCBIndex = objIndex++;
    wave->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    wave->IndexCount = wave->Mesh->SubMesh["Wave"].IndexCount;
    wave->StartVertexLocation = wave->Mesh->SubMesh["Wave"].StartVertexLocation;
    wave->StartIndexLocation = wave->Mesh->SubMesh["Wave"].StartIndexLocation;
    wave->DirtyCount = g_GraphicsConfig.FrameResourceCount;

    m_pScene->RenderLayers[static_cast<uint16_t>(Acorn::RenderLayer::Transparent)].push_back(std::move(wave.get()));
    m_pScene->AllRenderItems.push_back(std::move(wave));

}

void TextureApp::UpdateInput(){
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

void TextureApp::UpdateScene(){
    const uint32_t waveVbByteSize = m_pWaves->VertexCount() * sizeof(Acorn::Vertex);
    const auto& vertexBuffer = m_pScene->DynamicMeshes["WaveGeo"]->VertexBufferCPU->GetBufferPointer();
    std::vector<Acorn::Vertex> vertices(m_pWaves->VertexCount());

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
		vertices[i].TexC.x = 0.5f + vertices[i].Position.x / m_pWaves->Width();
		vertices[i].TexC.y = 0.5f - vertices[i].Position.z / m_pWaves->Depth();
	}

    CopyMemory(vertexBuffer, vertices.data(), waveVbByteSize);

    AnimateMaterial();

}

void TextureApp::AnimateMaterial(){
    auto waterMat = m_pScene->Materials["water"].get();

    float& tu = waterMat->MatTransform(0, 3);
    float& tv = waterMat->MatTransform(1, 3);

    tu += 0.01f * m_Timer.DeltaTime();
    tv += 0.02f * m_Timer.DeltaTime();

    if(tu > 1.0f) tu -= 1.0f;
    if(tv > 1.0f) tv -= 1.0f;

    waterMat->NumFramesDirty = g_GraphicsConfig.FrameResourceCount;
}

LRESULT TextureApp::MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
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