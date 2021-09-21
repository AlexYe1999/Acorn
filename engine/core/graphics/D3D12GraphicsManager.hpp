#pragma once
#include"GraphicsManager.hpp"
#include"Vector.hpp"
#include<wrl.h>
#include<dxgi.h>
#include<D3D12.h>
#include<string>

namespace AcornEngine{
    using Microsoft::WRL::ComPtr;

    struct GraphicsParam{
        uint8_t AdapterIndex = 0;
        uint8_t BufferCount  = 2;
        uint16_t WndWidth  = 250;
        uint16_t WndHeight = 250;
        D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_12_0;
        DXGI_FORMAT BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
        HWND MainWnd = nullptr;
    };

    class D3D12GraphicsManager : public GraphicsManager{
    public:
        static GraphicsManager* GetInstance() {
            return s_pInstance != nullptr ? s_pInstance
                : s_pInstance = new GraphicsManager;
        }

        virtual void GetSettingParams(const GraphicsParam& param);
        virtual void Initialize() override;
        virtual void Finalize() override;
        virtual void Tick() override;

        virtual void Clear() override;
        virtual void Draw() override;

        virtual void DrawLine(const Vector3f& from, const Vector3f& to, const Vector3f& color) override;
        virtual void DrawBox(const Vector3f& bbMin, const Vector3f& bbMax, const Vector3f& color) override;
        virtual void ClearDebugBuffers() override;

    protected:
        virtual bool InitializeShaders() override;
        virtual void ClearShaders() override;
        //virtual void InitializeBuffers(const Scene& scene);
        virtual void ClearBuffers() override;

        virtual void InitConstants();
        virtual void CalculateCameraMatrix() override;
        virtual void CalculateLights() override;
        virtual void UpdateConstants() override;
        virtual void RenderBuffers() override;

    protected:
        void CreateCommandObject();
        void CreateSwapChain();


    protected:
        GraphicsParam m_GraphicsConfig;
        
        ComPtr<IDXGIFactory> m_pDXGIFactory;
        ComPtr<ID3D12Device> m_pD3D12Device;
        
        ComPtr<ID3D12CommandQueue> m_pD3D12CommandQueue;
        ComPtr<ID3D12CommandAllocator> m_pD3D12CommandAllocator;
        ComPtr<ID3D12GraphicsCommandList> m_pD3D12GraphicsCommandList;
        
        ComPtr<IDXGISwapChain> m_pDXGISwapChain;

    protected:
        GraphicsManager(){};
        GraphicsManager(const GraphicsManager& instance) = delete;
        void operator =(const GraphicsManager& instance) = delete;
        static inline GraphicsManager* s_pInstance = nullptr;

#if defined(_DEBUG)
    private:

        void LogAdaptors();
        void LogAdaptorOutputs(const ComPtr<IDXGIAdapter>& adapter);
        void LogOutputDisplayModes(const ComPtr<IDXGIOutput>& output);

#endif
    };
}