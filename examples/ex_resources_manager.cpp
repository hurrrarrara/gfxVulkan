#include <gfx_vulkan/Device.h>
#include <window/Window.h>
#include <gfx_vulkan/ResourcesManager.h>

int main()
{
    gfx::Window window("device_creation");
    gfx::Device device(window);
    gfx::GpuResourcesRegistry gpuResourcesRegistry {};
    gfx::GpuResourcesManager gpuResourcesManager(device, gpuResourcesRegistry);

    auto buffer1 = gpuResourcesManager.createBuffer(
        { .size = 1024,
            .usage = vk::BufferUsageFlagBits::eShaderDeviceAddress | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eStorageBuffer,
            .sharingMode = vk::SharingMode::eExclusive });

    auto buffer2 = gpuResourcesManager.createBuffer(
        { .size = 1024 * 1024,
            .usage = vk::BufferUsageFlagBits::eShaderDeviceAddress | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eStorageBuffer,
            .sharingMode = vk::SharingMode::eExclusive });

    gfx::PipelineLayoutInfo layoutInfo {};
    layoutInfo.pushConstantRanges.push_back({ .stageFlags = vk::ShaderStageFlagBits::eVertex,
        .offset = 0,
        .size = 64 });

    auto layout1 = gpuResourcesManager.createPipelineLayout(layoutInfo);

    (void)buffer1;

    gpuResourcesManager.flush();

    gpuResourcesManager.destroyBuffer(buffer2);

    gpuResourcesManager.destroyPipelineLayout(layout1);

    gpuResourcesManager.destroyBuffer(buffer1);

    gpuResourcesManager.flush();
}
