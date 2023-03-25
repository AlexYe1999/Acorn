#include "render_system.hpp"
#include "dx12/dx12_rhi.hpp"
#include "pass/first_triangle_pass.hpp"

namespace Acorn
{
    void RenderSystem::InitSystem()
    {
        m_rhi = std::make_unique<Dx12RHI>();
        m_rhi->Init();
    }

    void RenderSystem::ShutdownSystem()
    {
        m_rhi->Cleanup();
    }

    void RenderSystem::Tick(const float delta_time)
    {

    }

}