#include <gfx_vulkan/Device.h>
#include "utils.h"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace gfx {
void Device::createInstance() {
	vk::detail::DynamicLoader dl;
	auto vkGetInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
	VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

	constexpr vk::ApplicationInfo appInfo = vk::ApplicationInfo{.pApplicationName = "GFX Vulkan",
																.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
																.pEngineName = "No Name",
																.engineVersion = VK_MAKE_VERSION(1, 0, 0),
																.apiVersion = vk::ApiVersion13};

	// Check if the required layers are supported by the Vulkan Implementation
	auto layerResult = checkRequiredLayers(_RequiredInstanceLayers, vk::enumerateInstanceLayerProperties());
	if (layerResult != nullptr)
		throw std::runtime_error("Required layers not supported: " + std::string(layerResult));

	// Check if required extensions are supported by the Vulkan Implementation
	auto extensionResult = checkRequiredExtensions(_RequiredInstanceExtensions, vk::enumerateInstanceExtensionProperties());
	if (extensionResult != nullptr)
		throw std::runtime_error("Required extension not supported: " + std::string(extensionResult));

	vk::InstanceCreateInfo createInfo{.pApplicationInfo = &appInfo,
									  .enabledLayerCount = static_cast<uint32_t>(_RequiredInstanceLayers.size()),
									  .ppEnabledLayerNames = _RequiredInstanceLayers.data(),
									  .enabledExtensionCount = static_cast<uint32_t>(_RequiredInstanceExtensions.size()),
									  .ppEnabledExtensionNames = _RequiredInstanceExtensions.data()};
	_Instance = vk::createInstance(createInfo);
	VULKAN_HPP_DEFAULT_DISPATCHER.init(_Instance);

#ifndef NDEBUG
	LOG_INFO_MSG("Vulkan instance created");
#endif

	// Create Debug Messenger
#ifndef NDEBUG
	vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
														vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
	vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
													   vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);

	vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT{.messageSeverity = severityFlags, .messageType = messageTypeFlags, .pfnUserCallback = &debugCallback};
	_DebugMessenger = _Instance.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT);
#endif
}

} // namespace gfx::vulkan
