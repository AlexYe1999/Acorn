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
        m_uCbvUavDescriptorSize(0),
        m_pScene(nullptr),
        m_pTimer(nullptr)
    {}

    void D3D12GraphicsManager::ResetRtAndDs(){

        FlushCommandQueue();
        
        m_pD3D12GraphicsCommandList->Reset(m_pD3D12CommandAllocator.Get(), nullptr);

        	// Release the previous resources we will be recreating.
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

        UpdateConstants();
        Render();
    
    }

    void D3D12GraphicsManager::Clear(){

    }

    void D3D12GraphicsManager::Render(){

        auto& currFrameResource = m_pFrameResource[m_uCurrFrameResourceIndex];

        currFrameResource->CmdListAlloc->Reset();
        m_pD3D12GraphicsCommandList->Reset(
            currFrameResource->CmdListAlloc.Get(), m_pPSOs["defaultPSO"].Get()
        );

        m_pD3D12GraphicsCommandList->RSSetViewports(1, &g_GraphicsConfig.ViewPort);
        m_pD3D12GraphicsCommandList->RSSetScissorRects(1, &g_GraphicsConfig.ScissorRect);

        m_pD3D12GraphicsCommandList->ResourceBarrier(
            1, &CD3DX12_RESOURCE_BARRIER::Transition(
                m_pRtBuffer[m_uCurrentBackBufferIndex].Get(), 
                D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET)
        );

        static float black[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        m_pD3D12GraphicsCommandList->ClearRenderTargetView(
            CurrentBackBufferView(), black, 0, nullptr
        );

        m_pD3D12GraphicsCommandList->ClearDepthStencilView(
            DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
            1.0f, 0, 0, nullptr
        );
        
        m_pD3D12GraphicsCommandList->OMSetRenderTargets(
            1, &CurrentBackBufferView(), true, &DepthStencilView()
        );

        ID3D12DescriptorHeap *descriptorHeap[] = {m_pCbvHeap.Get()};
        m_pD3D12GraphicsCommandList->SetDescriptorHeaps(1, descriptorHeap);
        m_pD3D12GraphicsCommandList->SetGraphicsRootSignature(m_pRootSignature.Get());

        const auto passOffset = 
            m_pScene->OpaqueRenderItems.size() * g_GraphicsConfig.FrameResourceCount;
        auto cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(
            m_pCbvHeap->GetGPUDescriptorHandleForHeapStart()
        );
        cbvHandle.Offset(
            passOffset + m_uCurrFrameResourceIndex, m_uCbvUavDescriptorSize
        );

        m_pD3D12GraphicsCommandList->SetGraphicsRootDescriptorTable(0, cbvHandle);

        DrawOpaqueItems();
        
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
        m_pVSByteCode = D3DUtil::LoadBinaryShader("E:\\Code\\Acorn\\build\\engine\\core\\graphics\\Debug\\VS.cso");
        m_pPSByteCode = D3DUtil::LoadBinaryShader("E:\\Code\\Acorn\\build\\engine\\core\\graphics\\Debug\\PS.cso");
    }
    
    void D3D12GraphicsManager::ClearShaders(){

    }

    void D3D12GraphicsManager::ClearBuffers(){

    }

    void D3D12GraphicsManager::InitializeConstants(){

        const auto& frameResourceCount = g_GraphicsConfig.FrameResourceCount;
        for(int index = 0; index < frameResourceCount; index++){
            m_pFrameResource.push_back(std::move(
                std::make_unique<FrameResource<PassConstant, ObjectConstant>>(
                    m_pD3D12Device.Get(), 1, m_pScene->OpaqueRenderItems.size())
            ));
        }

        uint16_t objCount = m_pScene->OpaqueRenderItems.size();
        uint16_t numDescriptor = (objCount+1) * frameResourceCount;

        D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
        cbvHeapDesc.NumDescriptors = numDescriptor;
        cbvHeapDesc.Type  = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        cbvHeapDesc.NodeMask = 0;
         
        m_pD3D12Device->CreateDescriptorHeap(
            &cbvHeapDesc, IID_PPV_ARGS(m_pCbvHeap.GetAddressOf())
        );

        for(int index = 0; index < frameResourceCount; index++){
            const auto& objectCB = m_pFrameResource[index]->ObjectCB->Resource();
            auto cbAddress = objectCB->GetGPUVirtualAddress();

            for(int i = 0; i < objCount; i++){
                uint16_t heapIndex = index * objCount + i;
                auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
                    m_pCbvHeap->GetCPUDescriptorHandleForHeapStart()
                );
                handle.Offset(heapIndex, m_uCbvUavDescriptorSize);

                D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
                cbvDesc.BufferLocation = cbAddress;
                cbvDesc.SizeInBytes = D3DUtil::CalcAlignment(sizeof(ObjectConstant));

                m_pD3D12Device->CreateConstantBufferView(&cbvDesc, handle);

                cbAddress += D3DUtil::CalcAlignment(sizeof(ObjectConstant));
            }
        }

        auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
            m_pCbvHeap->GetCPUDescriptorHandleForHeapStart()
        );
        handle.Offset(frameResourceCount*objCount, m_uCbvUavDescriptorSize);
        for(int index = 0; index < frameResourceCount; index++){
            const auto& passCB = m_pFrameResource[index]->PassCB->Resource();
            auto cbAddress = passCB->GetGPUVirtualAddress();

            D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
            cbvDesc.BufferLocation = cbAddress;
            cbvDesc.SizeInBytes = D3DUtil::CalcAlignment(sizeof(PassConstant));

            m_pD3D12Device->CreateConstantBufferView(&cbvDesc, handle);

            handle.Offset(1, m_uCbvUavDescriptorSize);
        }

    }
    
    void D3D12GraphicsManager::UpdateConstants(){
        
        auto& currFrameResource = m_pFrameResource[m_uCurrFrameResourceIndex];

        if(currFrameResource->Fence != 0 
            && m_pD3D12Fence->GetCompletedValue() < currFrameResource->Fence){

            HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
            m_pD3D12Fence->SetEventOnCompletion(currFrameResource->Fence, eventHandle);
            WaitForSingleObject(eventHandle, INFINITE);
            CloseHandle(eventHandle);

        }

        UpdateMainPassConstBuffer();

        auto ObjectCB = currFrameResource->ObjectCB.get();

        for(auto& item : m_pScene->OpaqueRenderItems){

            if(item->DirtyCount > 0){
                XMMATRIX world = XMLoadFloat4x4(&item->World);

                ObjectConstant objConstant;
                XMStoreFloat4x4(&objConstant.World, XMMatrixTranspose(world));

                ObjectCB->CopyData(item->ObjCBIndex, objConstant);

                item->DirtyCount--;
            }
        }

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
        m_uCbvUavDescriptorSize = 
            m_pD3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        m_pD3D12Device->CreateFence(
            0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_pD3D12Fence.GetAddressOf())
        );
    }

    inline void D3D12GraphicsManager::BuildCommandObject(){
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

    inline void D3D12GraphicsManager::BuildSwapChain(){
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
        CD3DX12_ROOT_PARAMETER slotRootParam[2];
        CD3DX12_DESCRIPTOR_RANGE cbvTable0;
        cbvTable0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

        CD3DX12_DESCRIPTOR_RANGE cbvTable1;
        cbvTable1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);

        slotRootParam[0].InitAsDescriptorTable(1, &cbvTable0);
        slotRootParam[1].InitAsDescriptorTable(1, &cbvTable1);
        CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(
            2, slotRootParam, 0, nullptr,
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
            IID_PPV_ARGS(m_pRootSignature.GetAddressOf())
        );

    }

    void D3D12GraphicsManager::BuildPSO(){

        ComPtr<ID3D12PipelineState> defaultPSO;
        //Rasterizer state
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

        //Pipline State Object
        D3D12_GRAPHICS_PIPELINE_STATE_DESC piplineStateDesc;
        ZeroMemory(&piplineStateDesc, sizeof(piplineStateDesc));
        piplineStateDesc.InputLayout = {
            VertexP3C4::Desc.data(), 
            VertexP3C4::Desc.size()
        };
        piplineStateDesc.StreamOutput = {};
        piplineStateDesc.pRootSignature = m_pRootSignature.Get();
        piplineStateDesc.VS = {
            reinterpret_cast<BYTE*>(m_pVSByteCode->GetBufferPointer()),
            m_pVSByteCode->GetBufferSize()
        };
        piplineStateDesc.PS = {
            reinterpret_cast<BYTE *>(m_pPSByteCode->GetBufferPointer()),
            m_pPSByteCode->GetBufferSize()
        };
        piplineStateDesc.DS = {};
        piplineStateDesc.HS = {};
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

        HRESULT res =  m_pD3D12Device->CreateGraphicsPipelineState(
            &piplineStateDesc, IID_PPV_ARGS(defaultPSO.GetAddressOf())
        );

        m_pPSOs["defaultPSO"] = std::move(defaultPSO);
    }

    void D3D12GraphicsManager::DrawOpaqueItems(){

        constexpr uint16_t objCBByteSize = 
            D3DUtil::CalcAlignment(sizeof(ObjectConstant));
        uint16_t objCBByteOffset = 
            m_pScene->OpaqueRenderItems.size() * m_uCurrFrameResourceIndex;

        for(const auto& item : m_pScene->OpaqueRenderItems){
            m_pD3D12GraphicsCommandList->IASetVertexBuffers(0, 1, &item->Mesh->VertexBufferView());
            m_pD3D12GraphicsCommandList->IASetIndexBuffer(&item->Mesh->IndexBufferView());
            m_pD3D12GraphicsCommandList->IASetPrimitiveTopology(item->PrimitiveType);

            auto cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(
                m_pCbvHeap->GetGPUDescriptorHandleForHeapStart()
            );
            cbvHandle.Offset(objCBByteOffset+item->ObjCBIndex, m_uCbvUavDescriptorSize);
            m_pD3D12GraphicsCommandList->SetGraphicsRootDescriptorTable(1, cbvHandle);
            m_pD3D12GraphicsCommandList->DrawIndexedInstanced(
                item->IndexCount, 1, item->StartIndexLocation, item->StartVertexLocation, 0
            );
        }
    }
    
    inline void D3D12GraphicsManager::UpdateMainPassConstBuffer(){
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
        m_MainPassCB.RenderTargetSize = 
            Vector2f(g_GraphicsConfig.WndWidth, g_GraphicsConfig.WndHeight);
        m_MainPassCB.InvRenderTargetSize = 
            Vector2f(1.0f / g_GraphicsConfig.WndWidth, 1.0f / g_GraphicsConfig.WndHeight);
        m_MainPassCB.NearZ = 1.0f;
        m_MainPassCB.FarZ = 1000.0f;
        m_MainPassCB.TotalTime = m_pTimer->TotalTime();
        m_MainPassCB.DeltaTime = m_pTimer->DeltaTime();

        m_MainPassCB.pad1 = m_uCurrFrameResourceIndex;

        auto PassCB = m_pFrameResource[m_uCurrFrameResourceIndex]->PassCB.get();
        PassCB->CopyData(0, m_MainPassCB);

    }

    inline D3D12_CPU_DESCRIPTOR_HANDLE 
    D3D12GraphicsManager::CurrentBackBufferView() const{
        return CD3DX12_CPU_DESCRIPTOR_HANDLE(
            m_pRtvHeap->GetCPUDescriptorHandleForHeapStart(),
            m_uCurrentBackBufferIndex, m_uRtvDescriptorSize
        );
    }

    inline D3D12_CPU_DESCRIPTOR_HANDLE 
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