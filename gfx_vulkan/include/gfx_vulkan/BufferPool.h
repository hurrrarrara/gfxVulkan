#pragma once

#include <cstdint>
#include <gfx_vulkan/details/prerequisites.h>
#include <vector>

#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1
#include "vk_mem_alloc.h"

namespace gfx {

static inline uint32_t findMemoryType(vk::PhysicalDevice& physicalDevice, uint32_t typeFilter, vk::MemoryPropertyFlags properties);

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

    std::vector<uint16_t> generation; // for validations
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
//
// struct GpuAllocator {
//     std::vector<MemoryBlock> gpu_only;
//
//     [[nodiscard]] Allocation allocate(vk::MemoryAllocateInfo& allocationInfo)
//     {
//         Allocation allocation;
//
//         return allocation;
//     }
//     void newBlock();
//
//     vk::PhysicalDevice _physicalDevice = nullptr;
//     vk::Device _device;
// };

class ResourceManager {
public:
    ResourceManager(vk::Device device, vk::PhysicalDevice physicalDevice, vk::Instance instance)
        : _device(device)
        , _physicalDevice(physicalDevice)
    {
        VmaVulkanFunctions vulkanFunctions = {};
        vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
        vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

        VmaAllocatorCreateInfo allocatorCreateInfo = {};
        allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
        allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_3;
        allocatorCreateInfo.physicalDevice = physicalDevice;
        allocatorCreateInfo.device = device;
        allocatorCreateInfo.instance = instance;
        allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;

        vmaCreateAllocator(&allocatorCreateInfo, &_allocator);
    }

    ~ResourceManager()
    {
        vmaDestroyAllocator(_allocator);
    }

    [[nodiscard]] GpuRessourceHandle createBuffer(vk::BufferCreateInfo bufferInfo, vk::MemoryPropertyFlags memoryProperty)
    {
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

        _buffers.buffer[handle.index] = buffer;
        _buffers.memory[handle.index] = allocation.memory;
        _buffers.size[handle.index] = allocation.size;
        _buffers.offset[handle.index] = allocation.offset;
        _buffers.address[handle.index] = address;

        return handle;
    }

    [[nodiscard]] GpuRessourceHandle newBuffeHandle();

    // creation des handle ici
private:
    vk::Device _device;
    vk::PhysicalDevice _physicalDevice = nullptr;

    VmaAllocator _allocator;

    BufferPool _buffers;
};

static inline uint32_t findMemoryType(vk::PhysicalDevice& physicalDevice, uint32_t typeFilter, vk::MemoryPropertyFlags properties)
{
    vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice.getMemoryProperties();
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    throw std::runtime_error("failed to find a suitable memory type!");
}

}
