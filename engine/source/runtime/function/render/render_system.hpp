#pragma once

#include "rhi.hpp"

#include <memory>

namespace Acorn
{
    struct FrameResource
    {

    };

    class RenderSystem
    {
    public:
        RenderSystem()  = default;
        ~RenderSystem() = default;

        virtual void InitSystem();
        virtual void ShutdownSystem();

        virtual void Tick(const float delta_time);

    protected:
        std::unique_ptr<class RHI> m_rhi { nullptr };

    };
}
