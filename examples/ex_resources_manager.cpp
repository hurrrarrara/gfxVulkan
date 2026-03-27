#include <gfx_vulkan/Device.h>
#include <window/Window.h>
#include <gfx_vulkan/ResourcesManager.h>

int main() {
	gfx::Window window("device_creation");
	gfx::Device device(window);
	gfx::ResourcesManager resourcesManager(device);


	auto buffer1 = resourcesManager.createBuffer("buffer1",
			{ .size = 1024,
            .usage = vk::BufferUsageFlagBits::eShaderDeviceAddress | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eStorageBuffer,
            .sharingMode = vk::SharingMode::eExclusive
           });

	auto buffer2 = resourcesManager.createBuffer("buffer2",
			{ .size = 1024 * 1024,
            .usage = vk::BufferUsageFlagBits::eShaderDeviceAddress | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eStorageBuffer,
            .sharingMode = vk::SharingMode::eExclusive
           });

	gfx::PipelineLayoutInfo layoutInfo{};
	layoutInfo.pushConstantRanges.push_back({
			.stageFlags = vk::ShaderStageFlagBits::eVertex,
			.offset = 0,
			.size = 64});

	auto layout1 = resourcesManager.createPipelineLayout("layout1", layoutInfo);

	while(window.isOpen()) {
		window.pollEvents();
	}
	(void)buffer1;

	resourcesManager.destroyBuffer("buffer1");

	resourcesManager.flush();

	resourcesManager.destroyBuffer(buffer2);

	resourcesManager.destroyPipelineLayout(layout1);

	resourcesManager.flush();
}
