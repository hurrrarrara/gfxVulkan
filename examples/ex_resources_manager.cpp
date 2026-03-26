#include <gfx_vulkan/Device.h>
#include <window/Window.h>
#include <gfx_vulkan/ResourcesManager.h>

int main() {
	gfx::Window window("device_creation");
	gfx::Device device(window);
	gfx::ResourcesManager resourcesManager(device);


	while(window.isOpen()) {
		window.pollEvents();
	}
}
