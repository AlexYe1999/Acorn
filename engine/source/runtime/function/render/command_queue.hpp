#pragma once

#include "struct.hpp"
#include "command_list.hpp"

namespace Acorn 
{
    class CommandQueue
    {
    public:
        CommandQueue(COMMAND_LIST_TYPE type)
            : m_command_list_type(type) 
        {}

        virtual CommandListPtr GetCommandList() = 0;

        virtual uint64_t Signal() = 0;
        virtual bool IsFenceComplete(uint64_t fence_value) = 0;

        virtual void WaitForFenceValue(uint64_t fence_value) = 0;
        virtual void Flush() = 0;

    private:
        COMMAND_LIST_TYPE m_command_list_type;
    };
    using CommandQueuePtr = std::unique_ptr<CommandQueue>;
}
