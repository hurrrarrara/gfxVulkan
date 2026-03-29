#pragma once

#include <gfx_vulkan/details/prerequisites.h>
#include <gfx_vulkan/Device.h>
#include <gfx_vulkan/GpuDescriptorRegistry.h>
#include <ecs/SSet.hpp>
#include <unordered_map>

namespace gfx {

class GpuDescriptorsManager {
public:
    GpuDescriptorsManager(Device& device, GpuDescriptorRegistry& descriptorsRegistry);
    ~GpuDescriptorsManager();

    [[nodiscard]]
    UniformBufferDescriptor createUniformBufferDescriptor(vk::Buffer buffer);
    [[nodiscard]]
    StorageBufferDescriptor createStorageBufferDescriptor(vk::Buffer buffer);
    [[nodiscard]]
    SamplerDescriptor createSamplerDescriptor(vk::Sampler sampler);
    [[nodiscard]]
    SampledImageDescriptor createSampledImageDescriptor(vk::ImageView imageView);
    [[nodiscard]]
    StorageImageDescriptor createStorageImageDescriptor(vk::ImageView imageView);

    template <ecs::Entity E>
    void free(E handle)
    {
        if constexpr (std::same_as<E, UniformBufferDescriptor>) {
            _descriptorsRegistry.uniformBuffers.free(handle);
        } else if constexpr (std::same_as<E, StorageBufferDescriptor>) {
            _descriptorsRegistry.storageBuffers.free(handle);
        } else if constexpr (std::same_as<E, SamplerDescriptor>) {
            _descriptorsRegistry.samplers.free(handle);
        } else if constexpr (std::same_as<E, SampledImageDescriptor>) {
            _descriptorsRegistry.sampledImages.free(handle);
        } else if constexpr (std::same_as<E, StorageImageDescriptor>) {
            _descriptorsRegistry.storageImages.free(handle);
        } else {
            static_assert(false, "GpuDescriptorsManager::free, invalid type used");
        }
    }

private:
    vk::Device _vkDevice;
    GpuDescriptorRegistry& _descriptorsRegistry;
    vk::DescriptorSetLayout _descriptorLayout = nullptr;
    vk::DescriptorPool _descriptorPool = nullptr;
    vk::DescriptorSet _descriptorSet = nullptr;

    std::unordered_map<vk::DescriptorType, std::pair<uint32_t, uint32_t>> _typesMap {
        { vk::DescriptorType::eUniformBuffer, { GFX_UNIFORM_BUFFER_BINDING, GFX_MAX_UNIFORM_BUFFER_DESCRIPTOR_COUNT } },
        { vk::DescriptorType::eStorageBuffer, { GFX_STORAGE_BUFFER_BINDING, GFX_MAX_STORAGE_BUFFER_DESCRIPTOR_COUNT } },
        { vk::DescriptorType::eSampler, { GFX_SAMPLER_BINDING, GFX_MAX_SAMPLER_DESCRIPTOR_COUNT } },
        { vk::DescriptorType::eSampledImage, { GFX_SAMPLED_IMAGE_BINDING, GFX_MAX_SAMPLED_IMAGE_DESCRIPTOR_COUNT } },
        { vk::DescriptorType::eStorageImage, { GFX_STORAGE_IMAGE_BINDING, GFX_MAX_STORAGE_IMAGE_DESCRIPTOR_COUNT } },
    };
};

}
