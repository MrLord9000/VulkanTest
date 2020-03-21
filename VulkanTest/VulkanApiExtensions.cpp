#include "VulkanApiImplementation.hpp"

bool VulkanApi::checkRequiredExtensionsAvailability(bool verbose = false)
{
	// Preparing for get required function
	uint32_t requiredExtensionCount = 0;
	const char** requiredExtensions;

	// Getting all the required instance extensions
	requiredExtensions = glfwGetRequiredInstanceExtensions(&requiredExtensionCount);


	if (verbose)
	{
		// Printing out the required extensions
		std::cout << "Required extensions (" << requiredExtensionCount << "):\n";

		for (uint32_t i = 0; i < requiredExtensionCount; i++)
		{
			std::cout << "\t" << *(requiredExtensions + i) << std::endl;
		}
	}

	// Preparing for enumerate extension function
	uint32_t availableExtensionCount = 0; // 32 bit int for storing extension count
	vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr); // Function for retrieving a list of supported extensions and their number

	std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount); // Creating vector of extension properties details

	// Getting all the extension properties and saving them to the extensions vector
	vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions.data());

	if (verbose)
	{
		// Printing out all the available extensions
		std::cout << "Available extensions (" << availableExtensionCount << "):\n";

		// Printing out all the extension names
		// Notice how the for each loop is created with const, auto and & address reference
		for (const auto& extension : availableExtensions)
		{
			std::cout << "\t" << extension.extensionName << std::endl;
		}
	}

	// Now that we have all the required and available extensions, 
	// we should check if all the required are in the available, and throw an exception otherwise
	for (uint32_t i = 0; i < requiredExtensionCount; ++i)
	{
		bool contains = false;
		std::string currExtName = *(requiredExtensions + i);
		for (const auto& availableExtension : availableExtensions)
		{
			if (availableExtension.extensionName == currExtName)
			{
				contains = true;
				break;
			}
		}

		if (contains == false)
		{
			return false;
		}
	}

	return true;
}

// Returns vector of required extension names based on whether the validation layers are enabled or not
std::vector<const char*> VulkanApi::getRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0; // Extension count variable, pretty standard
	const char** glfwExtensions; // Double pointer for storing estension names in c-strings

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount); // Getting BASE extension names and count

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount); // Saving pointers to extension names to const char* vector

	if (enableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); // Getting and saving VALIDATION layers extension name
		// This VK_EXT_DEBUG_UTILS_EXTENSION_NAME macro is equal to VK_EXT_debug_utils
	}

	return extensions;
}