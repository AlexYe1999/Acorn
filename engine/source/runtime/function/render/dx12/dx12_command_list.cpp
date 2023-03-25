#include "dx12_command_list.hpp"

namespace Acorn
{
    Dx12CommandList::Dx12CommandList(Dx12CommandQueue* command_queue)
        : m_command_queue(command_queue)
    {
        m_command_list = m_command_queue->QueryCommandList();
    }

    void Dx12CommandList::BeginRenderPass()
    {

    }

    void Dx12CommandList::EndRenderPass()
    {

    }

    uint64_t Dx12CommandList::Submit()
    {
        return m_command_queue->ExecuteCommandList(m_command_list);
    }
} 
