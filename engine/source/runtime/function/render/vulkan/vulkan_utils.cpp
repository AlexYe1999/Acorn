#include "vulkan_utils.hpp"

#include <stdexcept>

namespace Acorn
{
    namespace VulkanUtils
    {
        VkImageView CreateImageView(
            VkDevice device, VkImage& image, VkFormat format,
            VkImageAspectFlags image_aspect_flags, VkImageViewType view_type,
            uint32_t layout_count, uint32_t miplevels
        ){
            VkImageViewCreateInfo image_view_create_info{};
            image_view_create_info.sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            image_view_create_info.image    = image;
            image_view_create_info.viewType = view_type;
            image_view_create_info.format   = format;
            image_view_create_info.subresourceRange.aspectMask     = image_aspect_flags;
            image_view_create_info.subresourceRange.baseMipLevel   = 0;
            image_view_create_info.subresourceRange.levelCount     = miplevels;
            image_view_create_info.subresourceRange.baseArrayLayer = 0;
            image_view_create_info.subresourceRange.layerCount     = layout_count;

            VkImageView image_view;
            if (vkCreateImageView(device, &image_view_create_info, nullptr, &image_view) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create image view");
            }

            return image_view;
        }

    }
} 
