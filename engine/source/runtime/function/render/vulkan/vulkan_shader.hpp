#pragma once

#include "runtime/function/render/shader.hpp"

#include <vulkan/vulkan.h>

namespace Acorn
{
    class VulkanShader : public Shader
    {
    public:
        VulkanShader(ShaderDescriptor const& descriptor, VkDevice device);
        ~VulkanShader();

        VkPipelineShaderStageCreateInfo ShaderStageInfo();

    private:
        VkDevice       m_device        { nullptr };
        VkShaderModule m_shader_module { nullptr };
    };
} 