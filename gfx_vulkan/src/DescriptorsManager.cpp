#include <gfx_vulkan/DescriptorsManager.h>

namespace gfx {

GpuDescriptorsManager::GpuDescriptorsManager(Device& device, GpuDescriptorRegistry& descriptorsRegistry)
    : _vkDevice(device.device())
    , _descriptorsRegistry(descriptorsRegistry)
{
    std::vector<vk::DescriptorSetLayoutBinding> bindings;
    std::vector<vk::DescriptorBindingFlags> flags;
    for (auto itr : _typesMap) {
        bindings.push_back(vk::DescriptorSetLayoutBinding {
            .binding = itr.second.first,
            .descriptorType = itr.first,
            .descriptorCount = itr.second.second,
            .stageFlags = vk::ShaderStageFlagBits::eAll });
        flags.push_back(vk::DescriptorBindingFlagBits::ePartiallyBound | vk::DescriptorBindingFlagBits::eUpdateAfterBind);
    }

    vk::DescriptorSetLayoutBindingFlagsCreateInfo bindingFlags {};
    bindingFlags.pBindingFlags = flags.data();
    bindingFlags.bindingCount = bindings.size();

    vk::DescriptorSetLayoutCreateInfo layoutInfo {};
    layoutInfo.setBindingCount(bindings.size())
        .setPBindings(bindings.data())
        .setFlags(vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool)
        .setPNext(&bindingFlags);

    _descriptorLayout = _vkDevice.createDescriptorSetLayout(layoutInfo);

    std::vector<vk::DescriptorPoolSize> poolSizes;
    for (auto itr : _typesMap) {
        poolSizes.push_back({ .type = itr.first,
            .descriptorCount = itr.second.second });
    }
    vk::DescriptorPoolCreateInfo poolInfo {};
    poolInfo.setFlags(vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind)
        .setPoolSizeCount(poolSizes.size())
        .setPPoolSizes(poolSizes.data())
        .setMaxSets(1);
    _descriptorPool = _vkDevice.createDescriptorPool(poolInfo);

    vk::DescriptorSetAllocateInfo setAllocateInfo {};
    setAllocateInfo.setDescriptorPool(_descriptorPool)
        .setPSetLayouts(&_descriptorLayout)
        .setDescriptorSetCount(1);
    _descriptorSet = _vkDevice.allocateDescriptorSets(setAllocateInfo).front();
}

GpuDescriptorsManager::~GpuDescriptorsManager()
{
    _vkDevice.destroyDescriptorPool(_descriptorPool);
    _vkDevice.destroyDescriptorSetLayout(_descriptorLayout);
}

[[nodiscard]]
UniformBufferDescriptor GpuDescriptorsManager::createUniformBufferDescriptor(vk::Buffer buffer)
{
    vk::DescriptorBufferInfo bufferInfo {};
    bufferInfo.setBuffer(buffer)
        .setOffset(0)
        .setRange(vk::WholeSize);

    UniformBufferDescriptor handle = _descriptorsRegistry.uniformBuffers.allocate();

    vk::WriteDescriptorSet write {};
    write.setDstSet(_descriptorSet)
        .setDescriptorCount(1)
        .setDescriptorType(vk::DescriptorType::eUniformBuffer)
        .setDstArrayElement(handle.index)
        .setDstBinding(handle.binding())
        .setBufferInfo(bufferInfo);

    _vkDevice.updateDescriptorSets(1, &write, 0, nullptr);

    return handle;
}

}
