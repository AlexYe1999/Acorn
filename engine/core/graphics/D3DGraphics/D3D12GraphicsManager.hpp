#pragma once
#include"../GraphicsManager.hpp"
#include"Vertex.hpp"
#include"UploadBuffer.hpp"
#include"FrameResource.hpp"
#include"d3dx12.h"
#include"d3dUtil.hpp"
#include<wrl.h>
#include<memory>
#include<cassert>
#include<string>
#include<dxgi.h>
#include<D3D12.h>
#include<d3dcompiler.h>

namespace Acorn{
    using Microsoft::WRL::ComPtr;
    using FrameResourcePtr = std::unique_ptr<FrameResource<PassConstant, ObjectConstant>>;
    using FrameResourcePtrVector = std::vector<FrameResourcePtr>;

    struct GraphicsParam{
        HWND MainWnd = nullptr;
        uint16_t WndWidth  = 500;
        uint16_t WndHeight = 500;
        uint8_t AdapterIndex = 1;
        uint8_t BackBufferCount = 2;
        uint8_t FrameResourceCount = 3;
        D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_12_0;
        DXGI_FORMAT DepthStansilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
        DXGI_FORMAT BackBufferFormat   = DXGI_FORMAT_R8G8B8A8_UNORM;
        D3D12_VIEWPORT ViewPort = {};
        tagRECT ScissorRect = {};
        std::string VSAddress = "";
        std::string PSAddress = "";
    };

    class D3D12GraphicsManager : public GraphicsManager{
    public:
        static D3D12GraphicsManager* GetInstance() {
            return ms_pInstance != nullptr
                ? ms_pInstance : ms_pInstance = new D3D12GraphicsManager;
        }

        void ResetRtAndDs();

        virtual void Initialize() override {};
        virtual void Initialize(Scene* const scene, Timer* const timer) override;
        virtual void Finalize() override;
        virtual void Tick() override;

        virtual void Render() override;
        virtual void Clear() override;

        virtual void DrawLine(const Vector3f& from, const Vector3f& to, const Vector3f& color) override;
        virtual void DrawBox(const Vector3f& bbMin, const Vector3f& bbMax, const Vector3f& color) override;
        virtual void ClearDebugBuffers() override;

    protected:
        virtual void InitializeBuffers() override;
        virtual void InitializeConstants() override;
        virtual void InitializeShaders() override;

        virtual void ClearBuffers() override;
        virtual void ClearShaders() override;

        virtual void UpdateConstants() override;
        virtual void RenderBuffers() override;

    protected:
        void BuildDeviceAndFence();
        void BuildCommandObject();
        void BuildSwapChain();
        void BuildRtAndDs();
        void BuildRootSignature();
        void BuildPSO();

    protected:
        void DrawOpaqueItems();
        void UpdateMainPassConstBuffer();


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

        std::unique_ptr<ComPtr<ID3D12Resource>[]> m_pRtBuffer;
        ComPtr<ID3D12Resource> m_pDsBuffer;
        FrameResourcePtrVector m_pFrameResource;

        ComPtr<ID3D12RootSignature> m_pRootSignature;

        ComPtr<ID3DBlob> m_pVSByteCode;
        ComPtr<ID3DBlob> m_pPSByteCode;

        uint64_t m_uCurrentFence;
        uint16_t m_uCurrentBackBufferIndex; 
        uint16_t m_uCurrFrameResourceIndex;
        uint16_t m_uRtvDescriptorSize;
        uint16_t m_uDsvDescriptorSize;
        uint16_t m_uCbvUavDescriptorSize;

    protected:
        Scene* m_pScene;
        Timer* m_pTimer;
        PassConstant m_MainPassCB;
        std::unordered_map<std::string, ComPtr<ID3D12PipelineState>> m_pPSOs;

    protected:
        D3D12GraphicsManager();
        D3D12GraphicsManager(const D3D12GraphicsManager& instance) = delete;
        void operator =(const D3D12GraphicsManager& instance) = delete;
        static inline D3D12GraphicsManager* ms_pInstance = nullptr;

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