#include "dx12_rhi.hpp"
#include "dx12_graphics_pipeline.hpp"
#include "dx12_command_list.hpp"
#include "dx12_command_queue.hpp"
#include "runtime/engine.hpp"
#include "runtime/function/window/win32_window.hpp"

#include <stdexcept>
#include <iostream>
#include <memory>

namespace Acorn
{
	void Dx12RHI::Init()
	{
		CreateDevice();

		CreateSwapchain();

		m_rtv_descriptor_size = m_d3d12_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		m_dsv_descriptor_size = m_d3d12_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		m_srv_descriptor_size = m_d3d12_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	

	}

	void Dx12RHI::Cleanup()
	{

	}

	GraphicsPipelinePtr Dx12RHI::CreateGraphicsPipeline(GraphicsPipelineStateDescriptor const& descriptor)
	{
		return std::make_unique<Dx12GraphicsPipeline>(descriptor, m_d3d12_device.Get());
	}

	CommandQueuePtr Dx12RHI::CreateCommandQueue(COMMAND_LIST_TYPE type) const
	{
		return std::make_unique<Dx12CommandQueue>(type, m_d3d12_device);
	}

	void Dx12RHI::GetRenderTargets() const
	{

	}

	void Dx12RHI::ResizeBackBuffer(uint32_t frame_count, uint32_t width, uint32_t height) const
	{

	}

	void Dx12RHI::CreateDevice()
	{
		uint32_t dxgi_factory_flags = 0;

	#if defined(_DEBUG)

		ComPtr<ID3D12Debug> debug_controller;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller))))
		{
			debug_controller->EnableDebugLayer();
			dxgi_factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
		}

	#endif

		if (FAILED(CreateDXGIFactory2(dxgi_factory_flags, IID_PPV_ARGS(&m_dxgi_factory))))
		{
			throw std::runtime_error("failed to create dxgi factory !");
		}

		for (UINT adapter_index = 0; m_dxgi_factory->EnumAdapterByGpuPreference(adapter_index, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&m_dxgi_adapter)) != DXGI_ERROR_NOT_FOUND; adapter_index++)
		{
			DXGI_ADAPTER_DESC1 desc;
			m_dxgi_adapter->GetDesc1(&desc);
			if ((desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0)
			{
				if (SUCCEEDED(D3D12CreateDevice(m_dxgi_adapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&m_d3d12_device))))
				{
					break;
				}
			}

			m_dxgi_adapter = nullptr;
		}

		if (!IsDirectXRaytracingSupported(m_dxgi_adapter.Get()))
		{
			throw std::runtime_error("dx raytracing is not supported !");
		}

	#if defined(_DEBUG)

		DXGI_ADAPTER_DESC adapterDesc;
		m_dxgi_adapter->GetDesc(&adapterDesc);

		std::wstring text;

		text = L"selected adaptor : ";
		text += adapterDesc.Description;

		std::wcout << text << std::endl;

	#endif

	}

	void Dx12RHI::CreateSwapchain()
	{
		auto window = dynamic_cast<Win32Window*>(EngineRuntimeContext::WindowSystem());
		auto [width, height] = window->GetWindowSize();

        DXGI_SWAP_CHAIN_DESC1 swapchain_desc = {};
        swapchain_desc.Width       = width;
        swapchain_desc.Height      = height;
        swapchain_desc.BufferCount = 2;
        swapchain_desc.Format      = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapchain_desc.SwapEffect  = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
        swapchain_desc.SampleDesc.Count = 1;

		auto command_queue = Dx12CommandQueue(COMMAND_LIST_TYPE::GRAPHICS, m_d3d12_device);
		ComPtr<IDXGISwapChain1> swapchain;
		if (FAILED(m_dxgi_factory->CreateSwapChainForHwnd(command_queue.GetCommandQueue().Get(), window->Hwnd(), &swapchain_desc, nullptr, nullptr, swapchain.GetAddressOf())))
		{
			throw std::runtime_error("failed to create swapchain !");
		}

		if (FAILED(swapchain.As(&m_dxgi_swapchain)))
		{
			throw std::runtime_error("failed to cast to swapchain3 !");
		}

	}

	bool Dx12RHI::IsDirectXRaytracingSupported(IDXGIAdapter1* adapter)
	{
		ComPtr<ID3D12Device> test_device;
		D3D12_FEATURE_DATA_D3D12_OPTIONS5 feature_support_data = {};

		return SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&test_device)))
			&& SUCCEEDED(test_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &feature_support_data, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS5)))
			&& feature_support_data.RaytracingTier != D3D12_RAYTRACING_TIER_NOT_SUPPORTED;
	}


}