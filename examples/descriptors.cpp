#include <gfx_vulkan/Device.h>
#include <window/Window.h>
#include <gfx_vulkan/ResourcesManager.h>
#include <gfx_vulkan/GpuDescriptorRegistry.h>
#include <gfx_vulkan/DescriptorsManager.h>

int main()
{
    gfx::Window window("device_creation");
    gfx::Device device(window);
    gfx::GpuResourcesRegistry gpuResourcesRegistry {};
    gfx::GpuResourcesManager gpuResourcesManager(device, gpuResourcesRegistry);
    gfx::GpuDescriptorRegistry gpuDescriptorRegistry {};
    gfx::GpuDescriptorsManager gpuDescriptorManager(device, gpuDescriptorRegistry);

    auto buffer1 = gpuResourcesManager.createBuffer(
        { .size = 1024,
            .usage = vk::BufferUsageFlagBits::eShaderDeviceAddress | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eUniformBuffer,
            .sharingMode = vk::SharingMode::eExclusive });

    auto des = gpuDescriptorManager.createUniformBufferDescriptor(gpuResourcesRegistry.buffers.get<gfx::BufferGpuHandle>(buffer1.index).value);

    gpuResourcesManager.destroyBuffer(buffer1);

    gpuDescriptorManager.free(des);

	// gfx::BufferHandle b;
	// gpuDescriptorManager.free(b);

    gpuResourcesManager.flush();
}
