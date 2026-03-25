#include <gfx_vulkan/Device.h>
#include "utils.h"

namespace gfx {

void Device::pickPhysicalDevice() {
	std::vector<vk::PhysicalDevice> devices = _Instance.enumeratePhysicalDevices();

	if (devices.empty())
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
#ifndef NDEBUG
	LOG_VERBOSE_MSG("Available GPUs: ");
	for (auto& d : devices) LOG_VERBOSE_MSG(d.getProperties().deviceName);
#endif

	for (vk::PhysicalDevice& device : devices) {
		// LOG_WARNING_MSG("Device: " + std::string(device.getProperties().deviceName));
		// LOG_INFO_MSG("Prop: " + std::to_string(checkPhysicalDeviceProperties(device)));
		// LOG_INFO_MSG("Ext: " + std::to_string(checkPhysicalDeviceExtensions(device)));
		// LOG_INFO_MSG("Feat: " + std::to_string(checkPhysicalDeviceFeatures(device)));
		// LOG_INFO_MSG("Queue: " + std::to_string(checkPhysicalDeviceQueues(device)));
		if (!(checkPhysicalDeviceProperties(device) && checkPhysicalDeviceExtensions(device) && checkPhysicalDeviceFeatures(device) && checkPhysicalDeviceQueues(device)))
			continue;
		_PhysicalDevice = device;
		break;
	}

	if (_PhysicalDevice == nullptr)
		throw std::runtime_error("failed to find a suitable GPU!");

	// _PhysicalDeviceMemoryProperties = _PhysicalDevice.getMemoryProperties();

#ifndef NDEBUG
	auto properties = _PhysicalDevice.getProperties();
	LOG_INFO_MSG("GPU Selected: " + std::string(properties.deviceName));
	LOG_VERBOSE_MSG(" API version: " + std::to_string(vk::apiVersionMajor(properties.apiVersion)) + "." + std::to_string(vk::apiVersionMinor(properties.apiVersion)) + "." +
					std::to_string(vk::apiVersionPatch(properties.apiVersion)));
	LOG_VERBOSE_MSG(" type: " + vk::to_string(properties.deviceType));
	auto queueFamilies = _PhysicalDevice.getQueueFamilyProperties();
	LOG_VERBOSE_MSG(" queue families: " + std::to_string(queueFamilies.size()));
	for (uint32_t i = 0; i < queueFamilies.size(); i++)
		LOG_VERBOSE_MSG("  -family " + std::to_string(i + 1) + ": " + std::to_string(queueFamilies[i].queueCount) + " queues with " + vk::to_string(queueFamilies[i].queueFlags) + " support");
#endif
}

bool Device::checkPhysicalDeviceProperties(vk::PhysicalDevice& device) {
	const vk::PhysicalDeviceProperties properties = device.getProperties();

	// return properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu && properties.apiVersion >= vk::ApiVersion13;
	return properties.apiVersion >= vk::ApiVersion13;
}

bool Device::checkPhysicalDeviceExtensions(vk::PhysicalDevice& device) { return checkRequiredExtensions(_RequiredDeviceExtensions, device.enumerateDeviceExtensionProperties()) == nullptr; }

bool Device::checkPhysicalDeviceFeatures(vk::PhysicalDevice& device) {
	auto features = device.template getFeatures2<vk::PhysicalDeviceFeatures2,
												 vk::PhysicalDeviceVulkan11Features,
												 vk::PhysicalDeviceVulkan12Features,
												 vk::PhysicalDeviceVulkan13Features,
												 vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT,
												 vk::PhysicalDeviceDescriptorIndexingFeatures>();

	auto vk11Features = features.template get<vk::PhysicalDeviceVulkan11Features>();
	auto vk12Features = features.template get<vk::PhysicalDeviceVulkan12Features>();
	auto vk13Features = features.template get<vk::PhysicalDeviceVulkan13Features>();
	auto vkDynamicStateFeatures = features.template get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();
	auto vkIndexingFeatures = features.template get<vk::PhysicalDeviceDescriptorIndexingFeatures>();

	return	vk11Features.shaderDrawParameters &&
			vk12Features.bufferDeviceAddress &&
			vk13Features.synchronization2 && vk13Features.dynamicRendering && 
			vkDynamicStateFeatures.extendedDynamicState &&
			// Enable non uniform array indexing
			//  = #extension GL_EXT_nonuniform_qualifier : require,
			vkIndexingFeatures.shaderSampledImageArrayNonUniformIndexing && vkIndexingFeatures.shaderStorageBufferArrayNonUniformIndexing &&
			vkIndexingFeatures.shaderStorageImageArrayNonUniformIndexing &&
			// All of these enables to update after the
			// commandbuffer used the bindDescriptors
			vkIndexingFeatures.descriptorBindingSampledImageUpdateAfterBind && vkIndexingFeatures.descriptorBindingStorageBufferUpdateAfterBind &&
			vkIndexingFeatures.descriptorBindingStorageImageUpdateAfterBind &&
			// Enable non bound descriptors slots
			vkIndexingFeatures.descriptorBindingPartiallyBound &&
			// Enable non sized arrays
			vkIndexingFeatures.runtimeDescriptorArray;
}

bool Device::checkPhysicalDeviceQueues(vk::PhysicalDevice& device) {
	findGraphicsQueue(device);
	findTransferQueue(device);
	findPresentQueue(device);
	findComputeQueue(device);

	// LOG_WARNING_MSG("GR: " + std::to_string(mGraphicsQueueInfo.family));
	// LOG_WARNING_MSG("TR: " + std::to_string(mTransferQueueInfo.family));
	// LOG_WARNING_MSG("CP: " + std::to_string(mComputeQueueInfo.family));
	// LOG_WARNING_MSG("PR: " + std::to_string(mPresentQueueInfo.family));

	return _GraphicsQueueInfo.family != U32_ERROR && _TransferQueueInfo.family != U32_ERROR && _ComputeQueueInfo.family != U32_ERROR && _PresentQueueInfo.family != U32_ERROR;
}

void Device::findGraphicsQueue(vk::PhysicalDevice& device) {
	std::vector<vk::QueueFamilyProperties> families = device.getQueueFamilyProperties();
	_GraphicsQueueInfo.family = U32_ERROR;
	_GraphicsQueueInfo.index = 0;

	for (uint32_t i = 0; i < families.size(); i++) {
		vk::QueueFamilyProperties family = families[i];
		bool present = device.getSurfaceSupportKHR(i, _Surface);
		if ((family.queueFlags & vk::QueueFlagBits::eGraphics) && (family.queueFlags & vk::QueueFlagBits::eCompute) && present) {
			_GraphicsQueueInfo.family = i;
			_GraphicsQueueInfo.index = 0;
			return;
		}
	}
}

void Device::findTransferQueue(vk::PhysicalDevice& device) {
	std::vector<vk::QueueFamilyProperties> families = device.getQueueFamilyProperties();
	_TransferQueueInfo.family = U32_ERROR;
	_TransferQueueInfo.index = 0;
	QueueInfo best;
	QueueInfo work;
	for (uint32_t i = 0; i < families.size(); i++) {
		vk::QueueFamilyProperties family = families[i];

		if ((family.queueFlags & vk::QueueFlagBits::eTransfer)) {
			if (!(family.queueFlags & vk::QueueFlagBits::eGraphics)) {
				_TransferQueueInfo.family = i;
				_TransferQueueInfo.index = 0;
				return;
			}
			best.family = i;
			best.index = 0;
			// else if (i == mGraphicsQueueInfo.family && family.queueCount > 1) {
			//  	best.family = i;
			//  	best.index = 1;
			//  } else {
			//  	work.family = i;
			//  	work.index = 0;
			//  }
		}
	}

	if (best.family != U32_ERROR)
		_TransferQueueInfo = best;
	else
		_TransferQueueInfo = work;
}

void Device::findPresentQueue(vk::PhysicalDevice& device) { _PresentQueueInfo = _GraphicsQueueInfo; (void)device;}

void Device::findComputeQueue(vk::PhysicalDevice& device) {
	std::vector<vk::QueueFamilyProperties> families = device.getQueueFamilyProperties();
	_ComputeQueueInfo.family = U32_ERROR;
	_ComputeQueueInfo.index = 0;
	QueueInfo best;
	QueueInfo work;
	for (uint32_t i = 0; i < families.size(); i++) {
		vk::QueueFamilyProperties family = families[i];

		if ((family.queueFlags & vk::QueueFlagBits::eCompute)) {
			work.family = i;
			work.index = 0;
			if (!(family.queueFlags & vk::QueueFlagBits::eGraphics)) {
				if (i != _TransferQueueInfo.family) {
					_ComputeQueueInfo.family = i;
					_ComputeQueueInfo.index = 0;
					return;
				} else if (family.queueCount > 1) {
					best.family = i;
					best.index = 1;
				}
			} else if (i == _GraphicsQueueInfo.family && family.queueCount > 1) {
				work.family = i;
				work.index = 1;
			}
		}
	}

	if (best.family != U32_ERROR)
		_ComputeQueueInfo = best;
	else
		_ComputeQueueInfo = work;
}
} // namespace gfx::vulkan
