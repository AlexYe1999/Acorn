#include"D3D12GraphicsManager.hpp"

extern Acorn::GraphicsParam g_GraphicsConfig;

namespace Acorn{

    D3D12GraphicsManager::D3D12GraphicsManager()
        :
        m_VertexBufferView(),
        m_IndexBufferView(),
        m_pRtBuffer(),
        m_pFrameResource(),
        m_uCurrentFence(0),
        m_uCurrentBackBufferIndex(0),
        m_uCurrFrameResourceIndex(0),
        m_uRtvDescriptorSize(0),
        m_uDsvDescriptorSize(0),
        m_uCbvSrvUavDescriptorSize(0),
        m_pScene(nullptr),
        m_pTimer(nullptr)
    {}

    void D3D12GraphicsManager::ResetRtAndDs(){

        FlushCommandQueue();
        
        m_pD3D12GraphicsCommandList->Reset(m_pD3D12CommandAllocator.Get(), nullptr);

        for(int i = 0; i < g_GraphicsConfig.BackBufferCount; ++i)
            m_pRtBuffer[i].Reset();
        m_pDsBuffer.Reset();

        m_pDXGISwapChain->ResizeBuffers(
            g_GraphicsConfig.BackBufferCount,
            g_GraphicsConfig.WndWidth, g_GraphicsConfig.WndHeight,
            g_GraphicsConfig.BackBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
        );

        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
        rtvHeapDesc.Type  = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        rtvHeapDesc.NumDescriptors = g_GraphicsConfig.BackBufferCount;
        rtvHeapDesc.NodeMask = 0;

        m_pD3D12Device->CreateDescriptorHeap(
            &rtvHeapDesc, IID_PPV_ARGS(m_pRtvHeap.GetAddressOf())
        );

        //TODO: Use MemoryManager to alloc Memory
        m_pRtBuffer.reset(new ComPtr<ID3D12Resource>[g_GraphicsConfig.BackBufferCount]);

        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(
            m_pRtvHeap->GetCPUDescriptorHandleForHeapStart()
        );

        for(uint8_t index = 0; index < g_GraphicsConfig.BackBufferCount; index++){
            m_pDXGISwapChain->GetBuffer(
                index, IID_PPV_ARGS(m_pRtBuffer[index].GetAddressOf())
            );

            m_pD3D12Device->CreateRenderTargetView(
                m_pRtBuffer[index].Get(), nullptr, rtvHeapHandle
            );

            rtvHeapHandle.Offset(1, m_uRtvDescriptorSize);
        }


        D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
        dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        dsvHeapDesc.NumDescriptors = 1;
        dsvHeapDesc.NodeMask = 0;

        m_pD3D12Device->CreateDescriptorHeap(
            &dsvHeapDesc, IID_PPV_ARGS(m_pDsvHeap.GetAddressOf())
        );

        D3D12_RESOURCE_DESC depthStencilDesc;
        depthStencilDesc.Width  = g_GraphicsConfig.WndWidth;
        depthStencilDesc.Height = g_GraphicsConfig.WndHeight;
        depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        depthStencilDesc.Alignment = 0;
        depthStencilDesc.MipLevels = 1;
        depthStencilDesc.DepthOrArraySize = 1;
        depthStencilDesc.Format = g_GraphicsConfig.DepthStansilFormat;
        depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        depthStencilDesc.Flags  = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
        depthStencilDesc.SampleDesc.Count   = 1;
        depthStencilDesc.SampleDesc.Quality = 0;

        D3D12_CLEAR_VALUE optClear;
        optClear.Format = g_GraphicsConfig.DepthStansilFormat;
        optClear.DepthStencil.Depth   = 1.0f;
        optClear.DepthStencil.Stencil = 0;
        m_pD3D12Device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAG_NONE, &depthStencilDesc, 
            D3D12_RESOURCE_STATE_COMMON, &optClear,
            IID_PPV_ARGS(m_pDsBuffer.GetAddressOf())
        );

        m_pD3D12Device->CreateDepthStencilView(
            m_pDsBuffer.Get(), nullptr, DepthStencilView()
        );

        m_pD3D12GraphicsCommandList->ResourceBarrier(
            1, 
            &CD3DX12_RESOURCE_BARRIER::Transition(
            m_pDsBuffer.Get(),
            D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE
        ));

        m_pD3D12GraphicsCommandList->Close();
        ID3D12CommandList *cmdsLists[] = {m_pD3D12GraphicsCommandList.Get()};
        m_pD3D12CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

        FlushCommandQueue();

        g_GraphicsConfig.ViewPort.TopLeftX = 0;
        g_GraphicsConfig.ViewPort.TopLeftY = 0;
        g_GraphicsConfig.ViewPort.Width = static_cast<float>(g_GraphicsConfig.WndWidth);
        g_GraphicsConfig.ViewPort.Height = static_cast<float>(g_GraphicsConfig.WndHeight);
        g_GraphicsConfig.ViewPort.MinDepth = 0.0f;
        g_GraphicsConfig.ViewPort.MaxDepth = 1.0f;

