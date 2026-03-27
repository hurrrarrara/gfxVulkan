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
	flush();
}

void ResourcesManager::flush() {
	for (uint32_t i = 0; i < _deletedBuffers.size(); i++) {
		vmaDestroyBuffer(_allocator, _deletedBuffers[i].first.value, _deletedBuffers[i].second.value);
	}
	_deletedBuffers.clear();
}

[[nodiscard]]
GpuBufferHandle ResourcesManager::createBuffer(std::string name, vk::BufferCreateInfo bufferInfo, VmaMemoryUsage memoryUsage) {
	assert(_GpuBufferHandleRegistry.find(name) == _GpuBufferHandleRegistry.end());
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

	_GpuBufferHandleRegistry.insert({name, handle});
	_invGpuBufferHandleRegistry.insert({handle, name});

	return handle;
}


void ResourcesManager::destroyBuffer(GpuBufferHandle handle) {
	assert(_buffers.has(handle));
	_deletedBuffers.push_back(std::make_pair(_buffers.get<BufferHandle>(handle), _buffers.get<BufferAllocation>(handle)));
	_buffers.remove(handle);

	auto name = _invGpuBufferHandleRegistry[handle];
	_GpuBufferHandleRegistry.erase(name);
	_invGpuBufferHandleRegistry.erase(handle);
}

void ResourcesManager::destroyBuffer(std::string name) {
	assert(_GpuBufferHandleRegistry.find(name) != _GpuBufferHandleRegistry.end());
	destroyBuffer(_GpuBufferHandleRegistry[name]);
}

[[nodiscard]]
PipelineLayoutHandle ResourcesManager::createPipelineLayout(std::string name, PipelineLayoutInfo& info) {
	assert(_PipelineLayoutHandleRegistry.find(name) == _PipelineLayoutHandleRegistry.end());

	vk::PipelineLayoutCreateInfo createInfo{};
	createInfo.setSetLayoutCount(static_cast<uint32_t>(info.descriptorSetLayouts.size()))
	.setPSetLayouts(info.descriptorSetLayouts.data())
	.setPushConstantRangeCount(static_cast<uint32_t>(info.pushConstantRanges.size()))
	.setPPushConstantRanges(info.pushConstantRanges.data());

	vk::PipelineLayout layout = _vkDevice.createPipelineLayout(createInfo);

	PipelineHandle handle = _PipelineLayoutHandleCount;
	_PipelineLayoutHandleCount++;

	_piplineLayouts.add(handle, layout);
	_PipelineLayoutHandleRegistry.insert({name, handle});
	_invPipelineLayoutHandleRegistry.insert({handle, name});
	return handle;
}

void ResourcesManager::destroyPipelineLayout(PipelineLayoutHandle handle) {
	assert(_piplineLayouts.has(handle));

	_deletedPipelineLayouts.push_back(*_piplineLayouts.get(handle));
	_piplineLayouts.remove(handle);

	auto name = _invPipelineLayoutHandleRegistry[handle];
	_PipelineLayoutHandleRegistry.erase(name);
	_invPipelineLayoutHandleRegistry.erase(handle);
}

void ResourcesManager::destroyPipelineLayout(std::string name) {
	assert(_PipelineLayoutHandleRegistry.find(name) != _PipelineLayoutHandleRegistry.end());
	destroyPipelineLayout(_PipelineLayoutHandleRegistry[name]);
}



}
