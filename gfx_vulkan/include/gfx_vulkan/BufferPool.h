#pragma once

#include <cstdint>
#include <gfx_vulkan/details/prerequisites.h>
#include <vector>

namespace gfx {

static inline uint32_t	findMemoryType(vk::PhysicalDevice& physicalDevice, uint32_t	typeFilter, vk::MemoryPropertyFlags properties);

union GpuRessourceHandle {
	uint32_t pack;
	struct {
		uint32_t index : 16;
		uint32_t generation : 16;
	};
};

struct BufferPool {
	std::vector<vk::Buffer> buffer;
	std::vector<vk::DeviceAddress> address;
	std::vector<vk::DeviceMemory> memory;
	std::vector<vk::DeviceSize> offset;
	std::vector<vk::DeviceSize> size;

	std::vector<uint16_t> generation; //for validations
	void bind();
};

struct Allocation {
	vk::DeviceMemory memory;
	vk::DeviceSize offset;
	vk::DeviceSize size;
};

struct MemoryBlock {
	vk::DeviceMemory memory;
	vk::DeviceSize size;
	vk::DeviceSize current_offset;
};

struct GpuAllocator {
	std::vector<MemoryBlock> gpu_only;

	[[nodiscard]] Allocation allocate(vk::MemoryAllocateInfo& allocationInfo) {
		Allocation allocation;

		return allocation;
	}
	void newBlock();

	vk::PhysicalDevice _physicalDevice = nullptr;
	vk::Device _device;
};

struct ResourceManager {
	GpuAllocator allocator;
	BufferPool buffers;

	[[nodiscard]] GpuRessourceHandle createBuffer(vk::BufferCreateInfo bufferInfo, vk::MemoryPropertyFlags memoryProperty) {
		GpuRessourceHandle handle = newBuffeHandle();

		auto buffer = _device.createBuffer(bufferInfo);
		auto memoryRequirement = _device.getBufferMemoryRequirements(buffer);
		vk::MemoryAllocateInfo allocationInfo {
			.allocationSize = memoryRequirement.size,
			.memoryTypeIndex = findMemoryType(_physicalDevice, memoryRequirement.memoryTypeBits, memoryProperty)
		};

		vk::MemoryAllocateFlagsInfo allocationFlags {
			.flags = static_cast<uint32_t>(bufferInfo.usage) & static_cast<uint32_t>(vk::BufferUsageFlagBits::eShaderDeviceAddress) ? vk::MemoryAllocateFlagBits::eDeviceAddress : vk::MemoryAllocateFlagBits::eZeroInitializeEXT
		};
		allocationInfo.pNext = &allocationFlags;

		auto allocation = allocator.allocate(allocationInfo);

		_device.bindBufferMemory(buffer, allocation.memory, allocation.offset);

		vk::DeviceAddress address = 0;
		if (static_cast<uint32_t>(bufferInfo.usage) & static_cast<uint32_t>(vk::BufferUsageFlagBits::eShaderDeviceAddress)) {
			vk::BufferDeviceAddressInfo addressInfo {
				.buffer = buffer
			};
			address = _device.getBufferAddress(addressInfo);
		}

		buffers.buffer[handle.index] = buffer;
		buffers.memory[handle.index] = allocation.memory;
		buffers.size[handle.index] = allocation.size;
		buffers.offset[handle.index] = allocation.offset;
		buffers.address[handle.index] = address;

		return handle;
	}

	[[nodiscard]] GpuRessourceHandle newBuffeHandle();

	//creation des handle ici

	vk::Device _device;
	vk::PhysicalDevice _physicalDevice = nullptr;

};

static inline uint32_t	findMemoryType(vk::PhysicalDevice& physicalDevice, uint32_t	typeFilter, vk::MemoryPropertyFlags properties) {
	vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice.getMemoryProperties();
	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
        	return i;
    	}
	}
	throw std::runtime_error("failed to find a suitable memory type!");
}



}
