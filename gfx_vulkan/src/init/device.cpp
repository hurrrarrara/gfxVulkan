#include <gfx_vulkan/Device.h>

#include <set>
#include <unordered_map>

namespace gfx {

void Device::createDevice() {
	// device features
	vk::StructureChain<vk::PhysicalDeviceFeatures2,
					   vk::PhysicalDeviceVulkan11Features,
					   vk::PhysicalDeviceVulkan12Features,
					   vk::PhysicalDeviceVulkan13Features,
					   vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>
		features = {
					{.features = vk::PhysicalDeviceFeatures{
									.robustBufferAccess = true,
									.geometryShader = true,
									.fillModeNonSolid = true,
									.shaderInt64 = true,
					}},
					{.shaderDrawParameters = true},
					{
						// Enable non uniform array indexing
						//  = #extension GL_EXT_nonuniform_qualifier : require,
						.shaderSampledImageArrayNonUniformIndexing = true,
						.shaderStorageBufferArrayNonUniformIndexing = true,
						.shaderStorageImageArrayNonUniformIndexing = true,
						// All of these enables to update after the
						// commandbuffer used the bindDescriptors
						.descriptorBindingSampledImageUpdateAfterBind = true,
						.descriptorBindingStorageImageUpdateAfterBind = true,
						.descriptorBindingStorageBufferUpdateAfterBind = true,
						// Enable non bound descriptors slots
						.descriptorBindingPartiallyBound = true,
						// Enable non sized arrays
						.runtimeDescriptorArray = true,

						.bufferDeviceAddress = true,
					},
					{
						.synchronization2 = true,
						.dynamicRendering = true,
					},
					{.extendedDynamicState = true},
					};

	// device queues
	// for now we consider that the present queue is the same as the graphics queue

	std::unordered_map<uint32_t, std::set<uint32_t>> queueFamilies;
	queueFamilies[_GraphicsQueueInfo.family].insert(_GraphicsQueueInfo.index);
	queueFamilies[_TransferQueueInfo.family].insert(_TransferQueueInfo.index);
	queueFamilies[_PresentQueueInfo.family].insert(_PresentQueueInfo.index);
	queueFamilies[_ComputeQueueInfo.family].insert(_ComputeQueueInfo.index);

	// std::cout << "Queues: family | index"
	// 		  << "\nGraphics: " << _GraphicsQueueInfo.family << " | " << mGraphicsQueueInfo.index << "\nTransfer: " << _TransferQueueInfo.family << " | " << _TransferQueueInfo.index
	// 		  << "\nPresent: " << _PresentQueueInfo.family << " | " << _PresentQueueInfo.index << "\nCompute: " << _ComputeQueueInfo.family << " | " << _ComputeQueueInfo.index << std::endl;

	std::vector<vk::DeviceQueueCreateInfo> queuesCreateInfo;
	float queuePriority[] = {1.0f, 1.0f, 1.0f, 1.0f};
	// std::cout << "QueueCreateInfo: family | count \n";
	for (auto& queueFamily : queueFamilies) {
		vk::DeviceQueueCreateInfo queueCreateInfo{.queueFamilyIndex = queueFamily.first, .queueCount = static_cast<uint32_t>(queueFamily.second.size()), .pQueuePriorities = queuePriority};
		queuesCreateInfo.push_back(queueCreateInfo);
		// std::cout << queueCreateInfo.queueFamilyIndex << " | " << queueCreateInfo.queueCount << std::endl;
	}

	// device
	vk::DeviceCreateInfo deviceCreateInfo = {.pNext = features.get<vk::PhysicalDeviceFeatures2>(),
											 .queueCreateInfoCount = static_cast<uint32_t>(queuesCreateInfo.size()),
											 .pQueueCreateInfos = queuesCreateInfo.data(),
											 .enabledExtensionCount = static_cast<uint32_t>(_RequiredDeviceExtensions.size()),
											 .ppEnabledExtensionNames = _RequiredDeviceExtensions.data()};

	_Device = _PhysicalDevice.createDevice(deviceCreateInfo);

	_GraphicsQueue = _Device.getQueue(_GraphicsQueueInfo.family, _GraphicsQueueInfo.index);
	_PresentQueue = _Device.getQueue(_PresentQueueInfo.family, _PresentQueueInfo.index);
	_TransferQueue = _Device.getQueue(_TransferQueueInfo.family, _TransferQueueInfo.index);
	_ComputeQueue = _Device.getQueue(_ComputeQueueInfo.family, _ComputeQueueInfo.index);

#ifndef NDEBUG
	LOG_INFO_MSG("Vulkan device and queues created");
	LOG_VERBOSE_MSG(" Graphics queue: family " + std::to_string(_GraphicsQueueInfo.family) + ", index " + std::to_string(_GraphicsQueueInfo.index))
	LOG_VERBOSE_MSG(" Present  queue: family " + std::to_string(_PresentQueueInfo.family) + ", index " + std::to_string(_PresentQueueInfo.index))
	LOG_VERBOSE_MSG(" Transfer queue: family " + std::to_string(_TransferQueueInfo.family) + ", index " + std::to_string(_TransferQueueInfo.index))
	LOG_VERBOSE_MSG(" Compute  queue: family " + std::to_string(_ComputeQueueInfo.family) + ", index " + std::to_string(_ComputeQueueInfo.index))
#endif
}

} // namespace gfx::vulkan
