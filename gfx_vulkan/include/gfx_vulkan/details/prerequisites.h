#pragma once

#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include <vulkan/vulkan.hpp>


namespace gfx {

const uint32_t U32_ERROR = 0xFFFFFF;
extern std::string SELF_PATH;
constexpr uint32_t MAX_FRAME_IN_FLIGHT = 2;

struct QueueInfo {
	uint32_t family = U32_ERROR;
	uint32_t index = 0;
	bool operator==(QueueInfo& rhs) { return this->family == rhs.family && this->index == rhs.index; }
};
} // namespace gfx::vulkan
