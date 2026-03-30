#pragma once

#include <gfx_vulkan/ResourcesRegistry.h>
#include <functional>

namespace gfx {

struct Pass {
    std::vector<BufferHandle> inputBuffers;
    std::vector<ImageHandle> inputImages;
    std::vector<BufferHandle> outputBuffers;
    std::vector<ImageHandle> outputImages;

    // std::function<void(vk::CommandBuffer&)> callback;
};
}
