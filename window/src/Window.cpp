#include <window/Window.h>
#include <stdexcept>

namespace gfx{

Window::Window(std::string name, uint32_t width, uint32_t height) : _name(name), _width(width), _height(height)
{
#ifndef GFX_X11
    glfwInitHint(GLFW_WAYLAND_LIBDECOR, GLFW_WAYLAND_DISABLE_LIBDECOR);
#else
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
#endif
    if (glfwInit() == GLFW_FALSE)
        throw std::runtime_error("failed to initialize glfw window!");
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    this->_window = glfwCreateWindow(_width, _height, _name.c_str(), nullptr, nullptr);
    if (_window == nullptr) {
        throw std::runtime_error("failed to create window");
	}
	glfwSetWindowUserPointer(_window, this);
	// glfwSetMouseButtonCallback(_window, mouseButtonCallback);
	// glfwSetKeyCallback(_window, keyCallback);
	// glfwSetCursorPosCallback(_window, mouseMoveCallback);
	// glfwSetScrollCallback(_window, mouseScrollCallback);
}

Window::~Window()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}

std::vector<const char*> Window::getRequiredExtensions() const
{
    uint32_t count = 0;

    const char** extensions = glfwGetRequiredInstanceExtensions(&count);

    std::vector<const char*> requiredExtensions;
    requiredExtensions.reserve(count);
    for (uint32_t i = 0; i < count; i++) {
        requiredExtensions.push_back(extensions[i]);
    }
    return requiredExtensions;
}

VkSurfaceKHR Window::createVulkanSurface(VkInstance& instance) const
{
    VkSurfaceKHR _surface;
    if (glfwCreateWindowSurface(instance, _window, nullptr, &_surface) != 0)
        throw std::runtime_error("failed to create window surface!");
    return _surface;
}

VkExtent2D Window::getExtent() const
{
    int width;
    int height;
    glfwGetFramebufferSize(_window, &width, &height);
    return VkExtent2D { .width = static_cast<uint32_t>(width), .height = static_cast<uint32_t>(height) };
}

} // namespace gfx::window
