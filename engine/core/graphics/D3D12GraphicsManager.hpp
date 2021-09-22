#pragma once
#include"GraphicsManager.hpp"
#include"Vector.hpp"
#include"d3dx12.h"
#include<wrl.h>
#include<memory>
#include<string>
#include<dxgi.h>
#include<D3D12.h>


namespace AcornEngine{
    using Microsoft::WRL::ComPtr;
    using std::unique_ptr;

    struct GraphicsParam{
        HWND MainWnd = nullptr;
        uint16_t WndWidth  = 250;
        uint16_t WndHeight = 250;
        uint8_t AdapterIndex = 1;
        uint8_t SwapChainBufferCount = 2;
        D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_12_0;
        DXGI_FORMAT DepthStansilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
        DXGI_FORMAT BackBufferFormat   = DXGI_FORMAT_R8G8B8A8_UNORM;
        D3D12_VIEWPORT ViewPort = {};
        tagRECT ScissorRect = {};
    };

    class D3D12GraphicsManager : public GraphicsManager{
    public:
        static D3D12GraphicsManager* GetInstance() {
            return s_pInstance != nullptr ? s_pInstance
                : s_pInstance = new D3D12GraphicsManager;
        }

        virtual void GetSettingParams(const GraphicsParam& param);
        virtual void Initialize() override;
        virtual void Finalize() override;
        virtual void Tick() override;

        virtual void Clear() override;
        virtual void Render() override;

        virtual void DrawLine(const Vector3f& from, const Vector3f& to, const Vector3f& color) override;
        virtual void DrawBox(const Vector3f& bbMin, const Vector3f& bbMax, const Vector3f& color) override;
        virtual void ClearDebugBuffers() override;

    protected:
        //virtual void InitializeBuffers(const Scene& scene);
        virtual void InitializeShaders() override;
        virtual void ClearBuffers() override;
        virtual void ClearShaders() override;

        virtual void InitConstants() override;
        virtual void CalculateCameraMatrix() override;
        virtual void CalculateLights() override;
        virtual void UpdateConstants() override;
        virtual void RenderBuffers() override;

    protected:
        void CreateCommandObject();
        void CreateSwapChain();
        void CreateDescriptorHeaps();
        void CreateBufferAndView();

    protected:
        GraphicsParam m_GraphicsConfig;
        
        ComPtr<IDXGIFactory> m_pDXGIFactory;
        ComPtr<ID3D12Device> m_pD3D12Device;
        
        ComPtr<ID3D12CommandQueue> m_pD3D12CommandQueue;
        ComPtr<ID3D12CommandAllocator> m_pD3D12CommandAllocator;
        ComPtr<ID3D12GraphicsCommandList> m_pD3D12GraphicsCommandList;
        
        ComPtr<IDXGISwapChain> m_pDXGISwapChain;

        ComPtr<ID3D12DescriptorHeap> m_pRtvHeap;
        ComPtr<ID3D12DescriptorHeap> m_pDsvHeap;

        unique_ptr<ComPtr<ID3D12Resource>[]> m_pRtBuffer;
        ComPtr<ID3D12Resource> m_pDsBuffer;

        uint16_t m_uRtvDescriptorSize;
        uint16_t m_uDsvDescriptorSize;
        uint16_t m_uCbvUavDescriptorSize;
        uint16_t m_uCurrBufferIndex;

    protected:
        D3D12GraphicsManager();
        D3D12GraphicsManager(const D3D12GraphicsManager& instance) = delete;
        void operator =(const D3D12GraphicsManager& instance) = delete;
        static inline D3D12GraphicsManager* s_pInstance = nullptr;

    protected:
        D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
        D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;

#if defined(_DEBUG)
    protected:

        void LogAdaptors();
        void LogAdaptorOutputs(const ComPtr<IDXGIAdapter>& adapter);
        void LogOutputDisplayModes(const ComPtr<IDXGIOutput>& output);

#endif
    };
}