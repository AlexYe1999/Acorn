#pragma once

#include "runtime/function/render/struct.hpp"

#include <stdexcept>
#include <vulkan/vulkan.h>

namespace Acorn
{
    namespace VulkanEnum
    {
        VkPrimitiveTopology PrimitiveTopology(INPUT_PRIMITIVE_TOPOLOGY topology)
        {
            switch (topology)
            {
                case INPUT_PRIMITIVE_TOPOLOGY::POINT_LIST:
                {
                    return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
                }
                case INPUT_PRIMITIVE_TOPOLOGY::LINE_LIST:
                {
                    return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
                }
                case INPUT_PRIMITIVE_TOPOLOGY::LINE_STRIP:
                {
                    return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
                }
                case INPUT_PRIMITIVE_TOPOLOGY::TRIANGLE_LIST:
                {
                    return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
                }
                case INPUT_PRIMITIVE_TOPOLOGY::TRIANGLE_STRIP:
                {
                    return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
                }
                default:
                {
                    throw std::runtime_error("invalid primitive topology");
                    return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
                }
            }
        }

        VkCullModeFlagBits CullMode(RASTER_CULL_MODE mode)
        {
            switch (mode)
            {
                case RASTER_CULL_MODE::ALL:
                {
                    return VkCullModeFlagBits::VK_CULL_MODE_FRONT_AND_BACK;
                }
                case RASTER_CULL_MODE::FRONT:
                {
                    return VkCullModeFlagBits::VK_CULL_MODE_FRONT_BIT;
                }
                case RASTER_CULL_MODE::BACK:
                {
                    return VkCullModeFlagBits::VK_CULL_MODE_BACK_BIT;
                }
                case RASTER_CULL_MODE::NONE:
                {
                    return VkCullModeFlagBits::VK_CULL_MODE_NONE;
                }
                default:
                {
                    throw std::runtime_error("invalid cull mode");
                    return VkCullModeFlagBits::VK_CULL_MODE_FLAG_BITS_MAX_ENUM;
                }
            }
        }

        VkColorComponentFlags ColorComponetMask(RENDER_TARGET_WRITE_MASK mask)
        {
            VkColorComponentFlags vk_mask {};

            if (static_cast<std::uint8_t>(mask) | static_cast<std::uint8_t>(RENDER_TARGET_WRITE_MASK::R))
            {
                vk_mask |= VK_COLOR_COMPONENT_R_BIT;
            }

            if (static_cast<std::uint8_t>(mask) | static_cast<std::uint8_t>(RENDER_TARGET_WRITE_MASK::G))
            {
                vk_mask |= VK_COLOR_COMPONENT_G_BIT;
            }

            if (static_cast<std::uint8_t>(mask) | static_cast<std::uint8_t>(RENDER_TARGET_WRITE_MASK::B))
            {
                vk_mask |= VK_COLOR_COMPONENT_B_BIT;
            }

            if (static_cast<std::uint8_t>(mask) | static_cast<std::uint8_t>(RENDER_TARGET_WRITE_MASK::A))
            {
                vk_mask |= VK_COLOR_COMPONENT_A_BIT;
            }
            
            return vk_mask;
        }

        VkDynamicState DynamicState(DYNAMIC_STATE state)
        {
            switch (state)
            {
                case Acorn::DYNAMIC_STATE::VIEW_PORT:
                {
                    return VkDynamicState::VK_DYNAMIC_STATE_VIEWPORT;
                }
                case Acorn::DYNAMIC_STATE::STATE_LINE_WIDTH:
                {
                    return VkDynamicState::VK_DYNAMIC_STATE_LINE_WIDTH;
                }
                default:
                {
                    throw std::runtime_error("invalid dynamic state");
                    return VkDynamicState::VK_DYNAMIC_STATE_MAX_ENUM;
                }
            }
        }
    }

    namespace VulkanStruct
    {

    }
}