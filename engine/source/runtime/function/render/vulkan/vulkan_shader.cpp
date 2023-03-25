#include "runtime/function/utils/misc.hpp"
#include "vulkan_shader.hpp"


#include <stdexcept>

namespace Acorn
{
    VulkanShader::VulkanShader(ShaderDescriptor const& descriptor, VkDevice device)
        : Shader { descriptor }
    {
		auto buffer = ReadFromFile(descriptor.shader_path.c_str());

		VkShaderModuleCreateInfo create_info {};
		create_info.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		create_info.codeSize = buffer.size();
		create_info.pCode    = reinterpret_cast<const uint32_t*>(buffer.data());

		VkShaderModule shader_module;
		if (vkCreateShaderModule(m_device, &create_info, nullptr, &shader_module) != VK_SUCCESS)
		{
			throw std::runtime_error("create shader failed");
		}
    }

    VulkanShader::~VulkanShader()
    {
		vkDestroyShaderModule(m_device, m_shader_module, nullptr);
    }

	VkPipelineShaderStageCreateInfo VulkanShader::ShaderStageInfo()
	{
		VkPipelineShaderStageCreateInfo shader_create_info {};
		shader_create_info.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shader_create_info.module = m_shader_module;
		shader_create_info.pName  = m_descriptor.shader_entry.c_str();

		switch (m_descriptor.shader_type)
		{
			case SHADER_TYPE::VERTEX_SHADER:
			{
				shader_create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
				break;
			}
			case SHADER_TYPE::FRAGMENT_SHADER:
			{
				shader_create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
				break;
			}
			default:
			{
				throw std::runtime_error("invalid shader type");
			}
		}
		return shader_create_info;
	}
}