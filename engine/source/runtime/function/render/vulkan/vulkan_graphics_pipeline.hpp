#pragma once

#include "runtime/function/render/graphics_pipeline.hpp"
#include "runtime/function/render/shader.hpp"

#include <vulkan/vulkan.h>

namespace Acorn
{
    class VulkanGraphicsPipeline : public GraphicsPipeline
    {
    public:
        VulkanGraphicsPipeline(GraphicsPipelineStateDescriptor const& descriptor, VkDevice device);
        ~VulkanGraphicsPipeline();

    private:
        VkDevice         m_device            { nullptr };
        ShaderPtr        m_vert_shader       { nullptr };
        ShaderPtr        m_frag_shader       { nullptr };
        VkRenderPass     m_render_pass       { nullptr };
        VkPipelineLayout m_pipeline_layout   { nullptr };
        VkPipeline       m_graphics_pipeline { nullptr };
    };
}