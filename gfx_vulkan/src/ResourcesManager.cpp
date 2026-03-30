#define VMA_IMPLEMENTATION
#include <gfx_vulkan/ResourcesManager.h>

namespace gfx {

GpuResourcesManager::GpuResourcesManager(Device& device, GpuResourcesRegistry& resourcesRegistry)
    : _vkDevice(device.device())
    , _resourcesRegistry(resourcesRegistry)
{
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

GpuResourcesManager::~GpuResourcesManager()
{
    vmaDestroyAllocator(_allocator);
    flush();
}

void GpuResourcesManager::flush()
{
    for (uint32_t i = 0; i < _deletedBuffers.size(); i++) {
        vmaDestroyBuffer(_allocator, _deletedBuffers[i].first.value, _deletedBuffers[i].second.value);
    }
    for (uint32_t i = 0; i < _deletedPipelineLayouts.size(); i++) {
        _vkDevice.destroyPipelineLayout(_deletedPipelineLayouts[i]);
    }
    _deletedBuffers.clear();
    _deletedPipelineLayouts.clear();
}

[[nodiscard]]
BufferHandle GpuResourcesManager::createBuffer(vk::BufferCreateInfo bufferInfo, VmaMemoryUsage memoryUsage)
{
    VmaAllocationCreateInfo allocationCreateInfo = {};
    allocationCreateInfo.usage = memoryUsage;

    VmaAllocation allocation;
    VmaAllocationInfo allocationInfo;
    VkBuffer buffer;

    if (vmaCreateBuffer(_allocator, &*bufferInfo, &allocationCreateInfo, &buffer, &allocation, &allocationInfo))
        throw std::runtime_error("failed to create buffer");

    BufferHandle handle = _bufferHandleRegistry.allocate();

    vk::DeviceAddress address = 0;
    if (static_cast<uint32_t>(bufferInfo.usage) & static_cast<uint32_t>(vk::BufferUsageFlagBits::eShaderDeviceAddress)) {
        vk::BufferDeviceAddressInfo addressInfo {
            .buffer = buffer
        };
        address = _vkDevice.getBufferAddress(addressInfo);
    }

    _resourcesRegistry.buffers.add(handle.index);
    _resourcesRegistry.buffers.set(handle.index, BufferGpuHandle { buffer });
    _resourcesRegistry.buffers.set(handle.index, BufferAddress { address });
    _resourcesRegistry.buffers.set(handle.index, BufferMapped { allocationInfo.pMappedData });
    _resourcesRegistry.buffers.set(handle.index, BufferOffset { allocationInfo.offset });
    _resourcesRegistry.buffers.set(handle.index, BufferSize { allocationInfo.size });
    _resourcesRegistry.buffers.set(handle.index, BufferMemory { allocationInfo.deviceMemory });
    _resourcesRegistry.buffers.set(handle.index, BufferAllocation { allocation });

    return handle;
}

void GpuResourcesManager::destroyBuffer(BufferHandle handle)
{
    assert(_bufferHandleRegistry.valid(handle));

    _deletedBuffers.push_back(std::make_pair(_resourcesRegistry.buffers.get<BufferGpuHandle>(handle.index), _resourcesRegistry.buffers.get<BufferAllocation>(handle.index)));
    _resourcesRegistry.buffers.remove(handle.index);

    _bufferHandleRegistry.free(handle);
}

[[nodiscard]]
PipelineLayoutHandle GpuResourcesManager::createPipelineLayout(PipelineLayoutInfo& info)
{
    vk::PipelineLayoutCreateInfo createInfo {};
    createInfo.setSetLayoutCount(static_cast<uint32_t>(info.descriptorSetLayouts.size()))
        .setPSetLayouts(info.descriptorSetLayouts.data())
        .setPushConstantRangeCount(static_cast<uint32_t>(info.pushConstantRanges.size()))
        .setPPushConstantRanges(info.pushConstantRanges.data());

    vk::PipelineLayout layout = _vkDevice.createPipelineLayout(createInfo);

    PipelineLayoutHandle handle = _pipelineLayoutHandleRegistry.allocate();

    _resourcesRegistry.pipelineLayouts.add(handle.index, layout);

    return handle;
}

void GpuResourcesManager::destroyPipelineLayout(PipelineLayoutHandle handle)
{
    assert(_pipelineLayoutHandleRegistry.valid(handle));

    _deletedPipelineLayouts.push_back(_resourcesRegistry.pipelineLayouts.get(handle.index));
    _resourcesRegistry.pipelineLayouts.remove(handle.index);

    _pipelineLayoutHandleRegistry.free(handle);
}

}
