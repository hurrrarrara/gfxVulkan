#include <gfx_vulkan/Device.h>
#include <window/Window.h>

int main() {
	gfx::Window window("device_creation");
	gfx::Device device(window);


	while(window.isOpen()) {
		window.pollEvents();
	}
}
