#pragma once

#include <gfx_vulkan/details/prerequisites.h>
#include <gfx_vulkan/details/Archetypes.h>
#include <gfx_vulkan/Device.h>

#include <ecs/SSet.hpp>

#include <unordered_map>

namespace gfx {

struct BufferHandle {vk::Buffer value;};
struct BufferAddress {vk::DeviceAddress value;};
struct BufferMapped {void* value;};
struct BufferOffset {vk::DeviceSize value;};
struct BufferSize {vk::DeviceSize value;};
struct BufferMemory {vk::DeviceMemory value;};
struct BufferAllocation {VmaAllocation value;};

struct PipelineInfo {
 	std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
    VkPipelineVertexInputStateCreateInfo        vertexInput{};
    VkPipelineInputAssemblyStateCreateInfo      inputAssembly{};
    VkPipelineRasterizationStateCreateInfo      rasterizer{};
    VkPipelineMultisampleStateCreateInfo        multisampling{};
    VkPipelineDepthStencilStateCreateInfo       depthStencil{};
    VkPipelineColorBlendAttachmentState         colorBlendAttachment{};
    VkPipelineRenderingCreateInfo               renderingInfo{};
	vk::PipelineDynamicStateCreateInfo          dynamicStateInfo{};
	vk::PipelineViewportStateCreateInfo			viewportInfo{};
};

struct PipelineLayoutInfo {
	std::vector<vk::PushConstantRange> pushConstantRanges;
    std::vector<vk::DescriptorSetLayout> descriptorSetLayouts;
};


typedef ecs::Entity GpuBufferHandle;
typedef ecs::Entity PipelineHandle;
typedef ecs::Entity PipelineLayoutHandle;

typedef gfx::details::Archetype<BufferHandle, BufferAddress, BufferMapped, BufferOffset, BufferSize, BufferMemory, BufferAllocation> BufferArchetype;


class ResourcesManager {
public:
	ResourcesManager(Device& device);
	~ResourcesManager();
	void flush();

	[[nodiscard]]
	GpuBufferHandle createBuffer(std::string name, vk::BufferCreateInfo bufferInfo, VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_AUTO);
	void destroyBuffer(GpuBufferHandle handle);
	void destroyBuffer(std::string name);
	[[nodiscard]]
	GpuBufferHandle getBufferHandle(std::string name) { 
		return (_GpuBufferHandleRegistry.find(name) != _GpuBufferHandleRegistry.end() ? _GpuBufferHandleRegistry[name] : ecs::INVALID_ENTITY);}

	[[nodiscard]]
	PipelineLayoutHandle createPipelineLayout(std::string name, PipelineLayoutInfo& info);
	void destroyPipelineLayout(PipelineLayoutHandle handle);
	void destroyPipelineLayout(std::string name);
	[[nodiscard]]
	PipelineLayoutHandle getPipelineHandle(std::string name) { 
		return (_PipelineLayoutHandleRegistry.find(name) != _PipelineLayoutHandleRegistry.end() ? _PipelineLayoutHandleRegistry[name] : ecs::INVALID_ENTITY);}


private:
	vk::Device& _vkDevice;
	VmaAllocator _allocator;

	BufferArchetype _buffers;
	std::vector<std::pair<BufferHandle, BufferAllocation>> _deletedBuffers;
	std::unordered_map<std::string, GpuBufferHandle> _GpuBufferHandleRegistry;
	std::unordered_map<GpuBufferHandle, std::string> _invGpuBufferHandleRegistry;
	GpuBufferHandle _GpuBufferHandleCount = 0;

	ecs::SSet<vk::PipelineLayout> _piplineLayouts;
	std::vector<vk::PipelineLayout> _deletedPipelineLayouts;
	std::unordered_map<std::string, PipelineLayoutHandle> _PipelineLayoutHandleRegistry;
	std::unordered_map<PipelineLayoutHandle, std::string> _invPipelineLayoutHandleRegistry;
	PipelineLayoutHandle _PipelineLayoutHandleCount = 0;

	gfx::details::Archetype<vk::Pipeline, PipelineLayoutHandle> _pipelines;
	std::vector<vk::Pipeline> _deletedPipelines;
	std::unordered_map<std::string, PipelineHandle> _PipelineHandleRegistry;
	std::unordered_map<PipelineHandle, std::string> _invPipelineHandleRegistry;
	PipelineHandle _PipelineHandleCount = 0;
};
}
