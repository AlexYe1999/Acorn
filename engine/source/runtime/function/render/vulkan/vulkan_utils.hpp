#pragma once

#include "runtime/function/render/graphics_pipeline.hpp"

#include <vulkan/vulkan.h>

namespace Acorn
{
    namespace VulkanUtil
    {
        VkImageView CreateImageView(
            VkDevice device, VkImage& image, VkFormat format, 
            VkImageAspectFlags image_aspect_flags, VkImageViewType view_type,
            uint32_t layout_count, uint32_t miplevels);
    }
}