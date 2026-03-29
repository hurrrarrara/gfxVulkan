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

[[nodiscard]]
StorageBufferDescriptor GpuDescriptorsManager::createStorageBufferDescriptor(vk::Buffer buffer)
{
    vk::DescriptorBufferInfo bufferInfo {};
    bufferInfo.setBuffer(buffer)
        .setOffset(0)
        .setRange(vk::WholeSize);

    StorageBufferDescriptor handle = _descriptorsRegistry.storageBuffers.allocate();

    vk::WriteDescriptorSet write {};
    write.setDstSet(_descriptorSet)
        .setDescriptorCount(1)
        .setDescriptorType(vk::DescriptorType::eStorageBuffer)
        .setDstArrayElement(handle.index)
        .setDstBinding(handle.binding())
        .setBufferInfo(bufferInfo);

    _vkDevice.updateDescriptorSets(1, &write, 0, nullptr);

    return handle;
}

[[nodiscard]]
SamplerDescriptor GpuDescriptorsManager::createSamplerDescriptor(vk::Sampler sampler)
{
    vk::DescriptorImageInfo imageInfo {};
    imageInfo.setSampler(sampler);

    SamplerDescriptor handle = _descriptorsRegistry.samplers.allocate();

    vk::WriteDescriptorSet write {};
    write.setDstSet(_descriptorSet)
        .setDescriptorCount(1)
        .setDescriptorType(vk::DescriptorType::eSampler)
        .setDstArrayElement(handle.index)
        .setDstBinding(handle.binding())
        .setImageInfo(imageInfo);

    _vkDevice.updateDescriptorSets(1, &write, 0, nullptr);

    return handle;
}

[[nodiscard]]
SampledImageDescriptor GpuDescriptorsManager::createSampledImageDescriptor(vk::ImageView imageView)
{
    vk::DescriptorImageInfo imageInfo {};
    imageInfo.setImageView(imageView)
        .setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal);

    SampledImageDescriptor handle = _descriptorsRegistry.sampledImages.allocate();

    vk::WriteDescriptorSet write {};
    write.setDstSet(_descriptorSet)
        .setDescriptorCount(1)
        .setDescriptorType(vk::DescriptorType::eSampledImage)
        .setDstArrayElement(handle.index)
        .setDstBinding(handle.binding())
        .setImageInfo(imageInfo);

    _vkDevice.updateDescriptorSets(1, &write, 0, nullptr);

    return handle;
}

[[nodiscard]]
StorageImageDescriptor GpuDescriptorsManager::createStorageImageDescriptor(vk::ImageView imageView)
{
    vk::DescriptorImageInfo imageInfo {};
    imageInfo.setImageView(imageView)
        .setImageLayout(vk::ImageLayout::eGeneral);

    StorageImageDescriptor handle = _descriptorsRegistry.storageImages.allocate();

    vk::WriteDescriptorSet write {};
    write.setDstSet(_descriptorSet)
        .setDescriptorCount(1)
        .setDescriptorType(vk::DescriptorType::eStorageImage)
        .setDstArrayElement(handle.index)
        .setDstBinding(handle.binding())
        .setImageInfo(imageInfo);

    _vkDevice.updateDescriptorSets(1, &write, 0, nullptr);

    return handle;
}

}
