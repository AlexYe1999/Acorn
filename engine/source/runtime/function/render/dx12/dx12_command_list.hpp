#pragma once

#include "runtime/function/render/command_list.hpp"
#include "runtime/function/render/dx12/dx12_command_queue.hpp"

#include <d3d12.h>
#include <wrl.h>

namespace Acorn
{
    using Microsoft::WRL::ComPtr;

    class Dx12CommandList : public CommandList
    {
    public:
        Dx12CommandList(Dx12CommandQueue* command_queue);
        void BeginRenderPass() override;
        void EndRenderPass() override;
        uint64_t Submit() override;

    private:
        Dx12CommandQueue*                  m_command_queue { nullptr };
        ComPtr<ID3D12GraphicsCommandList4> m_command_list  { nullptr };
    };
} 
