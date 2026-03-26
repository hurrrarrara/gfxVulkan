#include <window/Window.h>

int main() {
	gfx::Window window("window_creation");

	while(window.isOpen()) {
		window.pollEvents();
	}
	
	return 0;
}
