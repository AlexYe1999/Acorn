#include "vulkan_render_system.hpp"
#include "vulkan_rhi.hpp"

namespace Acorn
{
    void VulkanRenderSystem::InitSystem()
    {
        //m_rhi = std::make_unique<VulkanRHI>();
        //m_rhi->Init();
        
        //m_frame_graph = std::make_unique<FrameGraph>();
        //m_frame_graph->Init();
    }

    void VulkanRenderSystem::ShutdownSystem()
    {
        //m_rhi->Cleanup();
        //m_frame_graph->Cleanup();
    }

    void VulkanRenderSystem::Tick(float delta_time)
    {
        //m_frame_graph->Render();
    }
}