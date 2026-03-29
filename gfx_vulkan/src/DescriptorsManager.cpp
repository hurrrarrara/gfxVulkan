#include <gfx_vulkan/DescriptorsManager.h>

namespace gfx {

DescriptorsManager::DescriptorsManager(Device& device)
    : _vkDevice(device.device())
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

[[nodiscard]]
UniformBufferDescriptorHandle DescriptorsManager::createUniformBufferDescriptor(ecs::Entity entity, vk::Buffer buffer)
{
    assert(_availableUniformDescriptorHandles.size() > 0);

    _
}

}
