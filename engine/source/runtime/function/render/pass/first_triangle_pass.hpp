#pragma once

#include "runtime/engine.hpp"
#include "runtime/function/render/vulkan/vulkan_struct.hpp"
#include "runtime/function/render/render_pass.hpp"

namespace Acorn
{
    class FirstTrianglePass : public RenderPass
    {
    public:
        virtual void Init(RHI* const rhi) override
        {
            m_rhi = rhi;

            auto root = EngineRuntimeContext::ConfigSystem()->GetEngineRootPath();

            GraphicsPipelineStateDescriptor descriptor {};

            descriptor.input_assembly_descriptor =
            {
                INPUT_PRIMITIVE_TOPOLOGY::TRIANGLE_LIST,
                false
            };

            descriptor.shader_descriptors =
            {
                {
                    SHADER_TYPE::VERTEX_SHADER,
                    (root / "shader" / "simple_shader.vert.spv").string(),
                    "main"
                },
                {
                    SHADER_TYPE::FRAGMENT_SHADER,
                    (root / "shader" / "simple_shader.frag.spv").string(),
                    "main"
                }
            };

            descriptor.rasterizer_descriptor =
            {
                RASTER_MODE::FILL,
                RASTER_CULL_MODE::BACK,
                false, false, false, true,
                0.0f, 0.0f, 0.0f
            };

            descriptor.multisample_descriptor =
            {

            };

            descriptor.dynamic_states = {
                DYNAMIC_STATE::VIEW_PORT,
                DYNAMIC_STATE::STATE_LINE_WIDTH
            };

            m_graphics_pipeline_state = m_rhi->CreateGraphicsPipeline(descriptor);
        }
        
        virtual void Draw() override
        {

        }

        virtual void Cleanup() override
        {

        }

    private:
        GraphicsPipelinePtr m_graphics_pipeline_state;
    };

}