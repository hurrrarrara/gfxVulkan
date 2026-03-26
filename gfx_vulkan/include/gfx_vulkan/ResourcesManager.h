#pragma once

#include <gfx_vulkan/details/prerequisites.h>
#include <gfx_vulkan/details/Archetypes.h>
#include <gfx_vulkan/Device.h>




namespace gfx {

struct BufferHandle {vk::Buffer value;};
struct BufferAddress {vk::DeviceAddress value;};
struct BufferMapped {void* value;};
struct BufferOffset {vk::DeviceSize value;};
struct BufferSize {vk::DeviceSize value;};
struct BufferMemory {vk::DeviceMemory value;};
struct BufferAllocation {VmaAllocation value;};

typedef ecs::Entity GpuBufferHandle;


class ResourcesManager {
public:
	ResourcesManager(Device& device);
	~ResourcesManager();

	[[nodiscard]]
	GpuBufferHandle createBuffer(vk::BufferCreateInfo bufferInfo, VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_AUTO);
	void destroyBuffer(GpuBufferHandle handle);
private:
	vk::Device& _vkDevice;
	VmaAllocator _allocator;

	gfx::details::Archetype<BufferHandle, BufferAddress, BufferMapped, BufferOffset, BufferSize, BufferMemory, BufferAllocation> _buffers;

	GpuBufferHandle _GpuBufferHandleCount = 0;

};
}
