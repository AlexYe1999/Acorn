#pragma once
#include"../GraphicsManager.hpp"
#include"Vector.hpp"
#include"Scene.hpp"
#include"d3dx12.h"
#include"d3dUtil.hpp"
#include"UploadBuffer.hpp"
#include<wrl.h>
#include<memory>
#include<string>
#include<dxgi.h>
#include<D3D12.h>
#include<d3dcompiler.h>

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

        void ResetRtAndDs();

        virtual void Initialize() override;
        virtual void Finalize() override;
        virtual void Tick() override;

        virtual void Render() override;
        virtual void Clear() override;

        virtual void DrawLine(const Vector3f& from, const Vector3f& to, const Vector3f& color) override;
        virtual void DrawBox(const Vector3f& bbMin, const Vector3f& bbMax, const Vector3f& color) override;
        virtual void ClearDebugBuffers() override;

        void UpdataConstants(const float theta, const float phi);

    protected:
        virtual void InitializeBuffers(const Scene& scene) override;
        virtual void InitializeConstants() override;
        virtual void InitializeShaders() override;

        virtual void ClearBuffers() override;
        virtual void ClearShaders() override;

        virtual void CalculateCameraMatrix() override;
        virtual void CalculateLights() override;
        virtual void UpdateConstants() override;
        virtual void RenderBuffers() override;

    protected:
        void CreateDeviceAndFence();
        void CreateCommandObject();
        void CreateSwapChain();
        void CreateRtAndDs();
        void CreateRootSignature();
        void ConfigurePipline();

    protected:
        
        ComPtr<IDXGIFactory> m_pDXGIFactory;
        ComPtr<ID3D12Device> m_pD3D12Device;
        ComPtr<ID3D12Fence>  m_pD3D12Fence;

        ComPtr<ID3D12CommandQueue> m_pD3D12CommandQueue;
        ComPtr<ID3D12CommandAllocator> m_pD3D12CommandAllocator;
        ComPtr<ID3D12GraphicsCommandList> m_pD3D12GraphicsCommandList;
        
        ComPtr<IDXGISwapChain> m_pDXGISwapChain;

        ComPtr<ID3D12DescriptorHeap> m_pRtvHeap;
        ComPtr<ID3D12DescriptorHeap> m_pDsvHeap;
        ComPtr<ID3D12DescriptorHeap> m_pCbvHeap;
        D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
        D3D12_INDEX_BUFFER_VIEW  m_IndexBufferView;

        unique_ptr<ComPtr<ID3D12Resource>[]> m_pRtBuffer;
        ComPtr<ID3D12Resource> m_pDsBuffer;
        ComPtr<ID3D12Resource> m_pVertexUploadBuffer;
        ComPtr<ID3D12Resource> m_pIndexUploadBuffer;
        ComPtr<ID3D12Resource> m_pVertexBuffer;
        ComPtr<ID3D12Resource> m_pIndexBuffer;
        unique_ptr<UploadBuffer<Matrix4f, true>> m_pPass;

        ComPtr<ID3D12RootSignature> m_pRootSignature;

        ComPtr<ID3DBlob> m_pVSByteCode;
        ComPtr<ID3DBlob> m_pPSByteCode;
        ComPtr<ID3D12PipelineState> m_pPSO;

        uint16_t m_uCurrentFence;
        uint16_t m_uCurrentBufferIndex; 
        uint16_t m_uRtvDescriptorSize;
        uint16_t m_uDsvDescriptorSize;
        uint16_t m_uCbvUavDescriptorSize;

        Matrix4f m_MVPMatrix;

    protected:
        D3D12GraphicsManager();
        D3D12GraphicsManager(const D3D12GraphicsManager& instance) = delete;
        void operator =(const D3D12GraphicsManager& instance) = delete;
        static inline D3D12GraphicsManager* s_pInstance = nullptr;

    protected:
        D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
        D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;

        void FlushCommandQueue();

#if defined(_DEBUG)

    protected:

        void LogAdaptors();
        void LogAdaptorOutputs(const ComPtr<IDXGIAdapter>& adapter);
        void LogOutputDisplayModes(const ComPtr<IDXGIOutput>& output);

#endif
    };
}