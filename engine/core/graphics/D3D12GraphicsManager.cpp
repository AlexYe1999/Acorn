#include"D3D12GraphicsManager.hpp"

namespace AcornEngine{

    D3D12GraphicsManager::D3D12GraphicsManager()
        :
        m_uRtvDescriptorSize(0),
        m_uDsvDescriptorSize(0),
        m_uCbvUavDescriptorSize(0),
        m_uCurrBufferIndex(0)
    {}

    void D3D12GraphicsManager::GetSettingParams(const GraphicsParam& param){
        m_GraphicsConfig = param;
    }

    void D3D12GraphicsManager::Initialize(){

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

        ComPtr<IDXGIAdapter> adaptor;
        m_pDXGIFactory->EnumAdapters(m_GraphicsConfig.AdapterIndex, adaptor.GetAddressOf());

        D3D12CreateDevice(adaptor.Get(),
            m_GraphicsConfig.FeatureLevel, 
            IID_PPV_ARGS(m_pD3D12Device.GetAddressOf())
        );

        m_uRtvDescriptorSize = 
            m_pD3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        m_uDsvDescriptorSize =
            m_pD3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
        m_uCbvUavDescriptorSize = 
            m_pD3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        CreateCommandObject();
        CreateSwapChain();
        CreateDescriptorHeaps();
        CreateBufferAndView();

        m_bIsInit = true;
    }

    void D3D12GraphicsManager::Finalize(){

    }

    void D3D12GraphicsManager::Tick(){

    }

    void D3D12GraphicsManager::Clear(){

    }

    void D3D12GraphicsManager::Render(){

    }

    void D3D12GraphicsManager::DrawLine(const Vector3f &from, const Vector3f &to, const Vector3f &color){

    }

    void D3D12GraphicsManager::DrawBox(const Vector3f &bbMin, const Vector3f &bbMax, const Vector3f &color){

    }
    
    void D3D12GraphicsManager::ClearDebugBuffers(){

    }

    void D3D12GraphicsManager::InitializeShaders(){

    }
    
    void D3D12GraphicsManager::ClearShaders(){

    }

    //void InitializeBuffers(const Scene& scene){}

    void D3D12GraphicsManager::ClearBuffers(){

    }

    void D3D12GraphicsManager::InitConstants(){

    }
    
    void D3D12GraphicsManager::CalculateCameraMatrix(){

    }
    
    void D3D12GraphicsManager::CalculateLights(){

    }
    
    void D3D12GraphicsManager::UpdateConstants(){

    }

    void D3D12GraphicsManager::RenderBuffers(){

    }

    inline void D3D12GraphicsManager::CreateCommandObject(){
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

        m_pD3D12GraphicsCommandList->Close();
    }

    inline void D3D12GraphicsManager::CreateSwapChain(){
        m_pDXGISwapChain.Reset();

        DXGI_SWAP_CHAIN_DESC swapChainDesc;
        swapChainDesc.BufferDesc.Width = m_GraphicsConfig.WndWidth;
        swapChainDesc.BufferDesc.Height = m_GraphicsConfig.WndHeight;
        swapChainDesc.BufferDesc.RefreshRate.Numerator   = 60;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDesc.BufferDesc.Format = m_GraphicsConfig.BackBufferFormat;
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        swapChainDesc.SampleDesc.Count   = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferCount = m_GraphicsConfig.SwapChainBufferCount;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.OutputWindow = m_GraphicsConfig.MainWnd;
        swapChainDesc.Windowed = true;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        m_pDXGIFactory->CreateSwapChain(
            m_pD3D12CommandQueue.Get(), &swapChainDesc, m_pDXGISwapChain.GetAddressOf()
        );

    }

    inline void D3D12GraphicsManager::CreateDescriptorHeaps(){
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
        rtvHeapDesc.Type  = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        rtvHeapDesc.NumDescriptors = m_GraphicsConfig.SwapChainBufferCount;
        rtvHeapDesc.NodeMask = 0;

        m_pD3D12Device->CreateDescriptorHeap(
            &rtvHeapDesc, IID_PPV_ARGS(m_pRtvHeap.GetAddressOf())
        );

        D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
        dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        dsvHeapDesc.NumDescriptors = 1;
        dsvHeapDesc.NodeMask = 0;

        m_pD3D12Device->CreateDescriptorHeap(
            &dsvHeapDesc, IID_PPV_ARGS(m_pDsvHeap.GetAddressOf())
        );
    }

    void D3D12GraphicsManager::CreateBufferAndView(){

        //TODO: Use MemoryManager to alloc Memory
        m_pRtBuffer.reset(new ComPtr<ID3D12Resource>[m_GraphicsConfig.SwapChainBufferCount]);

        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(
            m_pRtvHeap->GetCPUDescriptorHandleForHeapStart()
        );

        for(uint8_t index = 0; index < m_GraphicsConfig.SwapChainBufferCount; index++){
            m_pDXGISwapChain->GetBuffer(
                index, IID_PPV_ARGS(m_pRtBuffer[index].GetAddressOf())
            );

            m_pD3D12Device->CreateRenderTargetView(
                m_pRtBuffer[index].Get(), nullptr, rtvHeapHandle
            );

            rtvHeapHandle.Offset(1, m_uRtvDescriptorSize);
        }

        D3D12_RESOURCE_DESC depthStencilDesc;
        depthStencilDesc.Width  = m_GraphicsConfig.WndWidth;
        depthStencilDesc.Height = m_GraphicsConfig.WndHeight;
        depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        depthStencilDesc.Alignment = 0;
        depthStencilDesc.MipLevels = 1;
        depthStencilDesc.DepthOrArraySize = 1;
        depthStencilDesc.Format = m_GraphicsConfig.DepthStansilFormat;
        depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        depthStencilDesc.Flags  = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
        depthStencilDesc.SampleDesc.Count   = 1;
        depthStencilDesc.SampleDesc.Quality = 0;

        D3D12_CLEAR_VALUE optClear;
        optClear.Format = m_GraphicsConfig.DepthStansilFormat;
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

    inline D3D12_CPU_DESCRIPTOR_HANDLE 
    D3D12GraphicsManager::CurrentBackBufferView() const{
        return CD3DX12_CPU_DESCRIPTOR_HANDLE(
            m_pRtvHeap->GetCPUDescriptorHandleForHeapStart(),
            m_uCurrBufferIndex, m_uRtvDescriptorSize
        );
    }

    inline D3D12_CPU_DESCRIPTOR_HANDLE 
    D3D12GraphicsManager::DepthStencilView() const{
        return m_pDsvHeap->GetCPUDescriptorHandleForHeapStart();
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