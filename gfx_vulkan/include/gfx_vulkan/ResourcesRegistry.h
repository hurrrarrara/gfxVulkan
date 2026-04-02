#pragma once

#include <gfx_vulkan/details/prerequisites.h>
#include <ecs/Entity.hpp>
#include <ecs/SSet.hpp>
#include <ecs/Archetypes.h>

namespace gfx {

/* clang-format off */

enum ResourceTypeFlagBits {
	eBuffer = 0x1,
	eImage = 0x2,
	eImageView = 0x3,
	ePipelineLayout = 0x4,
	ePipeline = 0x5
};

struct BufferGpuHandle {vk::Buffer value;};
struct BufferAddress {vk::DeviceAddress value;};
struct BufferMapped {void* value;};
struct BufferOffset {vk::DeviceSize value;};
struct BufferSize {vk::DeviceSize value;};
struct BufferMemory {vk::DeviceMemory value;};
struct BufferAllocation {VmaAllocation value;};
typedef ecs::Archetype<BufferGpuHandle, BufferAddress, BufferMapped, BufferOffset, BufferSize, BufferMemory, BufferAllocation> BufferArchetype;

struct BufferHandle {
    uint32_t index : 20 = 0xFFFFF;
    uint32_t generation : 8 = 0;
	const uint32_t type : 4 = ResourceTypeFlagBits::eBuffer;
    inline static constexpr BufferHandle invalid() { return {}; }
    inline bool operator==(const BufferHandle& c) const { return this->index == c.index && this->generation == c.generation; };
};

struct ImageHandle {
    uint32_t index : 20 = 0xFFFFF;
    uint32_t generation : 8 = 0;
	const uint32_t type : 4 = ResourceTypeFlagBits::eImage;
    inline static constexpr BufferHandle invalid() { return {}; }
    inline bool operator==(const BufferHandle& c) const { return this->index == c.index && this->generation == c.generation; };
};

struct ImageViewHandle {
    uint32_t index : 20 = 0xFFFFF;
    uint32_t generation : 8 = 0;
	const uint32_t type : 4 = ResourceTypeFlagBits::eImageView;
    inline static constexpr BufferHandle invalid() { return {}; }
    inline bool operator==(const BufferHandle& c) const { return this->index == c.index && this->generation == c.generation; };
};

struct PipelineLayoutHandle {
    uint32_t index : 20 = 0xFFFFF;
    uint32_t generation : 8 = 0;
	const uint32_t type : 4 = ResourceTypeFlagBits::ePipelineLayout;
    inline static constexpr PipelineLayoutHandle invalid() { return {}; }
    inline bool operator==(const PipelineLayoutHandle& c) const { return this->index == c.index && this->generation == c.generation; };
};

struct PipelineHandle {
    uint32_t index : 20 = 0xFFFFF;
    uint32_t generation : 8 = 0;
	const uint32_t type : 4 = ResourceTypeFlagBits::ePipeline;
    inline static constexpr PipelineHandle invalid() { return {}; }
    inline bool operator==(const PipelineHandle& c) const { return this->index == c.index && this->generation == c.generation; };
};

struct GpuResourcesRegistry {
	BufferArchetype buffers;
    ecs::SSet<vk::PipelineLayout> pipelineLayouts;
	ecs::Archetype<vk::Pipeline, PipelineLayoutHandle> piplines;
};




}
