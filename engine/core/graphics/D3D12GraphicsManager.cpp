#include"D3D12GraphicsManager.hpp"

namespace AcornEngine{

    void GraphicsManager::GetSettingParams(const GraphicsParam& param){
        m_GraphicsConfig = param;
    }


    void GraphicsManager::Initialize(){

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


        CreateCommandObject();
        CreateSwapChain();

        m_bIsInit = true;
    }

    void GraphicsManager::Finalize(){

    }

    void GraphicsManager::Tick(){

    }

    void GraphicsManager::Clear(){

    }

    void GraphicsManager::Draw(){

    }

    inline void GraphicsManager::CreateCommandObject(){
        D3D12_COMMAND_QUEUE_DESC commandQueueDesc;
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

    inline void GraphicsManager::CreateSwapChain(){
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
        swapChainDesc.BufferCount = m_GraphicsConfig.BufferCount;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.OutputWindow = m_GraphicsConfig.MainWnd;
        swapChainDesc.Windowed = true;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        m_pDXGIFactory->CreateSwapChain(
            m_pD3D12Device.Get(), &swapChainDesc, m_pDXGISwapChain.GetAddressOf()
        );

    }

#if defined(_DEBUG)

    void GraphicsManager::LogAdaptors(){
        uint8_t index = 0;
        ComPtr<IDXGIAdapter> adaptor;

        while(m_pDXGIFactory->EnumAdapters(index, adaptor.GetAddressOf()) != DXGI_ERROR_NOT_FOUND){
            index++;

            DXGI_ADAPTER_DESC adapterDesc;
            adaptor->GetDesc(&adapterDesc);
            
            std::wstring text;

            text = L"******Adaptor******";
            text += adapterDesc.Description;
            text += L"\n";

            OutputDebugString(std::string(text.begin(), text.end()).c_str());

            LogAdaptorOutputs(adaptor);
        }

    }

    void GraphicsManager::LogAdaptorOutputs(const ComPtr<IDXGIAdapter>& adapter){
        uint8_t index = 0;
        ComPtr<IDXGIOutput> output;
        while(adapter->EnumOutputs(index, output.GetAddressOf()) != DXGI_ERROR_NOT_FOUND){
            index++;

            DXGI_OUTPUT_DESC outputDesc;
            output->GetDesc(&outputDesc);
            
            std::wstring text;

            text = L"  ****Output****  ";
            text += outputDesc.DeviceName;
            text += L"\n";

            OutputDebugString(std::string(text.begin(), text.end()).c_str());

        }
    }

    void GraphicsManager::LogOutputDisplayModes(const ComPtr<IDXGIOutput> &output){

    }

#endif

}