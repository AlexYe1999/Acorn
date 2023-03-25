#pragma once

#include "runtime/function/render/command_queue.hpp"

#include <d3d12.h>
#include <wrl.h>
#include <cstdint>
#include <queue>

namespace Acorn 
{
    using Microsoft::WRL::ComPtr;

    class Dx12CommandQueue : public CommandQueue
    {
    public:
        Dx12CommandQueue(COMMAND_LIST_TYPE type, const ComPtr<ID3D12Device8>& device);
        ~Dx12CommandQueue();

        virtual CommandListPtr GetCommandList() override;
 
        virtual uint64_t Signal() override;
        virtual bool IsFenceComplete(uint64_t fence_value) override;

        virtual void WaitForFenceValue(uint64_t fence_value) override;
        virtual void Flush() override;

        uint64_t ExecuteCommandList(ComPtr<ID3D12GraphicsCommandList4>& command_list);

        ComPtr<ID3D12CommandQueue> GetCommandQueue() { return m_d3d12_command_queue; }
        ComPtr<ID3D12GraphicsCommandList4> QueryCommandList();

    private:
        ComPtr<ID3D12CommandAllocator> CreateCommandAllocator();
        ComPtr<ID3D12GraphicsCommandList4> CreateCommandList(ComPtr<ID3D12CommandAllocator>& allocator);

    private:
 
        D3D12_COMMAND_LIST_TYPE     m_command_list_type;
        ComPtr<ID3D12Device8>       m_d3d12_device;
        ComPtr<ID3D12CommandQueue>  m_d3d12_command_queue;
        ComPtr<ID3D12Fence>         m_d3d12_fence;
        HANDLE                      m_fence_event;
        uint64_t                    m_fence_value;
 
        struct CommandAllocatorEntry
        {
            uint64_t fence_value;
            ComPtr<ID3D12CommandAllocator> command_allocator;
        };

        using CommandAllocatorQueue = std::queue<CommandAllocatorEntry>;
        using CommandListQueue      = std::queue<ComPtr<ID3D12GraphicsCommandList4>>;

        CommandAllocatorQueue       m_command_allocator_queue;
        CommandListQueue            m_command_list_queue;
    };

}
