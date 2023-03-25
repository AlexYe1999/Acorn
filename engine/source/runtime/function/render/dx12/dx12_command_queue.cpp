#include "dx12_struct.hpp"
#include "dx12_command_list.hpp"
#include "dx12_command_queue.hpp"

#include <stdexcept>

namespace Acorn
{
    Dx12CommandQueue::Dx12CommandQueue(COMMAND_LIST_TYPE type, const ComPtr<ID3D12Device8>& device)
        : CommandQueue(type)
        , m_command_list_type(Dx12Enum::CommandListType(type))
        , m_fence_value(0)
        , m_d3d12_device(device)
    {

        D3D12_COMMAND_QUEUE_DESC desc = {};
        desc.Type = m_command_list_type;
        desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        desc.NodeMask = 0;

        if (FAILED(m_d3d12_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_d3d12_command_queue))))
        {
            throw std::runtime_error("failed to create command queue !");
        }

        if (FAILED(m_d3d12_device->CreateFence(m_fence_value, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_d3d12_fence))))
        {
            throw std::runtime_error("failed to create command queue !");
        }

        m_fence_event = ::CreateEvent(NULL, FALSE, FALSE, NULL);

        if (m_fence_event == nullptr)
        {
            throw std::runtime_error("failed to create fence event !");
        }
    }

    Dx12CommandQueue::~Dx12CommandQueue()
    {
        Flush();
    }

    CommandListPtr Dx12CommandQueue::GetCommandList()
    {
        return std::make_unique<Dx12CommandList>(this);
    }

    uint64_t Dx12CommandQueue::Signal()
    {
        uint64_t fence_value = ++m_fence_value;

        if (FAILED(m_d3d12_command_queue->Signal(m_d3d12_fence.Get(), fence_value)))
        {
            throw std::runtime_error("failed to signal !");
        }

        return fence_value;
    }

    bool Dx12CommandQueue::IsFenceComplete(uint64_t fenceValue)
    {
        return m_d3d12_fence->GetCompletedValue() >= fenceValue;
    }

    void Dx12CommandQueue::WaitForFenceValue(uint64_t fenceValue)
    {
        if (!IsFenceComplete(fenceValue))
        {
            if (FAILED(m_d3d12_fence->SetEventOnCompletion(fenceValue, m_fence_event)))
            {
                throw std::runtime_error("failed to set completion event !");
            }
            ::WaitForSingleObject(m_fence_event, DWORD_MAX);
        }
    }

    void Dx12CommandQueue::Flush()
    {
        WaitForFenceValue(Signal());
    }

    uint64_t Dx12CommandQueue::ExecuteCommandList(ComPtr<ID3D12GraphicsCommandList4>& command_list)
    {
        command_list->Close();

        ID3D12CommandAllocator* command_allocator;
        UINT data_size = sizeof(command_allocator);

        if (FAILED(command_list->GetPrivateData(__uuidof(ID3D12CommandAllocator), &data_size, &command_allocator)))
        {
            throw std::runtime_error("failed to get private data !");
        }

        ID3D12CommandList* const pp_command_lists[] = { command_list.Get() };

        m_d3d12_command_queue->ExecuteCommandLists(1, pp_command_lists);
        uint64_t fence_value = Signal();

        m_command_allocator_queue.emplace(CommandAllocatorEntry{ fence_value, command_allocator });
        m_command_list_queue.push(command_list);

        command_allocator->Release();
 
        return fence_value;
    }

    ComPtr<ID3D12GraphicsCommandList4> Dx12CommandQueue::QueryCommandList()
    {
        ComPtr<ID3D12CommandAllocator> command_allocator;
        ComPtr<ID3D12GraphicsCommandList4> command_list;

        if (!m_command_allocator_queue.empty() && IsFenceComplete(m_command_allocator_queue.front().fence_value))
        {
            command_allocator = m_command_allocator_queue.front().command_allocator;
 
            if (FAILED(command_allocator->Reset()))
            {
                throw std::runtime_error("failed to reset command allocator !");
            }

            m_command_allocator_queue.pop();
        }
        else
        {
            command_allocator = CreateCommandAllocator();
        }

        if (!m_command_list_queue.empty())
        {
            command_list = m_command_list_queue.front();
 
            if (FAILED(command_list->Reset(command_allocator.Get(), nullptr)))
            {
                throw std::runtime_error("failed to reset command list !");
            }

            m_command_list_queue.pop();
        }
        else
        {
            command_list = CreateCommandList(command_allocator);
        }

        if (FAILED(command_list->SetPrivateDataInterface(__uuidof(ID3D12CommandAllocator), command_allocator.Get())))
        {
            throw std::runtime_error("failed to set private data !");
        }

        return command_list;
    }

    ComPtr<ID3D12CommandAllocator> Dx12CommandQueue::CreateCommandAllocator()
    {
        ComPtr<ID3D12CommandAllocator> command_allocator;

        if (FAILED(m_d3d12_device->CreateCommandAllocator(m_command_list_type, IID_PPV_ARGS(command_allocator.GetAddressOf()))))
        {
            throw std::runtime_error("failed to create command allocator !");
        }

        return command_allocator;
    }

    ComPtr<ID3D12GraphicsCommandList4> Dx12CommandQueue::CreateCommandList(ComPtr<ID3D12CommandAllocator>& allocator)
    {
        ComPtr<ID3D12GraphicsCommandList4> command_list;

        if (FAILED(m_d3d12_device->CreateCommandList(0, m_command_list_type, allocator.Get(), nullptr, IID_PPV_ARGS(&command_list))))
        {
            throw std::runtime_error("failed to set private data !");
        }
 
        return command_list;
    }
}