#define GLFW_INCLUDE_VULKAN

#include "runtime/engine.hpp"
#include "runtime/function/render/vulkan/vulkan_rhi.hpp"
#include "runtime/function/window/glfw_window.hpp"
#include "vulkan_graphics_pipeline.hpp"
#include "vulkan_utils.hpp"

#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <set>

namespace Acorn
{
	void VulkanRHI::Init()
	{
		m_window = dynamic_cast<GlfwWindow*>(EngineRuntimeContext::WindowSystem())->GetWindow();
		
#ifndef NDEBUG
        m_enable_validation_layers  = true;
        m_enable_debug_utils_label  = true;
#else
        m_enable_validation_layers  = false;
        m_enable_debug_utils_label  = false;
#endif

		CreateInstance();

		CreateWindowSurface();

		CreateLogicalDevice();

		CreateSwapchain();

		CreateSwapchainImageViews(); 

		CreateCommandPool();

		CreateCommandBuffers();

	}

	void VulkanRHI::Cleanup()
	{
		vkDestroyCommandPool(m_device, m_command_pool, nullptr);

		for (auto swapchain_image_view : m_swapchain_image_views)
		{
			vkDestroyImageView(m_device, swapchain_image_view, nullptr);
		}

		vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);

		vkDestroyDevice(m_device, nullptr);

		if (m_enable_validation_layers)
		{
			DestroyDebugCallback();
		}

		vkDestroySurfaceKHR(m_instance, m_surface, nullptr);