        g_GraphicsConfig.ScissorRect = {
            0, 0, g_GraphicsConfig.WndWidth, g_GraphicsConfig.WndHeight
        };
    }

    void D3D12GraphicsManager::Initialize(Scene* const scene, Timer* const timer){

#if defined(_DEBUG)
        {
            ComPtr<ID3D12Debug> debugController;
            D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf()));
            debugController->EnableDebugLayer();
        }
#endif

        CreateDXGIFactory1(IID_PPV_ARGS(m_pDXGIFactory.GetAddressOf()));

#if defined(_DEBUG)
        LogAdaptors();
#endif

        m_pScene = scene;
        m_pTimer = timer;
        BuildDeviceAndFence();
        BuildCommandObject();
        BuildSwapChain();
        BuildRtAndDs();
        BuildRootSignature();

        InitializeResources();
        InitializeBuffers();
        InitializeConstants();
        InitializeShaders();

        BuildPSO();
        m_pD3D12GraphicsCommandList->Close();

        ID3D12CommandList *cmdsLists[] = {m_pD3D12GraphicsCommandList.Get()};
        m_pD3D12CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

        FlushCommandQueue();
        m_bIsInit = true;
    }

    void D3D12GraphicsManager::Finalize(){

    }

    void D3D12GraphicsManager::Tick(){

        UpdateFrameResource();
        Render();
    
    }

    void D3D12GraphicsManager::Clear(){

    }

    void D3D12GraphicsManager::Render(){

        auto& currFrameResource = m_pFrameResource[m_uCurrFrameResourceIndex];

        currFrameResource->CmdListAlloc->Reset();
        m_pD3D12GraphicsCommandList->Reset(
            currFrameResource->CmdListAlloc.Get(), m_pPSOs["Default"].Get()
        );

        m_pD3D12GraphicsCommandList->RSSetViewports(1, &g_GraphicsConfig.ViewPort);
        m_pD3D12GraphicsCommandList->RSSetScissorRects(1, &g_GraphicsConfig.ScissorRect);

        m_pD3D12GraphicsCommandList->ResourceBarrier(
            1, &CD3DX12_RESOURCE_BARRIER::Transition(
                m_pRtBuffer[m_uCurrentBackBufferIndex].Get(), 
                D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET)
        );

        m_pD3D12GraphicsCommandList->ClearRenderTargetView(
            CurrentBackBufferView(), (float*)&m_MainPassCB.FogColor, 0, nullptr
        );

        m_pD3D12GraphicsCommandList->ClearDepthStencilView(
            DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
            1.0f, 0, 0, nullptr
        );
        
        m_pD3D12GraphicsCommandList->OMSetRenderTargets(
            1, &CurrentBackBufferView(), true, &DepthStencilView()
        );

        m_pD3D12GraphicsCommandList->SetGraphicsRootSignature(m_pRootSignatures["Shading"].Get());

        ID3D12DescriptorHeap *descriptorHeap[] = {m_pSrvHeap.Get()};
        m_pD3D12GraphicsCommandList->SetDescriptorHeaps(1, descriptorHeap);

        m_pD3D12GraphicsCommandList->SetGraphicsRootDescriptorTable(
            3, m_pSrvHeap->GetGPUDescriptorHandleForHeapStart()
        );

        auto& PassCB = currFrameResource->PassCB;
        m_pD3D12GraphicsCommandList->SetGraphicsRootConstantBufferView(
            2, PassCB->Resource()->GetGPUVirtualAddress()
        );

        auto& matBuffer = currFrameResource->MaterialCB;
        m_pD3D12GraphicsCommandList->SetGraphicsRootShaderResourceView(
            1, matBuffer->Resource()->GetGPUVirtualAddress()
        );

        auto& insBuffer = currFrameResource->ObjectCB;
        m_pD3D12GraphicsCommandList->SetGraphicsRootShaderResourceView(
            0, insBuffer->Resource()->GetGPUVirtualAddress()
        );

        // Draw calls
        m_pD3D12GraphicsCommandList->IASetVertexBuffers(0, 1, &m_pScene->Instances->Mesh->VertexBufferView());
        m_pD3D12GraphicsCommandList->IASetIndexBuffer(&m_pScene->Instances->Mesh->IndexBufferView());
        m_pD3D12GraphicsCommandList->IASetPrimitiveTopology(m_pScene->Instances->PrimitiveType);

        m_pD3D12GraphicsCommandList->DrawIndexedInstanced(
            m_pScene->Instances->IndexCount, m_pScene->Instances->VisibleInstanceCount, 
            m_pScene->Instances->StartIndexLocation, m_pScene->Instances->StartVertexLocation, 0
        );
        // End

        m_pD3D12GraphicsCommandList->ResourceBarrier(
            1, &CD3DX12_RESOURCE_BARRIER::Transition(
                m_pRtBuffer[m_uCurrentBackBufferIndex].Get(), 
                D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT
            )
        );

        m_pD3D12GraphicsCommandList->Close();

        ID3D12CommandList *cmdLists[] = {m_pD3D12GraphicsCommandList.Get()};
        m_pD3D12CommandQueue->ExecuteCommandLists(1, cmdLists);

        m_pDXGISwapChain->Present(0, 0);

        currFrameResource->Fence = ++m_uCurrentFence;

        m_uCurrFrameResourceIndex =
            (m_uCurrFrameResourceIndex + 1) % g_GraphicsConfig.FrameResourceCount;
        m_uCurrentBackBufferIndex =
            (m_uCurrentBackBufferIndex + 1) % g_GraphicsConfig.BackBufferCount;

        m_pD3D12CommandQueue->Signal(m_pD3D12Fence.Get(), m_uCurrentFence);
    }

    void D3D12GraphicsManager::DrawLine(const Vector3f &from, const Vector3f &to, const Vector3f &color){

    }

    void D3D12GraphicsManager::DrawBox(const Vector3f &bbMin, const Vector3f &bbMax, const Vector3f &color){

    }
    
    void D3D12GraphicsManager::ClearDebugBuffers(){

    }

    void D3D12GraphicsManager::InitializeResources(){

        for(auto& p : m_pScene->Textures){
            auto& texture = p.second;
            HRESULT hr = CreateDDSTextureFromFile12(
                m_pD3D12Device.Get(), m_pD3D12GraphicsCommandList.Get(),
                texture->FileName.c_str(), texture->Resource, texture->UploadHeap
            );
        }

        D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
        srvHeapDesc.NumDescriptors = m_pScene->Materials.size();
        srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        srvHeapDesc.NodeMask = 
        m_pD3D12Device->CreateDescriptorHeap(
            &srvHeapDesc, IID_PPV_ARGS(m_pSrvHeap.GetAddressOf())
        );

        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

        auto& textures = m_pScene->Textures;
            
        CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(
            m_pSrvHeap->GetCPUDescriptorHandleForHeapStart()
        );
        srvDesc.Format = textures["Bricks0"]->Resource->GetDesc().Format;
        srvDesc.Texture2D.MipLevels = textures["Bricks0"]->Resource->GetDesc().MipLevels;
        m_pD3D12Device->CreateShaderResourceView(
            textures["Bricks0"]->Resource.Get(), &srvDesc, hDescriptor
        );

        hDescriptor.Offset(1, m_uCbvSrvUavDescriptorSize);
        srvDesc.Format = textures["Bricks1"]->Resource->GetDesc().Format;
        srvDesc.Texture2D.MipLevels = textures["Bricks1"]->Resource->GetDesc().MipLevels;
        m_pD3D12Device->CreateShaderResourceView(
            textures["Bricks1"]->Resource.Get(), &srvDesc, hDescriptor
        );

        hDescriptor.Offset(1, m_uCbvSrvUavDescriptorSize);
        srvDesc.Format = textures["Bricks2"]->Resource->GetDesc().Format;
        srvDesc.Texture2D.MipLevels = textures["Bricks2"]->Resource->GetDesc().MipLevels;
        m_pD3D12Device->CreateShaderResourceView(
            textures["Bricks2"]->Resource.Get(), &srvDesc, hDescriptor
        );

        hDescriptor.Offset(1, m_uCbvSrvUavDescriptorSize);
        srvDesc.Format = textures["Grass"]->Resource->GetDesc().Format;
        srvDesc.Texture2D.MipLevels = textures["Grass"]->Resource->GetDesc().MipLevels;
        m_pD3D12Device->CreateShaderResourceView(
            textures["Grass"]->Resource.Get(), &srvDesc, hDescriptor
        );

        hDescriptor.Offset(1, m_uCbvSrvUavDescriptorSize);
        srvDesc.Format = textures["Ice"]->Resource->GetDesc().Format;
        srvDesc.Texture2D.MipLevels = textures["Ice"]->Resource->GetDesc().MipLevels;
        m_pD3D12Device->CreateShaderResourceView(
            textures["Ice"]->Resource.Get(), &srvDesc, hDescriptor
        );

        hDescriptor.Offset(1, m_uCbvSrvUavDescriptorSize);
        srvDesc.Format = textures["Water"]->Resource->GetDesc().Format;
        srvDesc.Texture2D.MipLevels = textures["Water"]->Resource->GetDesc().MipLevels;
        m_pD3D12Device->CreateShaderResourceView(
            textures["Water"]->Resource.Get(), &srvDesc, hDescriptor
        );

        hDescriptor.Offset(1, m_uCbvSrvUavDescriptorSize);
        srvDesc.Format = textures["Wood1"]->Resource->GetDesc().Format;
        srvDesc.Texture2D.MipLevels = textures["Wood1"]->Resource->GetDesc().MipLevels;
        m_pD3D12Device->CreateShaderResourceView(
            textures["Wood1"]->Resource.Get(), &srvDesc, hDescriptor
        );

        hDescriptor.Offset(1, m_uCbvSrvUavDescriptorSize);
        srvDesc.Format = textures["Wood2"]->Resource->GetDesc().Format;
        srvDesc.Texture2D.MipLevels = textures["Wood2"]->Resource->GetDesc().MipLevels;
        m_pD3D12Device->CreateShaderResourceView(
            textures["Wood2"]->Resource.Get(), &srvDesc, hDescriptor
        );

    }

    void D3D12GraphicsManager::InitializeBuffers(){
        assert(m_pScene != nullptr);

        for(auto& mesh : m_pScene->Meshes){

            mesh.second->VertexBufferGPU =
                D3DUtil::CreateDefaultBuffer(
                    m_pD3D12Device.Get(), m_pD3D12GraphicsCommandList.Get(),
                    mesh.second->VertexBufferCPU->GetBufferPointer(),
                    mesh.second->VertexBufferCPU->GetBufferSize(),
                    mesh.second->VertexBufferUploader
                );

            mesh.second->IndexBufferGPU = 
                D3DUtil::CreateDefaultBuffer(
                    m_pD3D12Device.Get(), m_pD3D12GraphicsCommandList.Get(),
                    mesh.second->IndexBufferCPU->GetBufferPointer(),
                    mesh.second->IndexBufferCPU->GetBufferSize(),
                    mesh.second->IndexBufferUploader
                );
        }

    }

    void D3D12GraphicsManager::InitializeShaders(){
        m_pShaderByteCode["VS"] = D3DUtil::LoadBinaryShader("E:\\Code\\Acorn\\build\\engine\\core\\graphics\\Debug\\VS.cso");
        m_pShaderByteCode["PS"] = D3DUtil::LoadBinaryShader("E:\\Code\\Acorn\\build\\engine\\core\\graphics\\Debug\\PS.cso");
    }
    
    void D3D12GraphicsManager::ClearShaders(){

    }

    void D3D12GraphicsManager::ClearBuffers(){

    }

    void D3D12GraphicsManager::InitializeConstants(){

        const auto& frameResourceCount = g_GraphicsConfig.FrameResourceCount;
        const auto& instances = m_pScene->Instances;

        for(uint32_t index = 0; index < frameResourceCount; index++){
            m_pFrameResource.push_back(std::move(
                std::make_unique<FrameResourceT>(
                    m_pD3D12Device.Get(), 1, instances->InstanceDataArray.size(),
                    m_pScene->Materials.size())
            ));
        }

    }

    void D3D12GraphicsManager::UpdateFrameResource(){

        const auto& currFrameResource = m_pFrameResource[m_uCurrFrameResourceIndex];

        if(currFrameResource->Fence != 0
            && m_pD3D12Fence->GetCompletedValue() < currFrameResource->Fence){

            HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
            m_pD3D12Fence->SetEventOnCompletion(currFrameResource->Fence, eventHandle);
            WaitForSingleObject(eventHandle, INFINITE);
            CloseHandle(eventHandle);

        }

        UpdateConstants();

    }

    void D3D12GraphicsManager::UpdateConstants(){

        UpdateMainPassConstBuffer();
        UpdateObjectConstBuffer();
        UpdateMaterialConstBuffer();

    }

    void D3D12GraphicsManager::RenderBuffers(){

    }

    inline void D3D12GraphicsManager::BuildDeviceAndFence(){
        ComPtr<IDXGIAdapter> adaptor;
        m_pDXGIFactory->EnumAdapters(g_GraphicsConfig.AdapterIndex, adaptor.GetAddressOf());

        D3D12CreateDevice(adaptor.Get(),
            g_GraphicsConfig.FeatureLevel, 
            IID_PPV_ARGS(m_pD3D12Device.GetAddressOf())
        );

        m_uRtvDescriptorSize = 
            m_pD3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        m_uDsvDescriptorSize =
            m_pD3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
        m_uCbvSrvUavDescriptorSize = 
            m_pD3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        m_pD3D12Device->CreateFence(
            0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_pD3D12Fence.GetAddressOf())
        );
    }

    void D3D12GraphicsManager::BuildCommandObject(){
        D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
        commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

        m_pD3D12Device->CreateCommandQueue(
            &commandQueueDesc, IID_PPV_ARGS(m_pD3D12CommandQueue.GetAddressOf())
        );

        m_pD3D12Device->CreateCommandAllocator(
            D3D12_COMMAND_LIST_TYPE_DIRECT, 
            IID_PPV_ARGS(m_pD3D12CommandAllocator.GetAddressOf())
        );

        m_pD3D12Device->CreateCommandList(
            0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pD3D12CommandAllocator.Get(),
            nullptr, IID_PPV_ARGS(m_pD3D12GraphicsCommandList.GetAddressOf())
        );

    }

    void D3D12GraphicsManager::BuildSwapChain(){
        m_pDXGISwapChain.Reset();

        DXGI_SWAP_CHAIN_DESC swapChainDesc;
        swapChainDesc.BufferDesc.Width = g_GraphicsConfig.WndWidth;
        swapChainDesc.BufferDesc.Height = g_GraphicsConfig.WndHeight;
        swapChainDesc.BufferDesc.RefreshRate.Numerator   = 60;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDesc.BufferDesc.Format = g_GraphicsConfig.BackBufferFormat;
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        swapChainDesc.SampleDesc.Count   = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferCount = g_GraphicsConfig.BackBufferCount;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.OutputWindow = g_GraphicsConfig.MainWnd;
        swapChainDesc.Windowed = true;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        m_pDXGIFactory->CreateSwapChain(
            m_pD3D12CommandQueue.Get(), &swapChainDesc, m_pDXGISwapChain.GetAddressOf()
        );

    }

    void D3D12GraphicsManager::BuildRtAndDs(){

        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
        rtvHeapDesc.Type  = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        rtvHeapDesc.NumDescriptors = g_GraphicsConfig.BackBufferCount;
        rtvHeapDesc.NodeMask = 0;

        m_pD3D12Device->CreateDescriptorHeap(
            &rtvHeapDesc, IID_PPV_ARGS(m_pRtvHeap.GetAddressOf())
        );

        //TODO: Use MemoryManager to alloc Memory
        m_pRtBuffer = 
            std::make_unique<ComPtr<ID3D12Resource>[]>(g_GraphicsConfig.BackBufferCount);

        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(
            m_pRtvHeap->GetCPUDescriptorHandleForHeapStart()
        );

        for(uint8_t index = 0; index < g_GraphicsConfig.BackBufferCount; index++){
            m_pDXGISwapChain->GetBuffer(
                index, IID_PPV_ARGS(m_pRtBuffer[index].GetAddressOf())
            );

            m_pD3D12Device->CreateRenderTargetView(
                m_pRtBuffer[index].Get(), nullptr, rtvHeapHandle
            );

            rtvHeapHandle.Offset(1, m_uRtvDescriptorSize);
        }


        D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
        dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        dsvHeapDesc.NumDescriptors = 1;
        dsvHeapDesc.NodeMask = 0;

        m_pD3D12Device->CreateDescriptorHeap(
            &dsvHeapDesc, IID_PPV_ARGS(m_pDsvHeap.GetAddressOf())
        );

        D3D12_RESOURCE_DESC depthStencilDesc;
        depthStencilDesc.Width  = g_GraphicsConfig.WndWidth;
        depthStencilDesc.Height = g_GraphicsConfig.WndHeight;
        depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        depthStencilDesc.Alignment = 0;
        depthStencilDesc.MipLevels = 1;
        depthStencilDesc.DepthOrArraySize = 1;
        depthStencilDesc.Format = g_GraphicsConfig.DepthStansilFormat;
        depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        depthStencilDesc.Flags  = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
        depthStencilDesc.SampleDesc.Count   = 1;
        depthStencilDesc.SampleDesc.Quality = 0;

        D3D12_CLEAR_VALUE optClear;
        optClear.Format = g_GraphicsConfig.DepthStansilFormat;
        optClear.DepthStencil.Depth   = 1.0f;
        optClear.DepthStencil.Stencil = 0;
        m_pD3D12Device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAG_NONE, &depthStencilDesc, 
            D3D12_RESOURCE_STATE_COMMON, &optClear,
            IID_PPV_ARGS(m_pDsBuffer.GetAddressOf())
        );

        m_pD3D12Device->CreateDepthStencilView(
            m_pDsBuffer.Get(), nullptr, DepthStencilView()
        );

        m_pD3D12GraphicsCommandList->ResourceBarrier(
            1, 
            &CD3DX12_RESOURCE_BARRIER::Transition(
            m_pDsBuffer.Get(),
            D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE
        ));

    }

    void D3D12GraphicsManager::BuildRootSignature(){

        //default shading 
        CD3DX12_ROOT_PARAMETER slotRootParam[4];
        CD3DX12_DESCRIPTOR_RANGE srvTable;
        srvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 8, 0);

        slotRootParam[0].InitAsShaderResourceView(0, 1);
        slotRootParam[1].InitAsShaderResourceView(1, 1);
        slotRootParam[2].InitAsConstantBufferView(0);
        slotRootParam[3].InitAsDescriptorTable(1, &srvTable, D3D12_SHADER_VISIBILITY_PIXEL);

        StaticSamplerArray staticSampler = GetStaticSamplers();

        CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(
            4, slotRootParam, GetStaticSamplers().size(), GetStaticSamplers().data(),
            D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
        );

        ComPtr<ID3DBlob> serializedRootSig = nullptr;
        ComPtr<ID3DBlob> errorBlob = nullptr;
        D3D12SerializeRootSignature(
            &rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1,
            serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf()
        );

        m_pD3D12Device->CreateRootSignature( 
            0, 
            serializedRootSig->GetBufferPointer(),
            serializedRootSig->GetBufferSize(),
            IID_PPV_ARGS(m_pRootSignatures["Shading"].GetAddressOf())
        );

    }

    void D3D12GraphicsManager::BuildPSO(){

        // Raster state
        D3D12_RASTERIZER_DESC rasterDesc;
        rasterDesc.FillMode = D3D12_FILL_MODE_SOLID;
        rasterDesc.CullMode = D3D12_CULL_MODE_BACK;
        rasterDesc.FrontCounterClockwise = false;
        rasterDesc.DepthBias = 0;
        rasterDesc.DepthBiasClamp = 0.0f;
        rasterDesc.SlopeScaledDepthBias = 0.0f;
        rasterDesc.DepthClipEnable = true;
        rasterDesc.MultisampleEnable = false;
        rasterDesc.AntialiasedLineEnable = false;
        rasterDesc.ForcedSampleCount = 0;
        rasterDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

        D3D12_GRAPHICS_PIPELINE_STATE_DESC piplineStateDesc;
        ZeroMemory(&piplineStateDesc, sizeof(piplineStateDesc));
        piplineStateDesc.InputLayout = {
            Vertex::Desc.data(),
            Vertex::Desc.size()
        };
        piplineStateDesc.StreamOutput = {};
        piplineStateDesc.pRootSignature = m_pRootSignatures["Shading"].Get();
        piplineStateDesc.VS = {
            m_pShaderByteCode["VS"]->GetBufferPointer(),
            m_pShaderByteCode["VS"]->GetBufferSize()
        };
        piplineStateDesc.PS = {
            m_pShaderByteCode["PS"]->GetBufferPointer(),
            m_pShaderByteCode["PS"]->GetBufferSize()
        };
        piplineStateDesc.GS = {};
        piplineStateDesc.RasterizerState = rasterDesc;
        piplineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        piplineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        piplineStateDesc.SampleMask = UINT_MAX;
        piplineStateDesc.IBStripCutValue = {};
        piplineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        piplineStateDesc.NumRenderTargets = 1;
        piplineStateDesc.RTVFormats[0] = g_GraphicsConfig.BackBufferFormat;
        piplineStateDesc.DSVFormat     = g_GraphicsConfig.DepthStansilFormat;
        piplineStateDesc.SampleDesc.Count   = 1;
        piplineStateDesc.SampleDesc.Quality = 0;
        piplineStateDesc.NodeMask = {};
        piplineStateDesc.CachedPSO = {};
        piplineStateDesc.Flags = {};

        //PSO for default object
        m_pD3D12Device->CreateGraphicsPipelineState(
            &piplineStateDesc, IID_PPV_ARGS(m_pPSOs["Default"].GetAddressOf())
        );

    }

    StaticSamplerArray D3D12GraphicsManager::GetStaticSamplers(){
        const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
            0, // shaderRegister
            D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
            D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
            D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
            D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

        const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
            1, // shaderRegister
            D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
            D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
            D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
            D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

        const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
            2, // shaderRegister
            D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
            D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
            D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
            D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

        const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
            3, // shaderRegister
            D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
            D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
            D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
            D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

        const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
            4, // shaderRegister
            D3D12_FILTER_ANISOTROPIC, // filter
            D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
            D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
            D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressW
            0.0f,                             // mipLODBias
            8);                               // maxAnisotropy

        const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
            5, // shaderRegister
            D3D12_FILTER_ANISOTROPIC, // filter
            D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
            D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
            D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressW
            0.0f,                              // mipLODBias
            8);                                // maxAnisotropy

        return {
            pointWrap, pointClamp,
            linearWrap, linearClamp,
            anisotropicWrap, anisotropicClamp};
    }
    
    void D3D12GraphicsManager::DrawRenderLayer(Scene::RenderItems renderItems){

        constexpr uint16_t objCBByteSize =
            D3DUtil::CalcAlignment(sizeof(ObjectConstant));
        constexpr uint16_t matCBByteSize = 
            D3DUtil::CalcAlignment(sizeof(MaterialConstant));

        auto& frameResource = m_pFrameResource[m_uCurrFrameResourceIndex];
        auto objCBAddr = frameResource->ObjectCB->Resource()->GetGPUVirtualAddress();
        auto matCBAddr = frameResource->MaterialCB->Resource()->GetGPUVirtualAddress();

        for(const auto& item : renderItems){
            // Primitive
            m_pD3D12GraphicsCommandList->IASetVertexBuffers(0, 1, &item->Mesh->VertexBufferView());
            m_pD3D12GraphicsCommandList->IASetIndexBuffer(&item->Mesh->IndexBufferView());
            m_pD3D12GraphicsCommandList->IASetPrimitiveTopology(item->PrimitiveType);

            // Set CBV
            m_pD3D12GraphicsCommandList->SetGraphicsRootConstantBufferView(
                1, objCBAddr + item->ObjCBIndex * objCBByteSize
            );
            m_pD3D12GraphicsCommandList->SetGraphicsRootConstantBufferView(
                2, matCBAddr + item->Mat->MatCBIndex * matCBByteSize
            );
            
            // Set SRV
            auto srvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(
                m_pSrvHeap->GetGPUDescriptorHandleForHeapStart()
            );
            srvHandle.Offset(item->Mat->DiffuseSrvHeapIndex, m_uCbvSrvUavDescriptorSize);
            m_pD3D12GraphicsCommandList->SetGraphicsRootDescriptorTable(3, srvHandle);
            
            // Committe draw call
            m_pD3D12GraphicsCommandList->DrawIndexedInstanced(
                item->IndexCount, 1, item->StartIndexLocation, item->StartVertexLocation, 0
            );
        }

    }


    void D3D12GraphicsManager::UpdateMainPassConstBuffer(){
        auto& camera = m_pScene->MainCamera;

        XMMATRIX view = XMLoadFloat4x4(&camera.GetViewMatrix());
        XMMATRIX proj = XMLoadFloat4x4(&camera.GetProjMatrix());
        XMMATRIX viewProj = XMMatrixMultiply(view, proj);

        XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(view), view);
        XMMATRIX invProj = XMMatrixInverse(&XMMatrixDeterminant(proj), proj);
        XMMATRIX invViewProj = XMMatrixInverse(&XMMatrixDeterminant(viewProj), viewProj);

        XMStoreFloat4x4(&m_MainPassCB.View, XMMatrixTranspose(view));
        XMStoreFloat4x4(&m_MainPassCB.InvView, XMMatrixTranspose(invView));
        XMStoreFloat4x4(&m_MainPassCB.Proj, XMMatrixTranspose(proj));
        XMStoreFloat4x4(&m_MainPassCB.InvProj, XMMatrixTranspose(invProj));
        XMStoreFloat4x4(&m_MainPassCB.ViewProj, XMMatrixTranspose(viewProj));
        XMStoreFloat4x4(&m_MainPassCB.InvViewProj, XMMatrixTranspose(invViewProj));
        m_MainPassCB.EyePosW = camera.GetPosition();
        m_MainPassCB.pad1 = m_uCurrFrameResourceIndex;
        m_MainPassCB.RenderTargetSize = 
            Vector2f(g_GraphicsConfig.WndWidth, g_GraphicsConfig.WndHeight);
        m_MainPassCB.InvRenderTargetSize = 
            Vector2f(1.0f / g_GraphicsConfig.WndWidth, 1.0f / g_GraphicsConfig.WndHeight);
        m_MainPassCB.NearZ = 1.0f;
        m_MainPassCB.FarZ = 1000.0f;
        m_MainPassCB.TotalTime = m_pTimer->TotalTime();
        m_MainPassCB.DeltaTime = m_pTimer->DeltaTime();
        m_MainPassCB.AmbientLight = {0.05f, 0.05f, 0.05f, 1.0f};


        m_MainPassCB.Lights[0].Strength = Vector3f(0.4f, 0.4f, 0.4f);
        m_MainPassCB.Lights[0].Position = Vector3f(40.0f, 20.0f, 20.0f);

        m_MainPassCB.Lights[1].Position = Vector3f(
            105.0f * cosf(m_pTimer->TotalTime() / 10.0f),
            105.0f * sinf(m_pTimer->TotalTime() / 10.0f),
            0.0f
        );
        m_MainPassCB.Lights[1].Strength = Vector3f(0.5f, 0.5f, 0.5f);
        m_MainPassCB.Lights[1].FalloffStart = 1.0f;
        m_MainPassCB.Lights[1].FalloffEnd = 200.0f;

        m_MainPassCB.Lights[2].Position = Vector3f(0.0f, 25.0f, 0.0f);
        m_MainPassCB.Lights[2].Direction = Vector3f(
            0.8f * cosf(m_pTimer->TotalTime() / 2.0f),
            -0.6f,
            0.8f * sinf(m_pTimer->TotalTime() / 2.0f)
        );
        m_MainPassCB.Lights[2].Strength = Vector3f(2.0f, 0.0, 0.0f);
        m_MainPassCB.Lights[2].FalloffStart = 1.0f;
        m_MainPassCB.Lights[2].FalloffEnd = 200.0f;
        m_MainPassCB.Lights[2].SpotPower = 32.0f;

        m_MainPassCB.FogColor = {0.7f, 0.7f, 0.7f, 1.0f};
        m_MainPassCB.FogStart = 10.0f;
        m_MainPassCB.FogRange = 500.0;

        auto PassCB = m_pFrameResource[m_uCurrFrameResourceIndex]->PassCB.get();
        PassCB->CopyData(0, m_MainPassCB);

    }

    void D3D12GraphicsManager::UpdateObjectConstBuffer(){
        const auto& currFrameResource = m_pFrameResource[m_uCurrFrameResourceIndex];
        const auto& item = m_pScene->Instances;

        auto ObjectCB = currFrameResource->ObjectCB.get();

        XMMATRIX view = XMLoadFloat4x4(&m_pScene->MainCamera.GetViewMatrix());
        XMMATRIX invView = XMMatrixInverse(nullptr, view);

        BoundingFrustum localFrustum;
        item->VisibleInstanceCount = 0;

        for(uint32_t index = 0; index < item->InstanceDataArray.size(); index++){

            XMMATRIX World = XMLoadFloat4x4(&item->InstanceDataArray[index].World);
            World = XMMatrixTranspose(World);
            XMMATRIX invWorld = XMMatrixInverse(nullptr, World);

            XMMATRIX ViewToLocal = XMMatrixMultiply(invView, invWorld);

            m_pScene->MainCamera.GetFrustum().Transform(localFrustum, ViewToLocal);
            if(localFrustum.Contains(item->SubMesh->BBox) != DISJOINT){
                ObjectCB->CopyData(item->VisibleInstanceCount++, item->InstanceDataArray[index]);
            }

        }
            
    }

    void D3D12GraphicsManager::UpdateMaterialConstBuffer(){
        const auto& currFrameResource = m_pFrameResource[m_uCurrFrameResourceIndex];
        auto MaterialCB = currFrameResource->MaterialCB.get();

        for(auto& mat : m_pScene->Materials){

            if(mat.second->NumFramesDirty > 0){

                MaterialData matCB;
                matCB.DiffuseAlbedo = mat.second->DiffuseAlbedo;
                matCB.MatTransform  = mat.second->MatTransform;
                matCB.FresnelR0 = mat.second->FresnelR0;
                matCB.Roughness = mat.second->Roughness;
                matCB.DiffuseMapIndex = mat.second->DiffuseSrvHeapIndex;

                MaterialCB->CopyData(mat.second->MatCBIndex, matCB);
                mat.second->NumFramesDirty--;
            }

        }

    }

    D3D12_CPU_DESCRIPTOR_HANDLE 
    D3D12GraphicsManager::CurrentBackBufferView() const{
        return CD3DX12_CPU_DESCRIPTOR_HANDLE(
            m_pRtvHeap->GetCPUDescriptorHandleForHeapStart(),
            m_uCurrentBackBufferIndex, m_uRtvDescriptorSize
        );
    }

    D3D12_CPU_DESCRIPTOR_HANDLE 
    D3D12GraphicsManager::DepthStencilView() const{
        return m_pDsvHeap->GetCPUDescriptorHandleForHeapStart();
    }

    void D3D12GraphicsManager::FlushCommandQueue(){
        m_uCurrentFence++;

        m_pD3D12CommandQueue->Signal(m_pD3D12Fence.Get(), m_uCurrentFence);

        if(m_pD3D12Fence->GetCompletedValue() < m_uCurrentFence){

            HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);

            m_pD3D12Fence->SetEventOnCompletion(m_uCurrentFence, eventHandle);

            WaitForSingleObject(eventHandle, INFINITE);

            CloseHandle(eventHandle);
        }
    }

