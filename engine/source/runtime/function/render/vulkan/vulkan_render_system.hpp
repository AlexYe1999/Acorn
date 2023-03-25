#pragma once

#include "runtime/function/render/render_system.hpp"

namespace Acorn
{
    class VulkanRenderSystem : public RenderSystem
    {
    public:
        VulkanRenderSystem()  = default;
        ~VulkanRenderSystem() = default;

        virtual void InitSystem() override;
        virtual void ShutdownSystem() override;

        virtual void Tick(float delta_time) override;

    };
}