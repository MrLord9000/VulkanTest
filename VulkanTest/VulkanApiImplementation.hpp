#ifndef VULKAN_API_IMPLEMENTATION
#define VULKAN_API_IMPLEMENTATION

#define GLFW_INCLUDE_VULKAN // This define includes vulkan headers by glfw3 automatically
#include <GLFW/glfw3.h>

// For reporting and propagating errors
#include <iostream>
#include <stdexcept>
// For lambda functions
#include <functional>
// EXIT_SUCCESS and EXIT_FAILURE macros
#include <cstdlib>
#include <vector>
#include <string>
#include <map>
#include <optional>
#include <set>
#include <algorithm>
#include <fstream>

const int WIDTH = 800;
const int HEIGHT = 600;

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

// Validation layers vectors ======================
const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};


// Utility structures =============================
struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};


// Main class ====================================
class VulkanApi
{
public:
	void run()
	{
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	GLFWwindow* window; // Main glfw window handle

	VkInstance instance; // Main Vulkan instance
	VkDebugUtilsMessengerEXT debugMessenger; // Main debug callback messenger
	VkSurfaceKHR surface; // Surface handle member

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; // Physical device object
	VkDevice device; // Logical device handle is stored here

	VkQueue graphicsQueue; // Graphics queue handle goes here
	VkQueue presentQueue;

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	std::vector<VkImageView> swapChainImageViews;
	VkRenderPass renderPass;
	VkPipelineLayout pipelineLayout;

	VkPipeline graphicsPipeline;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;

	// Member function prototypes
	
	// ==== SETUP ====
	void drawFrame();
	void createInstance();
	// ==== EXTENSIONS ====
	bool checkRequiredExtensionsAvailability(bool verbose = false);
	std::vector<const char*> getRequiredExtensions();
	// ==== VALIDATION LAYERS ====
	bool checkValidationLayerSupport();
	
	// Initialization, main loop and cleanup
	
	void initWindow()
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Prevent the glfw from loading OpenGL libraries
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Prevent the window from resizing - because it takes some more attention to do it properly

		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr); // Creating the main application window
	}

	void initVulkan()
	{
		createInstance();
		setupDebugMessenger();
		createSurface();
		pickPhysicalDevice();
		createLogicalDevice();
		createSwapChain();
		createImageViews();
		createRenderPass();
		createGraphicsPipeline();
		createFramebuffers();
		createCommandPool();
		createCommandBuffers();
		createSemaphores();
	}

	void mainLoop()
	{
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
			drawFrame();
		}

		vkDeviceWaitIdle(device);
		/// LAST CHECKPOINT: Frames in flight
	}

	void cleanup()
	{
		vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
		vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);

		vkDestroyCommandPool(device, commandPool, nullptr);

		// Destroying the framebuffers
		for (auto framebuffer : swapChainFramebuffers)
		{
			vkDestroyFramebuffer(device, framebuffer, nullptr);
		}

		// Destroy the pipeline
		vkDestroyPipeline(device, graphicsPipeline, nullptr);

		// Destroy the pipeline layout
		vkDestroyPipelineLayout(device, pipelineLayout, nullptr);

		// Destroy the render pass
		vkDestroyRenderPass(device, renderPass, nullptr);

		// Destroy created image views
		for (auto imageView : swapChainImageViews)
		{
			vkDestroyImageView(device, imageView, nullptr);
		}

		// Destory the swap chain, must be before the device destruction
		vkDestroySwapchainKHR(device, swapChain, nullptr);

		// Destroy the logical device
		vkDestroyDevice(device, nullptr);

		if (enableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
		}

		// Destroy the window surface, must be done before instance destruction
		vkDestroySurfaceKHR(instance, surface, nullptr);

		// Destroy the instance we created in create instance function
		vkDestroyInstance(instance, nullptr);

		glfwDestroyWindow(window);

		glfwTerminate();
	}
};

#endif
