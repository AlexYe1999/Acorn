#pragma once

#include "graphics_pipeline.hpp"
#include "command_queue.hpp"
#include "texture.hpp"
#include "struct.hpp"

#include <cstdint>
#include <vector>

namespace Acorn
{
    class RHI
    {
    public:
        virtual ~RHI() {};

        virtual void Init()    = 0;
        virtual void Cleanup() = 0;

        virtual GraphicsPipelinePtr CreateGraphicsPipeline(GraphicsPipelineStateDescriptor const& descriptor) = 0;

        virtual CommandQueuePtr CreateCommandQueue(COMMAND_LIST_TYPE type) const = 0;

        virtual void GetRenderTargets() const = 0;
        virtual void ResizeBackBuffer(uint32_t frame_count, uint32_t width, uint32_t height) const = 0;

        //virtual void BeginPass()        = 0;

        //virtual void EndPass()          = 0;

        //virtual void SetPrimitive()     = 0;
        //virtual void SetViewports()     = 0;
        //virtual void SetScissorsRect()  = 0;
        //virtual void SetRenderTargets() = 0;

    };
}
