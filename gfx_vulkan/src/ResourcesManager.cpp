#define VMA_IMPLEMENTATION
#include <gfx_vulkan/ResourcesManager.h>

namespace gfx {

ResourcesManager::ResourcesManager(Device& device) : _vkDevice(device.device()) {
	VmaVulkanFunctions vulkanFunctions = {};
	vulkanFunctions.vkGetInstanceProcAddr = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetInstanceProcAddr;
	vulkanFunctions.vkGetDeviceProcAddr = VULKAN_HPP_DEFAULT_DISPATCHER.vkGetDeviceProcAddr;

	VmaAllocatorCreateInfo allocatorCreateInfo = {};
	allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT | VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
	allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_3;
	allocatorCreateInfo.physicalDevice = device.physicalDevice();
	allocatorCreateInfo.device = device.device();
	allocatorCreateInfo.instance = device.instance();
	allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;

	vmaCreateAllocator(&allocatorCreateInfo, &_allocator);
}

ResourcesManager::~ResourcesManager() {
	vmaDestroyAllocator(_allocator);
}

[[nodiscard]]
GpuBufferHandle ResourcesManager::createBuffer(vk::BufferCreateInfo bufferInfo, VmaMemoryUsage memoryUsage) {
	VmaAllocationCreateInfo allocationCreateInfo = {};
	allocationCreateInfo.usage = memoryUsage;

	VmaAllocation allocation;
	VmaAllocationInfo allocationInfo;
	VkBuffer buffer;


	if(vmaCreateBuffer(_allocator, &*bufferInfo, &allocationCreateInfo, &buffer, &allocation, &allocationInfo))
		throw std::runtime_error("failed to create buffer");

	GpuBufferHandle handle = _GpuBufferHandleCount;
	_GpuBufferHandleCount++;

	vk::DeviceAddress address = 0;
	if (static_cast<uint32_t>(bufferInfo.usage) & static_cast<uint32_t>(vk::BufferUsageFlagBits::eShaderDeviceAddress)) {
		vk::BufferDeviceAddressInfo addressInfo {
			.buffer = buffer
		};
		address = _vkDevice.getBufferAddress(addressInfo);
	}

	_buffers.add(handle);
	_buffers.set(handle, BufferHandle{buffer});
	_buffers.set(handle, BufferAddress{address});
	_buffers.set(handle, BufferMapped{allocationInfo.pMappedData});
	_buffers.set(handle, BufferOffset{allocationInfo.offset});
	_buffers.set(handle, BufferSize{allocationInfo.size});
	_buffers.set(handle, BufferMemory{allocationInfo.deviceMemory});
	_buffers.set(handle, BufferAllocation{allocation});

	return handle;
}

}
