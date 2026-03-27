#pragma once

#include <gfx_vulkan/details/prerequisites.h>

namespace gfx {

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

}
