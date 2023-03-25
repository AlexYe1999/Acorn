#pragma once

#include "runtime/function/render/rhi.hpp"
#include "runtime/function/render/dx12/dx12_command_queue.hpp"
#include "runtime/function/render/graphics_pipeline.hpp"

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>

namespace Acorn
{
    using Microsoft::WRL::ComPtr;

    class Dx12RHI final : public RHI
    {
    public:
        
         void Init()    override;
        void Cleanup() override;

        GraphicsPipelinePtr CreateGraphicsPipeline(GraphicsPipelineStateDescriptor const& descriptor) override;

        CommandQueuePtr CreateCommandQueue(COMMAND_LIST_TYPE type) const override;

        void GetRenderTargets() const override;
        void ResizeBackBuffer(uint32_t frame_count, uint32_t width, uint32_t height) const override;

    private:
        void CreateDevice();
        void CreateSwapchain();

    private:
        bool IsDirectXRaytracingSupported(IDXGIAdapter1* adapter);

    private:

        using Dx12CommandQueuePtr = std::unique_ptr<Dx12CommandQueue>;

        size_t m_rtv_descriptor_size { 0 };
        size_t m_dsv_descriptor_size { 0 };
        size_t m_srv_descriptor_size { 0 };

        ComPtr<IDXGIFactory6>   m_dxgi_factory   { nullptr };
        ComPtr<IDXGIAdapter1>   m_dxgi_adapter   { nullptr };
        ComPtr<IDXGISwapChain3> m_dxgi_swapchain { nullptr };
        ComPtr<ID3D12Device8>   m_d3d12_device   { nullptr };

    };
}