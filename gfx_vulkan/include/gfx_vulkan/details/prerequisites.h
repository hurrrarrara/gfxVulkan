#pragma once

#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include <vulkan/vulkan.hpp>

#ifdef __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wnullability-extension"
  #pragma clang diagnostic ignored "-Wnullability-completeness"
  #pragma clang diagnostic ignored "-Wpedantic"
  #pragma clang diagnostic ignored "-Wmissing-field-initializers"
  #pragma clang diagnostic ignored "-Wunused-parameter"
  #pragma clang diagnostic ignored "-Wall"
#endif

#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1
#include "vk_mem_alloc.h"

#ifdef __clang__
  #pragma clang diagnostic pop
#endif


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
