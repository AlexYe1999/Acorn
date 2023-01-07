
#include "runtime/function/render/rhi/vulkan/vulkan_rhi.hpp"

#include <iostream>
#include <stdexcept>
#include <algorithm>

namespace Acorn
{
	void VulkanRHI::Initialize()
	{
		CreateInstance();

		CreatePhysicalDevice();
	}

	void VulkanRHI::Shutdown()
	{
		if (m_enable_validation_layers)
		{
			DestroyDebugCallback();
		}

		vkDestroyInstance(m_instance, nullptr);
	}

	void VulkanRHI::CreateInstance()
	{
		VkApplicationInfo app_info {};
		app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
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
				throw std::runtime_error("validation_layers requested, but bot available!");
			}

			VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
			SetupDebugCallback(debugCreateInfo);

			instance_create_info.enabledLayerCount   = static_cast<uint32_t>(m_validation_layers.size());
			instance_create_info.ppEnabledLayerNames = m_validation_layers.data();
			instance_create_info.pNext               = &debugCreateInfo;

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
	}

	void VulkanRHI::CreateWindowSurface()
	{
		if (glfwCreateWindowSurface(m_instance, m_window, nullptr, &m_surface) != VK_SUCCESS)
		{
			throw std::runtime_error("glfwCreateWindowSurface failed!");
		}
	}

	void VulkanRHI::CreatePhysicalDevice()
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

	bool VulkanRHI::SetupDebugCallback(VkDebugUtilsMessengerCreateInfoEXT& debug_info) 
	{
		debug_info.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debug_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debug_info.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT  | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debug_info.pfnUserCallback = DebugCallback;

		auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT"));
		if (func != nullptr)
		{
			if (func(m_instance, &debug_info, nullptr, &m_debug_messager) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to set up debug callback");
			}
		}
		else
		{
			throw std::runtime_error("no debug message func");
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

	bool VulkanRHI::IsDeviceSuitable(VkPhysicalDevice physical_device) const
	{
		auto queue_indices           = FindQueueFamilies(physical_device);
		bool is_extensions_supported = checkDeviceExtensionSupport(physical_device);
		bool is_swapchain_adequate   = false;

		if (is_extensions_supported)
		{
			SwapChainSupportDetails swapchain_support_details = querySwapChainSupport(physical_device);
			is_swapchain_adequate =
				!swapchain_support_details.formats.empty() && !swapchain_support_details.presentModes.empty();
		}

		VkPhysicalDeviceFeatures physicalm_device_features;
		vkGetPhysicalDeviceFeatures(physical_device, &physicalm_device_features);

		if (!queue_indices.isComplete() || !is_swapchain_adequate || !physicalm_device_features.samplerAnisotropy)
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

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT,
		VkDebugUtilsMessageTypeFlagsEXT,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void*)
	{
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}

}