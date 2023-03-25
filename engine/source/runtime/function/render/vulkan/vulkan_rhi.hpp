#pragma once

#include "3rdparty/glfw/include/GLFW/glfw3.h"
#include "runtime/function/render/rhi.hpp"

#include <vulkan/vulkan.hpp>
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

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR        capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR>   present_modes;
    };

    class VulkanRHI : public RHI
    {
    public:
         
        virtual void Init()    override;
        virtual void Cleanup() override;

        virtual GraphicsPipelinePtr CreateGraphicsPipeline(GraphicsPipelineStateDescriptor const& descriptor) override;

    private:
        void CreateInstance();
        void CreateWindowSurface();
        void CreateLogicalDevice();
        void CreateSwapchain();
        void CreateSwapchainImageViews();
        void CreateCommandPool();
        void CreateCommandBuffers();
        void CreateFrameBuffers();
        
    private:
        std::vector<char const *> GetRequiredExtensions();

        bool CheckValidationLayerSupport();
        void SetupDebugMessager();
        void DestroyDebugCallback();

        void PickPhysicalDevice();
        bool IsDeviceSuitable(VkPhysicalDevice physical_device);

    private:
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice physicalm_device) const;
        SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice physical_device) const;
        bool CheckDeviceExtensionSupport(VkPhysicalDevice physical_device);

        VkSurfaceFormatKHR ChooseSwapchainSurfaceFormatFromDetails(std::vector<VkSurfaceFormatKHR> const &available_surface_formats);
        VkPresentModeKHR ChooseSwapchainPresentModeFromDetails(std::vector<VkPresentModeKHR> const &available_present_modes);
        VkExtent2D ChooseSwapchainExtentFromDetails(VkSurfaceCapabilitiesKHR const &capabilities);

    private:
        bool m_enable_validation_layers { true };
        bool m_enable_debug_utils_label { true };

        const std::vector<char const *> m_validation_layers { "VK_LAYER_KHRONOS_validation" };
        const std::vector<char const *> m_device_extensions { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

        QueueFamilyIndices         m_queue_indices   {};

        GLFWwindow*                m_window          { nullptr };
        VkInstance                 m_instance        { nullptr };
        VkSurfaceKHR               m_surface         { nullptr };
        VkPhysicalDevice           m_physical_device { nullptr };
        VkDevice                   m_device          { nullptr };
        VkDebugUtilsMessengerEXT   m_debug_messager  { nullptr };

        VkQueue                    m_graphics_queue  { nullptr };
        VkQueue                    m_present_queue   { nullptr };
        VkSwapchainKHR             m_swapchain       { nullptr };

        VkFormat                   m_swapchain_image_format;
        VkExtent2D                 m_swapchain_extent;
        std::vector<VkImage>       m_swapchain_images;
        std::vector<VkImageView>   m_swapchain_image_views;
        std::vector<VkFramebuffer> m_frame_buffers;

        using VkCommandPools   = std::vector<VkCommandPool>;
        using VkCommandBuffers = std::vector<VkCommandBuffer>;

        VkCommandPool            m_command_pool    {};
        VkCommandPools           m_command_pools   {};
        VkCommandBuffers         m_command_buffers {};
    };
}