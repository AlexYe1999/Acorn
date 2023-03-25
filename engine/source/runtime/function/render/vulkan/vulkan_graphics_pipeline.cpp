#include "runtime/engine.hpp"
#include "vulkan_graphics_pipeline.hpp"
#include "runtime/function/render/vulkan/vulkan_shader.hpp"
#include "runtime/function/render/vulkan/vulkan_struct.hpp"

namespace Acorn
{
    VulkanGraphicsPipeline::VulkanGraphicsPipeline(GraphicsPipelineStateDescriptor const& descriptor, VkDevice device)
    {
        m_device = device;

        auto vert_shader     = std::make_unique<VulkanShader>(descriptor.shader_descriptors[0], m_device);
        auto vert_stage_info = vert_shader->ShaderStageInfo();
        m_vert_shader        = std::move(vert_shader);

        auto frag_shader     = std::make_unique<VulkanShader>(descriptor.shader_descriptors[1], m_device);
        auto frag_stage_info = frag_shader->ShaderStageInfo();
        m_frag_shader        = std::move(vert_shader);

		VkPipelineShaderStageCreateInfo shader_stages[] =
		{
			vert_stage_info,
			frag_stage_info
		};

        // VertexInput
		VkPipelineVertexInputStateCreateInfo vertex_input_info {};
		vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertex_input_info.vertexBindingDescriptionCount   = 0;
		vertex_input_info.pVertexBindingDescriptions      = nullptr;
		vertex_input_info.vertexAttributeDescriptionCount = 0;
		vertex_input_info.pVertexAttributeDescriptions    = nullptr;

        // inputAssembly
		VkPipelineInputAssemblyStateCreateInfo input_assembly_info {};
        auto& primitive_descriptor = descriptor.input_assembly_descriptor;

        input_assembly_info.sType    = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        input_assembly_info.topology = VulkanEnum::PrimitiveTopology(primitive_descriptor.primitive_topology);
		input_assembly_info.primitiveRestartEnable = primitive_descriptor.primitive_restart ? VK_TRUE : VK_FALSE;

        auto [width, height] = EngineRuntimeContext::WindowSystem()->GetWindowSize();

        VkViewport viewport {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width  = width;
        viewport.height = height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D rect {};
        rect.offset.x = 0;
        rect.offset.y = 0;
        rect.extent.width  = width;
        rect.extent.height = height;

        VkPipelineViewportStateCreateInfo viewport_info {};
        viewport_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewport_info.viewportCount = 1;
        viewport_info.pViewports = &viewport;
        viewport_info.scissorCount = 1;
        viewport_info.pScissors = &rect;

        // rasterization
        VkPipelineRasterizationStateCreateInfo rasterizer_info {};
        auto& rasterizer_descriptor = descriptor.rasterizer_descriptor;

        rasterizer_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer_info.depthClampEnable        = rasterizer_descriptor.depth_clip_enable ? VK_TRUE : VK_FALSE;
        rasterizer_info.rasterizerDiscardEnable = rasterizer_descriptor.output_discard_enable ? VK_TRUE : VK_FALSE;
        rasterizer_info.cullMode  = VulkanEnum::CullMode(rasterizer_descriptor.raster_cull_mode);
        rasterizer_info.frontFace = rasterizer_descriptor.front_face_clockwise ? VK_FRONT_FACE_CLOCKWISE : VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizer_info.depthBiasEnable         = rasterizer_descriptor.depth_bias_enable ? VK_TRUE : VK_FALSE;
        rasterizer_info.depthBiasConstantFactor = rasterizer_descriptor.depth_bias_constant_factor;
        rasterizer_info.depthBiasClamp          = rasterizer_descriptor.depth_bias_clamp;
        rasterizer_info.depthBiasSlopeFactor    = rasterizer_descriptor.depth_bias_slope_factor;

        // multisample
        VkPipelineMultisampleStateCreateInfo multisample_info {};
        multisample_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisample_info.sampleShadingEnable  = VK_FALSE;
        multisample_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisample_info.pSampleMask = nullptr;
        multisample_info.alphaToCoverageEnable = VK_FALSE;
        multisample_info.alphaToOneEnable      = VK_FALSE;

        // merge
        VkPipelineColorBlendAttachmentState color_blend_attachment {};
        auto& color_blend_descriptor = descriptor.output_mergement_descriptor;

        color_blend_attachment.colorWriteMask = VulkanEnum::ColorComponetMask(color_blend_descriptor.render_target_write_mask);
        color_blend_attachment.blendEnable = VK_FALSE;
        color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
        color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
        color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
        color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo color_blend_state_info {};
        color_blend_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        color_blend_state_info.flags = 0;
        color_blend_state_info.logicOpEnable = false;
        color_blend_state_info.logicOp = VK_LOGIC_OP_COPY;
        color_blend_state_info.attachmentCount = 1;
        color_blend_state_info.pAttachments = &color_blend_attachment;
        color_blend_state_info.blendConstants[0] = 0.0f;
        color_blend_state_info.blendConstants[1] = 0.0f;
        color_blend_state_info.blendConstants[2] = 0.0f;
        color_blend_state_info.blendConstants[3] = 0.0f;

        VkPipelineDynamicStateCreateInfo dynamic_state_info {};
        std::vector<VkDynamicState> dynamic_state {};

        for (auto state : descriptor.dynamic_states)
        {
            dynamic_state.push_back(VulkanEnum::DynamicState(state));
        }

        dynamic_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamic_state_info.dynamicStateCount = dynamic_state.size();
        dynamic_state_info.pDynamicStates = dynamic_state.data();

        VkAttachmentDescription attachment_desc {};
        //attachment_desc.format  = format;
        attachment_desc.samples = VK_SAMPLE_COUNT_1_BIT;
        attachment_desc.loadOp  = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachment_desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachment_desc.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachment_desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachment_desc.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
        attachment_desc.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference color_attachment_reference {};
        color_attachment_reference.attachment = 0;
        color_attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color_attachment_reference;

        VkRenderPassCreateInfo render_pass_info {};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        render_pass_info.attachmentCount = 1;
        render_pass_info.subpassCount = 1;
        render_pass_info.pSubpasses = &subpass;
 
        if (vkCreateRenderPass(m_device, &render_pass_info, nullptr, &m_render_pass) == VK_SUCCESS)
        {
            throw std::runtime_error("failed to create render pass!");
        }

        VkPipelineLayoutCreateInfo pipeline_layout_info {};
        pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipeline_layout_info.setLayoutCount = 0;
        pipeline_layout_info.pSetLayouts = nullptr;
        pipeline_layout_info.pushConstantRangeCount = 0;
        pipeline_layout_info.pPushConstantRanges = nullptr;

        if (vkCreatePipelineLayout(m_device, &pipeline_layout_info, nullptr, &m_pipeline_layout) == VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout!");
        }

        VkGraphicsPipelineCreateInfo graphics_pipeline_info {};
        graphics_pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        graphics_pipeline_info.stageCount = 2;
        graphics_pipeline_info.pStages = shader_stages;
        graphics_pipeline_info.pVertexInputState = &vertex_input_info;
        graphics_pipeline_info.pInputAssemblyState = &input_assembly_info;
        graphics_pipeline_info.pViewportState = &viewport_info;
        graphics_pipeline_info.pRasterizationState = &rasterizer_info;
        graphics_pipeline_info.pMultisampleState = &multisample_info;
        graphics_pipeline_info.pDepthStencilState = nullptr;
        graphics_pipeline_info.pColorBlendState = &color_blend_state_info;
        graphics_pipeline_info.layout = m_pipeline_layout;
        graphics_pipeline_info.renderPass = m_render_pass;
        graphics_pipeline_info.subpass = 0;
        graphics_pipeline_info.basePipelineHandle = VK_NULL_HANDLE;
        graphics_pipeline_info.basePipelineIndex = -1;

        if (vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &graphics_pipeline_info, nullptr, &m_graphics_pipeline) == VK_SUCCESS)
        {
            throw std::runtime_error("failed to create graphics pipeline!");
        }


    }

    VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
    {
        vkDestroyPipeline(m_device, m_graphics_pipeline, nullptr);
        vkDestroyPipelineLayout(m_device, m_pipeline_layout, nullptr);
        vkDestroyRenderPass(m_device, m_render_pass, nullptr);
        m_vert_shader.reset();
        m_frag_shader.reset();
    }
}