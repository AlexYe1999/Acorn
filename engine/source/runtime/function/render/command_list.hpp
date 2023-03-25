#pragma once

#include <memory>

namespace Acorn
{
    class CommandList
    {
        virtual void BeginRenderPass() = 0;
        virtual void EndRenderPass() = 0;
        virtual uint64_t Submit() = 0;
    };
    using CommandListPtr = std::unique_ptr<CommandList>;
}
