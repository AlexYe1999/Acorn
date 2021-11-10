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
        Acorn::Vector3f(-98.0f, 40.0f, 100.0f),
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

    auto iceTex = std::make_unique<Acorn::Texture>();
    iceTex->FileName = L"E:/Code/Acorn/samples/TextureMapping/texture/ice.dds";
    iceTex->Name = "IceTexture";

    auto treeTex = std::make_unique<Acorn::Texture>();
    treeTex->FileName = L"E:/Code/Acorn/samples/TextureMapping/texture/treeArray2.dds";
    treeTex->Name = "TreeTexture";

    auto cubeTex = std::make_unique<Acorn::Texture>();
    cubeTex->FileName = L"E:/Code/Acorn/samples/TextureMapping/texture/cube1.dds";
    cubeTex->Name = "CubeMapTexture";

    auto rtTex = std::make_unique<Acorn::Texture>();
    rtTex->FileName = L"";
    rtTex->Name = "CubeMapRefTexture";
    m_pScene->Textures[grassTex->Name] = std::move(grassTex);
    m_pScene->Textures[waveTex->Name] = std::move(waveTex);
    m_pScene->Textures[crateTex->Name] = std::move(crateTex);
    m_pScene->Textures[iceTex->Name] = std::move(iceTex);
    m_pScene->Textures[treeTex->Name] = std::move(treeTex);
    m_pScene->Textures[cubeTex->Name] = std::move(cubeTex);
    m_pScene->Textures[rtTex->Name] = std::move(rtTex);
}

void TextureApp::CreateMaterial(){

    auto grass = std::make_unique<Acorn::Material>("grass");
    grass->MatTransform =  DirectX::XMMatrixScaling(5.0f, 5.0f, 1.0f);
    grass->MatCBIndex = 0;
    grass->DiffuseSrvHeapIndex = 0;
    grass->DiffuseAlbedo = Acorn::Vector4f(0.6f, 0.6f, 0.6f, 1.0f);
    grass->FresnelR0 = Acorn::Vector3f(0.01f, 0.01f, 0.01f);
    grass->Roughness = 0.125f;
    grass->NumFramesDirty = g_GraphicsConfig.FrameResourceCount;

    auto water = std::make_unique<Acorn::Material>("water");
    water->MatCBIndex = 1;
    water->DiffuseSrvHeapIndex = 1;
    water->DiffuseAlbedo = Acorn::Vector4f(0.0f, 0.2f, 0.6f, 0.5f);
    water->FresnelR0 = Acorn::Vector3f(0.1f, 0.1f, 0.1f);
    water->Roughness = 0.0f;
    water->NumFramesDirty = g_GraphicsConfig.FrameResourceCount;

    auto crate = std::make_unique<Acorn::Material>("crate");
	crate->MatCBIndex = 2;
	crate->DiffuseSrvHeapIndex = 2;
	crate->DiffuseAlbedo = Acorn::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
	crate->FresnelR0 = Acorn::Vector3f(0.05f, 0.05f, 0.05f);
	crate->Roughness = 0.2f;
    crate->NumFramesDirty = g_GraphicsConfig.FrameResourceCount;

    auto ice = std::make_unique<Acorn::Material>("ice");
	ice->MatCBIndex = 3;
	ice->DiffuseSrvHeapIndex = 3;
	ice->DiffuseAlbedo = Acorn::Vector4f(0.5f, 0.5f, 0.5f, 1.0f);
	ice->FresnelR0 = Acorn::Vector3f(0.6f, 0.6f, 0.6f);
	ice->Roughness = 0.2f;
    ice->NumFramesDirty = g_GraphicsConfig.FrameResourceCount;

    auto tree = std::make_unique<Acorn::Material>("tree");
	tree->MatCBIndex = 4;
	tree->DiffuseSrvHeapIndex = 4;
	tree->DiffuseAlbedo = Acorn::Vector4f(0.2f, 0.6f, 0.2f, 1.0f);
	tree->FresnelR0 = Acorn::Vector3f(0.01f, 0.01f, 0.01f);
	tree->Roughness = 0.2f;
    tree->NumFramesDirty = g_GraphicsConfig.FrameResourceCount;

    auto sky = std::make_unique<Acorn::Material>("sky");
	sky->MatCBIndex = 5;
	sky->DiffuseSrvHeapIndex = 5;
	sky->DiffuseAlbedo = Acorn::Vector4f(0.8f, 0.8f, 0.8f, 1.0f);
	sky->FresnelR0 = Acorn::Vector3f(0.01f, 0.01f, 0.01f);
	sky->Roughness = 0.5f;
    sky->NumFramesDirty = g_GraphicsConfig.FrameResourceCount;

    auto ref = std::make_unique<Acorn::Material>("ref");
	ref->MatCBIndex = 6;
	ref->DiffuseSrvHeapIndex = 6;
	ref->DiffuseAlbedo = Acorn::Vector4f(0.8f, 0.8f, 0.8f, 1.0f);
	ref->FresnelR0 = Acorn::Vector3f(0.01f, 0.01f, 0.01f);
	ref->Roughness = 0.5f;
    ref->NumFramesDirty = g_GraphicsConfig.FrameResourceCount;

    m_pScene->Materials[grass->Name] = std::move(grass);
    m_pScene->Materials[water->Name] = std::move(water);
    m_pScene->Materials[crate->Name] = std::move(crate);
    m_pScene->Materials[ice->Name]   = std::move(ice);
    m_pScene->Materials[tree->Name]  = std::move(tree);
    m_pScene->Materials[sky->Name]   = std::move(sky);
    m_pScene->Materials[ref->Name]   = std::move(ref);
}

