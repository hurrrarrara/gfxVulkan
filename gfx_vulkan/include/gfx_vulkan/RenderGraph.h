#pragma once

#include <gfx_vulkan/ResourcesRegistry.h>
#include <ecs/SSet.hpp>
#include <functional>

namespace gfx {

class RenderGraph {
public:
    struct ImageDesc {
        ImageHandle handle;
        vk::ImageLayout layout;
    };

    struct Pass {
        std::vector<BufferHandle> inputBuffers;
        std::vector<ImageDesc> inputImages;
        std::vector<BufferHandle> outputBuffers;
        std::vector<ImageHandle> outputImages;

        std::function<void(vk::CommandBuffer&)> callback;

        void addInputBuffer(BufferHandle handle) { inputBuffers.push_back(handle); };
        void addInputImage(ImageDesc handle) { inputImages.push_back(handle); }
        void addOutputBuffer(BufferHandle handle) { outputBuffers.push_back(handle); }
        void addOutputImage(ImageHandle handle) { outputImages.push_back(handle); }
    };

private:
    ecs::SSet<ImageHandle> _images;
    ecs::SSet<BufferHandle> _buffers;

    std::vector<Pass> _passes; // in order of addition
    std::vector<std::size_t> _executionOrder; // indices of passes
};

}
