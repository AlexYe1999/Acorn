#pragma once

#include "runtime/function/render/rhi.hpp"

namespace Acorn
{   
    class RenderPass
    {
    public:
        virtual void Init(RHI* const rhi) = 0;
        virtual void Draw()               = 0;
        virtual void Cleanup()            = 0;

    protected:
        RHI*                m_rhi { nullptr };
        GraphicsPipelinePtr m_graphics_pipeline { nullptr };
    };
}