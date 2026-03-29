#pragma once

#include <gfx_vulkan/details/prerequisites.h>
#include <gfx_vulkan/Device.h>
#include <ecs/SSet.hpp>
#include <queue>
#include <unordered_map>

#ifndef GFX_MAX_UNIFORM_BUFFER_DESCRIPTOR_COUNT
#define GFX_MAX_UNIFORM_BUFFER_DESCRIPTOR_COUNT 1000
#endif
#ifndef GFX_MAX_STORAGE_BUFFER_DESCRIPTOR_COUNT
#define GFX_MAX_STORAGE_BUFFER_DESCRIPTOR_COUNT 1000
#endif
#ifndef GFX_MAX_SAMPLER_DESCRIPTOR_COUNT
#define GFX_MAX_SAMPLER_DESCRIPTOR_COUNT 100
#endif
#ifndef GFX_MAX_SAMPLED_IMAGE_DESCRIPTOR_COUNT
#define GFX_MAX_SAMPLED_IMAGE_DESCRIPTOR_COUNT 1000
#endif
#ifndef GFX_MAX_STORAGE_IMAGE_DESCRIPTOR_COUNT
#define GFX_MAX_STORAGE_IMAGE_DESCRIPTOR_COUNT 1000
#endif

namespace gfx {
/* clang-format off */
struct UniformBufferDescriptorHandle {uint32_t value;};
struct StorageBufferDescriptorHandle {uint32_t value;};
struct SamplerDescriptorHandle {uint32_t value;};
struct SampledImageDescriptorHandle {uint32_t value;};
struct StorageImageDescriptorHandle {uint32_t value;};
/* clang-format on */

class DescriptorsManager {
public:
    DescriptorsManager(Device& device);

    [[nodiscard]]
    UniformBufferDescriptorHandle createUniformBufferDescriptor(vk::Buffer buffer);
    void freeUniformBufferDescriptor(UniformBufferDescriptorHandle handle);

private:
    vk::Device _vkDevice;
    vk::DescriptorSetLayout _descriptorLayout = nullptr;
    vk::DescriptorPool _descriptorPool = nullptr;
    vk::DescriptorSet _descriptorSet = nullptr;

    std::unordered_map<vk::DescriptorType, std::pair<uint32_t, uint32_t>> _typesMap {
        { vk::DescriptorType::eUniformBuffer, { 0, GFX_MAX_UNIFORM_BUFFER_DESCRIPTOR_COUNT } },
        { vk::DescriptorType::eStorageBuffer, { 1, GFX_MAX_STORAGE_BUFFER_DESCRIPTOR_COUNT } },
        { vk::DescriptorType::eSampler, { 2, GFX_MAX_SAMPLER_DESCRIPTOR_COUNT } },
        { vk::DescriptorType::eSampledImage, { 3, GFX_MAX_SAMPLED_IMAGE_DESCRIPTOR_COUNT } },
        { vk::DescriptorType::eStorageImage, { 4, GFX_MAX_STORAGE_IMAGE_DESCRIPTOR_COUNT } },
    };

    ecs::SSet<UniformBufferDescriptorHandle> _uniformBufferHandles;
    std::queue<UniformBufferDescriptorHandle> _availableUniformDescriptorHandles;
};

}
