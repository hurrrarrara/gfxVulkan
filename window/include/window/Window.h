#pragma once

#include <cstdint>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

namespace gfx {
class Window {
public:
	Window(std::string name, uint32_t width = 800, uint32_t height = 600);
	~Window();

	VkSurfaceKHR createVulkanSurface(VkInstance& instance) const;

	void close() { glfwSetWindowShouldClose(_window, GL_TRUE);};
	bool isOpen() const {return !glfwWindowShouldClose(_window);};
	void pollEvents() const {glfwPollEvents();};

	void* raw() const {return _window;}
	std::string name() const {return _name;}
	uint32_t width() const {return _width;}
	uint32_t height() const {return _height;}
	VkExtent2D getExtent() const;
	std::vector<const char*> getRequiredExtensions() const;

private:
	GLFWwindow* _window = nullptr;
	std::string _name;
	uint32_t _width;
	uint32_t _height;

};
} // namespace gfx
