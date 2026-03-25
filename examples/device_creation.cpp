#include <gfx_vulkan/Device.h>
#include <gfx_vulkan/BufferPool.h>
#include <window/Window.h>

int main() {
	gfx::Window window("device_creation");
	gfx::Device device(window);

	gfx::GpuRessourceHandle handle;
	handle.pack = 0xaaaaeeee;
	handle.index = 0xaaaa;
	handle.generation = 0xffff;
	std::cout << std::hex << handle.pack << "\n" << handle.index << "\n" << handle.generation;

	while(window.isOpen()) {
		window.pollEvents();
	}



}
