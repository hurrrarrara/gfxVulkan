#pragma once

#include <ecs/Entity.hpp>

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
#ifndef GFX_UNIFORM_BUFFER_BINDING
#define GFX_UNIFORM_BUFFER_BINDING 0
#endif
#ifndef GFX_STORAGE_BUFFER_BINDING
#define GFX_STORAGE_BUFFER_BINDING 1
#endif
#ifndef GFX_SAMPLER_BINDING
#define GFX_SAMPLER_BINDING 2
#endif
#ifndef GFX_SAMPLED_IMAGE_BINDING
#define GFX_SAMPLED_IMAGE_BINDING 3
#endif
#ifndef GFX_STORAGE_IMAGE_BINDING
#define GFX_STORAGE_IMAGE_BINDING 4
#endif

namespace gfx {

struct UniformBufferDescriptor {
    uint32_t index : 16 = 0xffff;
    uint32_t generation : 16 = 0;
    inline static constexpr uint32_t binding() { return GFX_UNIFORM_BUFFER_BINDING; }
    inline static constexpr UniformBufferDescriptor invalid() { return {}; }
    inline bool operator==(const UniformBufferDescriptor& c) const { return this->index == c.index && this->generation == c.generation; };
};

struct StorageBufferDescriptor {
    uint32_t index : 16 = 0xffff;
    uint32_t generation : 12 = 0;
    inline static constexpr uint32_t binding() { return GFX_STORAGE_BUFFER_BINDING; }
    inline static constexpr StorageBufferDescriptor invalid() { return {}; }
    inline bool operator==(const StorageBufferDescriptor& c) const { return this->index == c.index && this->generation == c.generation; };
};

struct SamplerDescriptor {
    uint32_t index : 16 = 0xffff;
    uint32_t generation : 16 = 0;
    inline static constexpr uint32_t binding() { return GFX_SAMPLER_BINDING; }
    inline static constexpr SamplerDescriptor invalid() { return {}; }
    inline bool operator==(const SamplerDescriptor& c) const { return this->index == c.index && this->generation == c.generation; };
};

struct SampledImageDescriptor {
    uint32_t index : 16 = 0xffff;
    uint32_t generation : 16 = 0;
    inline static constexpr uint32_t binding() { return GFX_SAMPLED_IMAGE_BINDING; }
    inline static constexpr SampledImageDescriptor invalid() { return {}; }
    inline bool operator==(const SampledImageDescriptor& c) const { return this->index == c.index && this->generation == c.generation; };
};

struct StorageImageDescriptor {
    uint32_t index : 16 = 0xffff;
    uint32_t generation : 16 = 0;
    inline static constexpr uint32_t binding() { return GFX_STORAGE_IMAGE_BINDING; }
    inline static constexpr StorageImageDescriptor invalid() { return {}; }
    inline bool operator==(const StorageImageDescriptor& c) const { return this->index == c.index && this->generation == c.generation; };
};

struct GpuDescriptorRegistry {
    ecs::EntityRegistry<UniformBufferDescriptor, GFX_MAX_UNIFORM_BUFFER_DESCRIPTOR_COUNT> uniformBuffers;
    ecs::EntityRegistry<StorageBufferDescriptor, GFX_MAX_STORAGE_BUFFER_DESCRIPTOR_COUNT> storageBuffers;
    ecs::EntityRegistry<SamplerDescriptor, GFX_MAX_SAMPLER_DESCRIPTOR_COUNT> samplers;
    ecs::EntityRegistry<SampledImageDescriptor, GFX_MAX_SAMPLED_IMAGE_DESCRIPTOR_COUNT> sampledImages;
    ecs::EntityRegistry<StorageImageDescriptor, GFX_MAX_STORAGE_IMAGE_DESCRIPTOR_COUNT> storageImages;
};
}