		vkDestroyInstance(m_instance, nullptr);
	}

	GraphicsPipelinePtr VulkanRHI::CreateGraphicsPipeline(GraphicsPipelineStateDescriptor const& descriptor)
	{
		return std::make_unique<VulkanGraphicsPipeline>(descriptor, m_device);
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT,
		VkDebugUtilsMessageTypeFlagsEXT,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void*
	){
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}

	void VulkanRHI::CreateInstance()
	{
		VkApplicationInfo app_info {};
		app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		app_info.pApplicationName   = "Hello Vulkan";
		app_info.pEngineName        = "Acorn Engine";
		app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		app_info.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
		app_info.apiVersion         = VK_API_VERSION_1_0;

		VkInstanceCreateInfo instance_create_info{};
		instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instance_create_info.pApplicationInfo = &app_info;

		auto extensions = GetRequiredExtensions();
		instance_create_info.enabledExtensionCount   = static_cast<uint32_t>(extensions.size());
		instance_create_info.ppEnabledExtensionNames = extensions.data();

		if (m_enable_validation_layers)
		{
			if (!CheckValidationLayerSupport())
			{
				throw std::runtime_error("validation_layers requested, but bot available");
			}

			instance_create_info.enabledLayerCount   = static_cast<uint32_t>(m_validation_layers.size());
			instance_create_info.ppEnabledLayerNames = m_validation_layers.data();

			VkDebugUtilsMessengerCreateInfoEXT debug_create_info {};
			debug_create_info.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debug_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			debug_create_info.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT  | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			debug_create_info.pfnUserCallback = DebugCallback;
			instance_create_info.pNext        = &debug_create_info;
		}
		else
		{
			instance_create_info.enabledLayerCount = 0;
			instance_create_info.pNext = nullptr;
		}

		if (vkCreateInstance(&instance_create_info, nullptr, &m_instance) != VK_SUCCESS)
		{
			throw std::runtime_error("create vk instance failed !");
		}

		SetupDebugMessager();
	}

	void VulkanRHI::CreateWindowSurface()
	{
		
		if (glfwCreateWindowSurface(m_instance, m_window, nullptr, &m_surface) != VK_SUCCESS)
		{
			throw std::runtime_error("create window surface failed");
		}
	}

	void VulkanRHI::CreateLogicalDevice()
	{
		PickPhysicalDevice();

        m_queue_indices = FindQueueFamilies(m_physical_device);

        std::vector<VkDeviceQueueCreateInfo> queue_create_infos; // all queues that need to be created
        uint32_t queue_families[] { m_queue_indices.graphics_family.value(), m_queue_indices.present_family.value() };

        float queue_priority = 1.0f;
        for (uint32_t queue_family : queue_families) // for every queue family
        {
            // queue create info
            VkDeviceQueueCreateInfo queue_create_info {};
            queue_create_info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_create_info.queueFamilyIndex = queue_family;
            queue_create_info.queueCount       = 1;
            queue_create_info.pQueuePriorities = &queue_priority;
            queue_create_infos.push_back(queue_create_info);
        }

        // physical device features
        VkPhysicalDeviceFeatures physical_device_features = {};

        physical_device_features.samplerAnisotropy        = VK_TRUE;
        physical_device_features.fragmentStoresAndAtomics = VK_TRUE;
        physical_device_features.independentBlend         = VK_TRUE;

        // support geometry shader
        //if (m_enable_point_light_shadow)
        //{
        //    physical_device_features.geometryShader = VK_TRUE;
        //}

        // device create info
        VkDeviceCreateInfo device_create_info {};
        device_create_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        device_create_info.pQueueCreateInfos       = queue_create_infos.data();
        device_create_info.queueCreateInfoCount    = static_cast<uint32_t>(queue_create_infos.size());
        device_create_info.pEnabledFeatures        = &physical_device_features;
        device_create_info.enabledExtensionCount   = static_cast<uint32_t>(m_device_extensions.size());
        device_create_info.ppEnabledExtensionNames = m_device_extensions.data();
        device_create_info.enabledLayerCount       = 0;

        if (vkCreateDevice(m_physical_device, &device_create_info, nullptr, &m_device) != VK_SUCCESS)
        {
            throw std::runtime_error("vk create logical device failed");
        }

        // initialize queues of this device
        vkGetDeviceQueue(m_device, m_queue_indices.graphics_family.value(), 0, &m_graphics_queue);
        vkGetDeviceQueue(m_device, m_queue_indices.present_family.value(), 0, &m_present_queue);
	}

	void VulkanRHI::CreateSwapchain()
    {
        SwapChainSupportDetails swapchain_support_details = QuerySwapChainSupport(m_physical_device);

        VkSurfaceFormatKHR chosen_surface_format = ChooseSwapchainSurfaceFormatFromDetails(swapchain_support_details.formats);
        VkPresentModeKHR chosen_presentMode = ChooseSwapchainPresentModeFromDetails(swapchain_support_details.present_modes);
        VkExtent2D chosen_extent = ChooseSwapchainExtentFromDetails(swapchain_support_details.capabilities);

        uint32_t image_count = swapchain_support_details.capabilities.minImageCount + 1;
        if (swapchain_support_details.capabilities.maxImageCount > 0 && image_count > swapchain_support_details.capabilities.maxImageCount)
        {
            image_count = swapchain_support_details.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo {};
        createInfo.sType   = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = m_surface;

        createInfo.minImageCount    = image_count;
        createInfo.imageFormat      = chosen_surface_format.format;
        createInfo.imageColorSpace  = chosen_surface_format.colorSpace;
        createInfo.imageExtent      = chosen_extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        uint32_t queueFamilyIndices[] = { m_queue_indices.graphics_family.value(), m_queue_indices.present_family.value()};

        if (m_queue_indices.graphics_family != m_queue_indices.present_family)
        {
            createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices   = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform   = swapchain_support_details.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode    = chosen_presentMode;
        createInfo.clipped        = VK_TRUE;

        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &m_swapchain) != VK_SUCCESS)
        {
            throw std::runtime_error("vk create swapchain khr failed");
        }

        vkGetSwapchainImagesKHR(m_device, m_swapchain, &image_count, nullptr);
        m_swapchain_images.resize(image_count);
        vkGetSwapchainImagesKHR(m_device, m_swapchain, &image_count, m_swapchain_images.data());

        m_swapchain_image_format = chosen_surface_format.format;
        m_swapchain_extent       = chosen_extent;

        //m_scissor = { {0, 0}, { m_swapchain_extent.width, m_swapchain_extent.height} };
    }

    void VulkanRHI::CreateSwapchainImageViews()
    {
        m_swapchain_image_views.resize(m_swapchain_images.size());

        // create imageview (one for each this time) for all swapchain images
        for (size_t i = 0; i < m_swapchain_images.size(); i++)
        {
            m_swapchain_image_views[i] = VulkanUtil::CreateImageView(
				m_device, m_swapchain_images[i], m_swapchain_image_format,
				VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_VIEW_TYPE_2D, 1, 1);
        }
    }

	void VulkanRHI::CreateCommandPool() 
	{
		if (m_queue_indices.graphics_family.has_value())
		{
            VkCommandPoolCreateInfo command_pool_create_info {};
            command_pool_create_info.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            command_pool_create_info.pNext            = NULL;
            command_pool_create_info.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            command_pool_create_info.queueFamilyIndex = m_queue_indices.graphics_family.value();

            if (vkCreateCommandPool(m_device, &command_pool_create_info, nullptr, &m_command_pool) != VK_SUCCESS)
            {
                throw std::runtime_error("vk create command pool failed");
            }
        }

        // other command pools
        {
            VkCommandPoolCreateInfo command_pool_create_info;
            command_pool_create_info.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            command_pool_create_info.pNext            = NULL;
            command_pool_create_info.flags            = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
            command_pool_create_info.queueFamilyIndex = m_queue_indices.graphics_family.value();

            for (auto index = 0; index < 1; index++)
            {
                if (vkCreateCommandPool(m_device, &command_pool_create_info, NULL, &m_command_pools[index]) != VK_SUCCESS)
                {
                    throw std::runtime_error("vk create command pools failed");
                }
            }
        }
	}

	void VulkanRHI::CreateCommandBuffers()
	{
        VkCommandBufferAllocateInfo command_buffer_allocate_info {};
        command_buffer_allocate_info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        command_buffer_allocate_info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        command_buffer_allocate_info.commandBufferCount = 1U;

        for (auto i = 0; i < 1; i++)
        {
            command_buffer_allocate_info.commandPool = m_command_pools[i];

            if (vkAllocateCommandBuffers(m_device, &command_buffer_allocate_info, &m_command_buffers[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("vk allocate command buffers failed");
            }
        }
	}

	void VulkanRHI::CreateFrameBuffers()
	{
		m_frame_buffers.resize(m_swapchain_image_views.size());

		VkFramebufferCreateInfo frame_buffer_info {};
		frame_buffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		
	}

	std::vector<const char*> VulkanRHI::GetRequiredExtensions()
	{
		uint32_t     glfwExtensionCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (m_enable_validation_layers || m_enable_debug_utils_label)
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
		return extensions;
	}

	bool VulkanRHI::CheckValidationLayerSupport()
	{
		uint32_t layer_count{ 0 };
		vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

		std::vector<VkLayerProperties> available_layers(layer_count);
		vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());
		

		for (auto const layer : m_validation_layers)
		{
			bool is_valid = false;
			for (auto const& layer_prop : available_layers)
			{
				if (strcmp(layer, layer_prop.layerName) == 0)
				{
					return true;
				}
			}
		}
		
		return false;
	}

	void VulkanRHI::SetupDebugMessager() 
	{
		if (m_enable_validation_layers)
		{
			VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};
			debug_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debug_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			debug_create_info.pfnUserCallback = DebugCallback;

			auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT"));
			if (func != nullptr)
			{
				if (func(m_instance, &debug_create_info, nullptr, &m_debug_messager) != VK_SUCCESS)
				{
					throw std::runtime_error("failed to set up debug callback");
				}
			}
			else
			{
				throw std::runtime_error("no debug message func");
			}
		}
	}

	void VulkanRHI::DestroyDebugCallback()
	{
		auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT"));
		if (func != nullptr)
		{
			func(m_instance, m_debug_messager, nullptr);
		}
		else
		{
			throw std::runtime_error("no debug message func");
		}
	}

	void VulkanRHI::PickPhysicalDevice()
	{
		uint32_t physical_device_count;
		vkEnumeratePhysicalDevices(m_instance, &physical_device_count, nullptr);

		if (physical_device_count == 0)
		{
			throw std::runtime_error("enumerate physical devices failed!");
		}
		else
		{
			std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
			vkEnumeratePhysicalDevices(m_instance, &physical_device_count, physical_devices.data());

			std::vector<std::pair<int, VkPhysicalDevice>> ranked_physical_devices;
			for (const auto& device : physical_devices)
			{
				VkPhysicalDeviceProperties physical_device_properties;
				vkGetPhysicalDeviceProperties(device, &physical_device_properties);
				int score = 0;

				if (physical_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
				{
					score += 1000;
				}
				else if (physical_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
				{
					score += 100;
				}

				ranked_physical_devices.push_back({ score, device });
			}

			std::sort(ranked_physical_devices.begin(), ranked_physical_devices.end(),
				[](const std::pair<int, VkPhysicalDevice>& p1, const std::pair<int, VkPhysicalDevice>& p2)
				{
					return p1 > p2;
				}
			);

			for (const auto& device : ranked_physical_devices)
			{
				if (IsDeviceSuitable(device.second))
				{
					m_physical_device = device.second;
					break;
				}
			}

			if (m_physical_device == VK_NULL_HANDLE)
			{
				throw std::runtime_error("failed to find suitable physical device");
			}
		}
	}

	bool VulkanRHI::IsDeviceSuitable(VkPhysicalDevice physical_device)
	{
		auto queue_indices           = FindQueueFamilies(physical_device);
		bool is_extensions_supported = CheckDeviceExtensionSupport(physical_device);
		bool is_swapchain_adequate   = false;

		if (is_extensions_supported)
		{
			SwapChainSupportDetails swapchain_support_details = QuerySwapChainSupport(physical_device);
			is_swapchain_adequate = !swapchain_support_details.formats.empty() && !swapchain_support_details.present_modes.empty();
		}

		VkPhysicalDeviceFeatures physical_device_features;
		vkGetPhysicalDeviceFeatures(physical_device, &physical_device_features);

		if (!queue_indices.IsComplete() || !is_swapchain_adequate || !physical_device_features.samplerAnisotropy)
		{
			return false;
		}

		return true;
	}

	QueueFamilyIndices VulkanRHI::FindQueueFamilies(VkPhysicalDevice physicalm_device) const
	{
		uint32_t queue_family_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalm_device, &queue_family_count, nullptr);

		std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalm_device, &queue_family_count, queue_families.data());

		QueueFamilyIndices indices;
		uint32_t index = 0;
		for (const auto& queue_family : queue_families)
		{
			if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphics_family = index;
			}

			if (queue_family.queueFlags & VK_QUEUE_COMPUTE_BIT)
			{
				indices.compute_family = index;
			}

			VkBool32 is_present_support = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalm_device, index, m_surface, &is_present_support);

			if (is_present_support)
			{
				indices.present_family = index;
			}

			if (indices.IsComplete())
			{
				break;
			}
			index++;
		}
		return indices;
	}

    SwapChainSupportDetails VulkanRHI::QuerySwapChainSupport(VkPhysicalDevice physical_device) const
    {
        SwapChainSupportDetails details_result;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, m_surface, &details_result.capabilities);

        uint32_t format_count;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, m_surface, &format_count, nullptr);
        if (format_count != 0)
        {
            details_result.formats.resize(format_count);
            vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, m_surface, &format_count, details_result.formats.data());
        }

        uint32_t presentmode_count;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, m_surface, &presentmode_count, nullptr);
        if (presentmode_count != 0)
        {
            details_result.present_modes.resize(presentmode_count);
            vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, m_surface, &presentmode_count, details_result.present_modes.data());
        }

        return details_result;
    }

    bool VulkanRHI::CheckDeviceExtensionSupport(VkPhysicalDevice physical_device)
    {
        uint32_t extension_count;
        vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, nullptr);

        std::vector<VkExtensionProperties> available_extensions(extension_count);
        vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, available_extensions.data());

        std::set<std::string> required_extensions(m_device_extensions.begin(), m_device_extensions.end());
        for (const auto& extension : available_extensions)
        {
            required_extensions.erase(extension.extensionName);
        }

        return required_extensions.empty();
    }

	VkSurfaceFormatKHR VulkanRHI::ChooseSwapchainSurfaceFormatFromDetails(const std::vector<VkSurfaceFormatKHR>& available_surface_formats)
	{
        for (const auto& surface_format : available_surface_formats)
        {
            if (surface_format.format == VK_FORMAT_B8G8R8A8_UNORM && surface_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return surface_format;
            }
        }
        return available_surface_formats[0];
    }

    VkPresentModeKHR VulkanRHI::ChooseSwapchainPresentModeFromDetails(const std::vector<VkPresentModeKHR>& available_present_modes)
	{
        for (VkPresentModeKHR present_mode : available_present_modes)
        {
            if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return VK_PRESENT_MODE_MAILBOX_KHR;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VulkanRHI::ChooseSwapchainExtentFromDetails(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        if (capabilities.currentExtent.width != UINT32_MAX)
        {
            return capabilities.currentExtent;
        }
        else
        {
            int width, height;
            glfwGetFramebufferSize(m_window, &width, &height);

            VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

            actualExtent.width  = std::clamp(actualExtent.width,  capabilities.minImageExtent.width,  capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

}