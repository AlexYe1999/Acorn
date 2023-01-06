#pragma once

#include "runtime/function/render/rhi.hpp"
#include "3rdparty/glfw/include/GLFW/glfw3.h"

#include <vulkan/vulkan.h>
#include <optional>
#include <vector>

namespace Acorn
{
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;
        std::optional<uint32_t> compute_family;

        bool IsComplete()
        {
            return graphics_family.has_value() && present_family.has_value() && compute_family.has_value();
        }
    };

    class VulkanRHI : public RHI
    {
    public:
        
        virtual void Initialize() override;
        virtual void Shutdown()   override;

    private:
        void CreateInstance();
        void CreateWindowSurface();
        void CreatePhysicalDevice();

    private:
        std::vector<const char*> GetRequiredExtensions();

        bool CheckValidationLayerSupport();
        
        bool SetupDebugCallback(VkDebugUtilsMessengerCreateInfoEXT& debug_info);
        void DestroyDebugCallback();

        bool IsDeviceSuitable(VkPhysicalDevice physical_device) const;
    
    private:
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice physicalm_device) const;


    private:
        bool m_enable_validation_layers{ true };
        bool m_enable_debug_utils_label{ true };

        const std::vector<char const*> m_validation_layers{ "VK_LAYER_KHRONOS_validation" };

        VkInstance               m_instance        { nullptr };
        VkSurfaceKHR             m_surface         { nullptr };
        VkPhysicalDevice         m_physical_device { nullptr };
        VkDebugUtilsMessengerEXT m_debug_messager  { nullptr };
    };
}