#include "VulkanApiImplementation.hpp"

bool VulkanApi::checkValidationLayerSupport()
{
	uint32_t layerCount; // Create a variable for storing the available validation layers count
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr); // Get the number of available validation layers

	std::vector<VkLayerProperties> availableLayers(layerCount); // Create vector of layer properties

	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()); // Fill the vector with layer properties data

	for (const char* layerName : validationLayers)
	{
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (layerFound == false)
		{
			return false;
		}
	}

	return true;
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT; // Standard double type enumeration
	// Setting which message severity types we'd like to be called for.
	// Here we set it to all except INFO_BIT, to leave out general debug info
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	// Setting which message types we'd like to receive
	// Here we set it to all but it can be changed to our liking
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	// Specifying pointer to the callback function (from the class field here)
	createInfo.pfnUserCallback = debugCallback;
	createInfo.pUserData = nullptr; // Optional, can be used to pass a pointer to the HelloTriangleApplication

	// See extension specification for more info about configuration of debug messenger
	// https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VK_EXT_debug_utils
}