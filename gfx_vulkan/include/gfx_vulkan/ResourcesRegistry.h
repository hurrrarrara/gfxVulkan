#pragma once

#include <gfx_vulkan/details/prerequisites.h>
#include <ecs/Entity.hpp>
#include <ecs/SSet.hpp>
#include <ecs/Archetypes.h>

namespace gfx {

/* clang-format off */
struct BufferGpuHandle {vk::Buffer value;};
struct BufferAddress {vk::DeviceAddress value;};
struct BufferMapped {void* value;};
struct BufferOffset {vk::DeviceSize value;};
struct BufferSize {vk::DeviceSize value;};
struct BufferMemory {vk::DeviceMemory value;};
struct BufferAllocation {VmaAllocation value;};
typedef ecs::Archetype<BufferGpuHandle, BufferAddress, BufferMapped, BufferOffset, BufferSize, BufferMemory, BufferAllocation> BufferArchetype;

struct BufferHandle {
    uint32_t index : 24 = 0xFFFFFF;
    uint32_t generation : 8 = 0;
    inline static constexpr BufferHandle invalid() { return {}; }
    inline bool operator==(const BufferHandle& c) const { return this->index == c.index && this->generation == c.generation; };
};

struct PipelineLayoutHandle {
    uint32_t index : 24 = 0xFFFFFF;
    uint32_t generation : 8 = 0;
    inline static constexpr PipelineLayoutHandle invalid() { return {}; }
    inline bool operator==(const PipelineLayoutHandle& c) const { return this->index == c.index && this->generation == c.generation; };
};

struct PipelineHandle {
    uint32_t index : 24 = 0xFFFFFF;
    uint32_t generation : 8 = 0;
    inline static constexpr PipelineHandle invalid() { return {}; }
    inline bool operator==(const PipelineHandle& c) const { return this->index == c.index && this->generation == c.generation; };
};

struct GpuResourcesRegistry {
	BufferArchetype buffers;
    ecs::SSet<vk::PipelineLayout> pipelineLayouts;
	ecs::Archetype<vk::Pipeline, PipelineLayoutHandle> piplines;
};




}