void TextureApp::CreateMesh(){
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

    box->SubMesh["Box"] = std::move(boxSubMesh);
    m_pScene->Meshes[box->Name] = std::move(box);

    // Mesh of sky box and sphere mirror
    GeoGenerator::MeshData skyBoxGeo = geoGen.CreateSphere(1.0f, 20.0f, 20.0f);
    GeoGenerator::MeshData sphereMirrorGeo = geoGen.CreateSphere(20.0f, 40.0f, 40.0f);

    std::unique_ptr<Acorn::Mesh> shpere = std::make_unique<Acorn::Mesh>("SphereGeo");
    std::vector<Acorn::Vertex> sphereVertices;
    std::vector<uint16_t> sphereIndices;

    Acorn::SubMesh skySubMesh;
    skySubMesh.StartVertexLocation = 0;
    skySubMesh.StartIndexLocation = 0;
    skySubMesh.IndexCount = skyBoxGeo.indices.size();
    shpere->SubMesh["Sky"] = std::move(skySubMesh);

    for(const auto& vertex : skyBoxGeo.vertices){
        sphereVertices.push_back(Acorn::Vertex(
            vertex.position,
            vertex.normal,
            vertex.uv
        ));
    }

    sphereIndices.insert(
        sphereIndices.end(), 
        skyBoxGeo.indices.begin(), skyBoxGeo.indices.end()
    );

    Acorn::SubMesh sphereMirrorSubMesh;
    sphereMirrorSubMesh.StartVertexLocation = sphereVertices.size();
    sphereMirrorSubMesh.StartIndexLocation = sphereIndices.size();
    sphereMirrorSubMesh.IndexCount = sphereMirrorGeo.indices.size();
    shpere->SubMesh["SphereMirror"] = std::move(sphereMirrorSubMesh);

    for(const auto& vertex : sphereMirrorGeo.vertices){
        sphereVertices.push_back(Acorn::Vertex(
            vertex.position,
            vertex.normal,
            vertex.uv
        ));
    }

    sphereIndices.insert(
        sphereIndices.end(), 
        sphereMirrorGeo.indices.begin(), sphereMirrorGeo.indices.end()
    );

    const uint32_t sphereVbByteSize = sphereVertices.size() * sizeof(Acorn::Vertex);
    const uint32_t sphereIbByteSize = sphereIndices.size() * sizeof(uint16_t);

    D3DCreateBlob(sphereVbByteSize, shpere->VertexBufferCPU.GetAddressOf());
    D3DCreateBlob(sphereIbByteSize, shpere->IndexBufferCPU.GetAddressOf());

    CopyMemory(shpere->VertexBufferCPU->GetBufferPointer(), sphereVertices.data(), sphereVbByteSize);
    CopyMemory(shpere->IndexBufferCPU->GetBufferPointer(), sphereIndices.data(), sphereIbByteSize);

    shpere->VertexByteStride = sizeof(Acorn::Vertex);
    shpere->IndexFormat = DXGI_FORMAT_R16_UINT;
    shpere->VertexBufferByteSize = sphereVbByteSize;
    shpere->IndexBufferByteSize = sphereIbByteSize;

    m_pScene->Meshes[shpere->Name] = std::move(shpere);

    // Mesh of land
    std::vector<Acorn::Vertex> landVertices = {
        Acorn::Vertex(-50.0f, 0.0f, +50.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
        Acorn::Vertex(+50.0f, 0.0f, +50.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),
        Acorn::Vertex(-50.0f, 0.0f, -50.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
        Acorn::Vertex(+50.0f, 0.0f, -50.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f)
    };

    std::vector<uint16_t> landIndices = {
        0, 1, 2, 3
    };

    std::unique_ptr<Acorn::Mesh> land = std::make_unique<Acorn::Mesh>("LandGeo");

    const uint32_t landVbByteSize = landVertices.size() * sizeof(Acorn::Vertex);
    const uint32_t landIbByteSize = landIndices.size() * sizeof(uint16_t);

    D3DCreateBlob(landVbByteSize, land->VertexBufferCPU.GetAddressOf());
    D3DCreateBlob(landIbByteSize, land->IndexBufferCPU.GetAddressOf());

    CopyMemory(land->VertexBufferCPU->GetBufferPointer(), landVertices.data(), landVbByteSize);
    CopyMemory(land->IndexBufferCPU->GetBufferPointer(), landIndices.data(), landIbByteSize);

    land->VertexByteStride = sizeof(Acorn::Vertex);
    land->IndexFormat = DXGI_FORMAT_R16_UINT;
    land->VertexBufferByteSize = landVbByteSize;
    land->IndexBufferByteSize = landIbByteSize;

    Acorn::SubMesh landSubMesh;
    landSubMesh.StartVertexLocation = 0;
    landSubMesh.StartIndexLocation = 0;
    landSubMesh.IndexCount = landIndices.size();

    land->SubMesh["Land"] = std::move(landSubMesh);
    m_pScene->Meshes[land->Name] = std::move(land);

    // Mesh of Wave
    m_pWaves = std::make_unique<Waves>(100.0f, 100.0f, 1.0f, 0.03f, 4.0f, 0.2f);
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

    const uint32_t waveVbByteSize = m_pWaves->VertexCount() * sizeof(Acorn::Vertex);
    const uint32_t waveIbByteSize = indices.size() * sizeof(uint16_t);

    auto wave = std::make_unique<Acorn::Mesh>("WaveGeo");

    D3DCreateBlob(waveVbByteSize, wave->VertexBufferCPU.GetAddressOf());
    D3DCreateBlob(waveIbByteSize, wave->IndexBufferCPU.GetAddressOf());

    CopyMemory(wave->IndexBufferCPU->GetBufferPointer(), indices.data(), waveIbByteSize);

    wave->VertexByteStride = sizeof(Acorn::Vertex);
    wave->IndexFormat = DXGI_FORMAT_R16_UINT;
    wave->VertexBufferByteSize = waveVbByteSize;
    wave->IndexBufferByteSize = waveIbByteSize;

    Acorn::SubMesh waveSubMesh;
    waveSubMesh.StartVertexLocation = 0;
    waveSubMesh.StartIndexLocation = 0;
    waveSubMesh.IndexCount = indices.size();

    wave->SubMesh["Wave"] = std::move(waveSubMesh);
    m_pScene->DynamicMeshes[wave->Name] = std::move(wave);

    // Mesh of tree
    std::unique_ptr<Acorn::Mesh> tree = std::make_unique<Acorn::Mesh>("TreeGeo");
    std::vector<Acorn::TreeSpriteVertex> treeVertices = {
        Acorn::TreeSpriteVertex(20.0f, 28.0f, 20.0f, 20.0f, 50.0f),
        Acorn::TreeSpriteVertex(-20.0f, 20.0f, -20.0f, 20.0f, 50.0f),
        Acorn::TreeSpriteVertex(-50.0f, 30.0f, -50.0f, 20.0f, 50.0f),
        Acorn::TreeSpriteVertex(-60.0f, 25.0f, -68.0f, 20.0f, 20.0f),
        Acorn::TreeSpriteVertex(-80.0f, 50.0f, -90.0f, 35.0f, 30.0f)
    };
    std::vector<uint16_t> treeIndices = {0, 1, 2, 3, 4};

    const uint32_t treeVbByteSize = treeVertices.size() * sizeof(Acorn::TreeSpriteVertex);
    const uint32_t treeIbByteSize = treeIndices.size() * sizeof(uint16_t);

    D3DCreateBlob(treeVbByteSize, tree->VertexBufferCPU.GetAddressOf());
    D3DCreateBlob(treeIbByteSize, tree->IndexBufferCPU.GetAddressOf());

    CopyMemory(tree->VertexBufferCPU->GetBufferPointer(), treeVertices.data(), treeVbByteSize);
    CopyMemory(tree->IndexBufferCPU->GetBufferPointer(), treeIndices.data(), treeIbByteSize);

    tree->VertexByteStride = sizeof(Acorn::TreeSpriteVertex);
    tree->IndexFormat = DXGI_FORMAT_R16_UINT;
    tree->VertexBufferByteSize = treeVbByteSize;
    tree->IndexBufferByteSize = treeIbByteSize;

    Acorn::SubMesh treeSubMesh;
    treeSubMesh.StartVertexLocation = 0;
    treeSubMesh.StartIndexLocation = 0;
    treeSubMesh.IndexCount = treeIndices.size();

    tree->SubMesh["Tree"] = std::move(treeSubMesh);
    m_pScene->Meshes[tree->Name] = std::move(tree);

}


void TextureApp::CreateRenderItem(){

    uint16_t objIndex = 0;

    auto land = std::make_unique<Acorn::RenderItem>();
    land->World = DirectX::XMMatrixScaling(4.0f, 3.0f, 4.0f);
    land->Mesh = m_pScene->Meshes["LandGeo"].get();
    land->Mat = m_pScene->Materials["grass"].get();
    land->ObjCBIndex = objIndex++;
    land->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;
    land->IndexCount = land->Mesh->SubMesh["Land"].IndexCount;
    land->StartVertexLocation = land->Mesh->SubMesh["Land"].StartVertexLocation;
    land->StartIndexLocation = land->Mesh->SubMesh["Land"].StartIndexLocation;
    land->DirtyCount = g_GraphicsConfig.FrameResourceCount;
    m_pScene->RenderLayers[static_cast<uint16_t>(Acorn::RenderLayer::Opaque)].push_back(std::move(land.get()));
    m_pScene->AllRenderItems.push_back(std::move(land));

    auto sky = std::make_unique<Acorn::RenderItem>();
    sky->Mesh = m_pScene->Meshes["SphereGeo"].get();
    sky->Mat = m_pScene->Materials["sky"].get();
    sky->ObjCBIndex = objIndex++;
    sky->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    sky->IndexCount = sky->Mesh->SubMesh["Sky"].IndexCount;
    sky->StartVertexLocation = sky->Mesh->SubMesh["Sky"].StartVertexLocation;
    sky->StartIndexLocation = sky->Mesh->SubMesh["Sky"].StartIndexLocation;
    sky->DirtyCount = g_GraphicsConfig.FrameResourceCount;
    m_pScene->RenderLayers[static_cast<uint16_t>(Acorn::RenderLayer::Background)].push_back(std::move(sky.get()));
    m_pScene->AllRenderItems.push_back(std::move(sky));

    auto sphereMirror = std::make_unique<Acorn::RenderItem>();
    sphereMirror->World = DirectX::XMMatrixTranslation(0.0f, 50.0f, 0.0f);
    sphereMirror->Mesh = m_pScene->Meshes["SphereGeo"].get();
    sphereMirror->Mat = m_pScene->Materials["ref"].get();
    sphereMirror->ObjCBIndex = objIndex++;
    sphereMirror->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    sphereMirror->IndexCount = sphereMirror->Mesh->SubMesh["SphereMirror"].IndexCount;
    sphereMirror->StartVertexLocation = sphereMirror->Mesh->SubMesh["SphereMirror"].StartVertexLocation;
    sphereMirror->StartIndexLocation = sphereMirror->Mesh->SubMesh["SphereMirror"].StartIndexLocation;
    sphereMirror->DirtyCount = g_GraphicsConfig.FrameResourceCount;
    m_pScene->RenderLayers[static_cast<uint16_t>(Acorn::RenderLayer::Mirrors)].push_back(std::move(sphereMirror.get()));
    m_pScene->AllRenderItems.push_back(std::move(sphereMirror));

    auto box = std::make_unique<Acorn::RenderItem>();
    box->World = DirectX::XMMatrixScaling(15.0f, 15.0f, 15.0f);
    box->Mesh = m_pScene->Meshes["BoxGeo"].get();
    box->Mat = m_pScene->Materials["crate"].get();
    box->ObjCBIndex = objIndex++;
    box->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    box->IndexCount = box->Mesh->SubMesh["Box"].IndexCount;
    box->StartVertexLocation = box->Mesh->SubMesh["Box"].StartVertexLocation;
    box->StartIndexLocation = box->Mesh->SubMesh["Box"].StartIndexLocation;
    box->DirtyCount = g_GraphicsConfig.FrameResourceCount;
    m_pScene->RenderLayers[static_cast<uint16_t>(Acorn::RenderLayer::Transparent)].push_back(std::move(box.get()));
    m_pScene->AllRenderItems.push_back(std::move(box));

    auto wave = std::make_unique<Acorn::RenderItem>();
    wave->World = DirectX::XMMatrixScaling(4.0f, 3.0f, 4.0f);
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

    auto tree = std::make_unique<Acorn::RenderItem>();
    tree->World = DirectX::XMMatrixScaling(5.0f, 5.0f, 5.0f);
    tree->Mesh = m_pScene->Meshes["TreeGeo"].get();
    tree->Mat = m_pScene->Materials["tree"].get();
    tree->ObjCBIndex = objIndex++;
    tree->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
    tree->IndexCount = tree->Mesh->SubMesh["Tree"].IndexCount;
    tree->StartVertexLocation = tree->Mesh->SubMesh["Tree"].StartVertexLocation;
    tree->StartIndexLocation = tree->Mesh->SubMesh["Tree"].StartIndexLocation;
    tree->DirtyCount = g_GraphicsConfig.FrameResourceCount;
    m_pScene->RenderLayers[static_cast<uint16_t>(Acorn::RenderLayer::Sprite)].push_back(std::move(tree.get()));
    m_pScene->AllRenderItems.push_back(std::move(tree));

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