#if defined(_DEBUG)

    void D3D12GraphicsManager::LogAdaptors(){
        uint8_t index = 0;
        ComPtr<IDXGIAdapter> adaptor;

        while(m_pDXGIFactory->EnumAdapters(index, adaptor.GetAddressOf()) != DXGI_ERROR_NOT_FOUND){

            DXGI_ADAPTER_DESC adapterDesc;
            adaptor->GetDesc(&adapterDesc);
            
            std::wstring text;

            text = L"******Adaptor"+std::to_wstring(index)+L"******\n";
            text += adapterDesc.Description;
            text += L"\n";

            OutputDebugString(std::string(text.begin(), text.end()).c_str());

            LogAdaptorOutputs(adaptor);

            index++;
        }

    }

    void D3D12GraphicsManager::LogAdaptorOutputs(const ComPtr<IDXGIAdapter>& adapter){
        uint8_t index = 0;
        ComPtr<IDXGIOutput> output;
        while(adapter->EnumOutputs(index, output.GetAddressOf()) != DXGI_ERROR_NOT_FOUND){
            index++;

            DXGI_OUTPUT_DESC outputDesc;
            output->GetDesc(&outputDesc);
            
            std::wstring text;

            text = L"  ****Output****  \n";
            text += outputDesc.DeviceName;
            text += L"\n";

            OutputDebugString(std::string(text.begin(), text.end()).c_str());

        }
    }

    void D3D12GraphicsManager::LogOutputDisplayModes(const ComPtr<IDXGIOutput> &output){

    }

#endif

}