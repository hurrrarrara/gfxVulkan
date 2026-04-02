#pragma once

#include <gfx_vulkan/details/prerequisites.h>
#include <gfx_vulkan/Device.h>
#include <gfx_vulkan/ResourcesRegistry.h>
#include <ecs/Entity.hpp>

namespace gfx {

/* clang-format off */
struct PipelineInfo {
 	std::vector<vk::PipelineShaderStageCreateInfo>	shaderStages;
	vk::PipelineVertexInputStateCreateInfo			vertexInput{};
	vk::PipelineInputAssemblyStateCreateInfo 		inputAssembly{};
	vk::PipelineRasterizationLineStateCreateInfo	rasterizer{};
	vk::PipelineMultisampleStateCreateInfo			multisampling{};
	vk::PipelineDepthStencilStateCreateInfo			depthStencil{};
	vk::PipelineColorBlendAttachmentState			colorBlendAttachment{};
	vk::PipelineDynamicStateCreateInfo         		dynamicStateInfo{};
	vk::PipelineViewportStateCreateInfo				viewportInfo{};
	vk::PipelineRenderingCreateInfo					renderingInfo{};
};

struct PipelineLayoutInfo {
	std::vector<vk::PushConstantRange> pushConstantRanges;
    std::vector<vk::DescriptorSetLayout> descriptorSetLayouts;
};
/* clang-format on */

class GpuResourcesManager {
public:
    GpuResourcesManager(Device& device, GpuResourcesRegistry& resourcesRegistry);
    ~GpuResourcesManager();
    void flush();

    [[nodiscard]]
    BufferHandle createBuffer(vk::BufferCreateInfo bufferInfo, VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_AUTO);
    void destroyBuffer(BufferHandle handle);

    [[nodiscard]]
    PipelineLayoutHandle createPipelineLayout(PipelineLayoutInfo& info);
    void destroyPipelineLayout(PipelineLayoutHandle handle);

private:
    vk::Device& _vkDevice;
    GpuResourcesRegistry& _resourcesRegistry;
    VmaAllocator _allocator;

    ecs::EntityRegistry<BufferHandle> _bufferHandleRegistry;
    ecs::EntityRegistry<PipelineLayoutHandle> _pipelineLayoutHandleRegistry;
    ecs::EntityRegistry<PipelineHandle> _pipelineHandleRegistry;

    std::vector<std::pair<BufferGpuHandle, BufferAllocation>> _deletedBuffers;
    std::vector<vk::PipelineLayout> _deletedPipelineLayouts;
    std::vector<vk::Pipeline> _deletedPipelines;
};

// typedef ecs::Entity GpuBufferHandle;
// typedef ecs::Entity PipelineHandle;
// typedef ecs::Entity PipelineLayoutHandle;
//
// typedef gfx::details::Archetype<BufferHandle, BufferAddress, BufferMapped, BufferOffset, BufferSize, BufferMemory, BufferAllocation> BufferArchetype;
/* clang-format on */

// class ResourcesManager {
// public:
//     ResourcesManager(Device& device);
//     ~ResourcesManager();
//     void flush();
//
//     [[nodiscard]]
//     GpuBufferHandle createBuffer(std::string name, vk::BufferCreateInfo bufferInfo, VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_AUTO);
//     void destroyBuffer(GpuBufferHandle handle);
//     void destroyBuffer(std::string name);
//     [[nodiscard]]
//     GpuBufferHandle getBufferHandle(std::string name)
//     {
//         return (_GpuBufferHandleRegistry.find(name) != _GpuBufferHandleRegistry.end() ? _GpuBufferHandleRegistry[name] : ecs::INVALID_ENTITY);
//     }
//
//     [[nodiscard]]
//     PipelineLayoutHandle createPipelineLayout(std::string name, PipelineLayoutInfo& info);
//     void destroyPipelineLayout(PipelineLayoutHandle handle);
//     void destroyPipelineLayout(std::string name);
//     [[nodiscard]]
//     PipelineLayoutHandle getPipelineHandle(std::string name)
//     {
//         return (_PipelineLayoutHandleRegistry.find(name) != _PipelineLayoutHandleRegistry.end() ? _PipelineLayoutHandleRegistry[name] : ecs::INVALID_ENTITY);
//     }
//
// private:
//     vk::Device& _vkDevice;
//     VmaAllocator _allocator;
//
//     BufferArchetype _buffers;
//     std::vector<std::pair<BufferHandle, BufferAllocation>> _deletedBuffers;
//     std::unordered_map<std::string, GpuBufferHandle> _GpuBufferHandleRegistry;
//     std::unordered_map<GpuBufferHandle, std::string> _invGpuBufferHandleRegistry;
//     GpuBufferHandle _GpuBufferHandleCount = 0;
//
//     ecs::SSet<vk::PipelineLayout> _piplineLayouts;
//     std::vector<vk::PipelineLayout> _deletedPipelineLayouts;
//     std::unordered_map<std::string, PipelineLayoutHandle> _PipelineLayoutHandleRegistry;
//     std::unordered_map<PipelineLayoutHandle, std::string> _invPipelineLayoutHandleRegistry;
//     PipelineLayoutHandle _PipelineLayoutHandleCount = 0;
//
//     gfx::details::Archetype<vk::Pipeline, PipelineLayoutHandle> _pipelines;
//     std::vector<vk::Pipeline> _deletedPipelines;
//     std::unordered_map<std::string, PipelineHandle> _PipelineHandleRegistry;
//     std::unordered_map<PipelineHandle, std::string> _invPipelineHandleRegistry;
//     PipelineHandle _PipelineHandleCount = 0;
// };
}
