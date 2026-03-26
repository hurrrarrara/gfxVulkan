#pragma once

#include <concepts>
#include <gfx_vulkan/details/log.h>
#include <gfx_vulkan/details/prerequisites.h>

namespace gfx {

template <typename T>
concept VulkanWindow = requires(T& window, VkInstance& instance) {
    { window.raw() } -> std::same_as<void*>;
    { window.createVulkanSurface(instance) } -> std::same_as<VkSurfaceKHR>;
    { window.getRequiredExtensions() } -> std::same_as<std::vector<const char*>>;
    { window.getExtent() } -> std::same_as<VkExtent2D>;
};

class Device {
public:
    template <VulkanWindow Window>
    Device(Window& window)
    {
        auto windowExtensios = window.getRequiredExtensions();
        for (auto& e : windowExtensios)
            _RequiredInstanceExtensions.push_back(e);
        createInstance();
        VkInstance vkInstance = _Instance;
        _Surface = vk::SurfaceKHR(window.createVulkanSurface(vkInstance));
        // mFrameBufferSize = window.getFrameBufferSize();
#ifndef NDEBUG
        LOG_INFO_MSG("Vulkan surface created")
#endif
        pickPhysicalDevice();
        createDevice();
        // createSwapChain();
        // createCommandPools();
        // createSyncObjects();
        // // createFramebuffer();
        // createFrames();
        //
        // vk::PipelineLayoutCreateInfo pipelineLayoutInfo { .setLayoutCount = 0, .pushConstantRangeCount = 0 };
        // mVkDefaultPipelineLayout = mVkDevice.createPipelineLayout(pipelineLayoutInfo);
        // mDeletionQueue = DeletionQueue(mVkDevice);
    }

	[[nodiscard]]
	vk::Instance instance() const {return _Instance;};
	[[nodiscard]]
	vk::PhysicalDevice physicalDevice() const {return _PhysicalDevice;};
	[[nodiscard]]
	vk::Device& device() {return _Device;};

private:
    void createInstance();
    void pickPhysicalDevice();
    void createDevice();

    uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);

    bool checkPhysicalDeviceProperties(vk::PhysicalDevice& device);
    bool checkPhysicalDeviceExtensions(vk::PhysicalDevice& device);
    bool checkPhysicalDeviceFeatures(vk::PhysicalDevice& device);
    bool checkPhysicalDeviceQueues(vk::PhysicalDevice& device);

    void findGraphicsQueue(vk::PhysicalDevice& device);
    void findTransferQueue(vk::PhysicalDevice& device);
    void findPresentQueue(vk::PhysicalDevice& device);
    void findComputeQueue(vk::PhysicalDevice& device);

    vk::Instance _Instance;
    vk::SurfaceKHR _Surface;
    vk::DebugUtilsMessengerEXT _DebugMessenger;
    vk::PhysicalDevice _PhysicalDevice = nullptr;
    vk::Device _Device;

    QueueInfo _GraphicsQueueInfo;
    QueueInfo _PresentQueueInfo;
    QueueInfo _ComputeQueueInfo;
    QueueInfo _TransferQueueInfo;
    vk::Queue _GraphicsQueue = nullptr;
    vk::Queue _ComputeQueue = nullptr;
    vk::Queue _PresentQueue = nullptr;
    vk::Queue _TransferQueue = nullptr;

    std::vector<const char*> _RequiredInstanceExtensions {
#ifndef NDEBUG
        vk::EXTDebugUtilsExtensionName,
#endif
    };
    std::vector<const char*> _RequiredInstanceLayers {
#ifndef NDEBUG
        "VK_LAYER_KHRONOS_validation",
#endif
    };

    static inline std::vector<const char*> _RequiredDeviceExtensions = {
        vk::KHRSwapchainExtensionName,
        vk::KHRSpirv14ExtensionName,
        vk::KHRSynchronization2ExtensionName,
        vk::KHRCreateRenderpass2ExtensionName,
        vk::KHRDynamicRenderingExtensionName,
        vk::KHRBufferDeviceAddressExtensionName
        // vk::KHRUnifiedImageLayoutsExtensionName
    };
    static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
        vk::DebugUtilsMessageTypeFlagsEXT type,
        const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void*)
    {
        (void)type;
        switch (severity) {
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose:
            LOG_VK_VERBOSE_MSG(pCallbackData->pMessage);
            break;
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning:
            LOG_VK_WARNING_MSG(pCallbackData->pMessage);
            break;
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eError:
            LOG_VK_ERROR_MSG(pCallbackData->pMessage);
            break;
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo:
            LOG_VK_INFO_MSG(pCallbackData->pMessage);
            break;
        }
        return vk::False;
    }
};
}

// namespace gfx {
// namespace vulkan {
//
//     struct ContextInfo {
//         void* windowHandle = nullptr;
//         const std::vector<const char*> windowExtension;
//     };
//
//     template <typename T>
//     concept VulkanWindow = requires(T& window, VkInstance& instance) {
//         { window.createVulkanSurface(instance) } -> std::same_as<VkSurfaceKHR>;
//         { window.getRequiredExtensions() } -> std::same_as<std::vector<const char*>>;
//         { window.getFrameBufferSize() } -> std::same_as<VkExtent2D>;
//     };
//
//     class Context {
//     public:
//         vk::Instance mVkInstance;
//         vk::SurfaceKHR mVkSurface;
//         vk::DebugUtilsMessengerEXT mVkDebugMessenger;
//         vk::PhysicalDevice mVkPhysicalDevice = nullptr;
//         vk::Device mVkDevice;
//
//         QueueInfo mGraphicsQueueInfo;
//         QueueInfo mPresentQueueInfo;
//         QueueInfo mComputeQueueInfo;
//         QueueInfo mTransferQueueInfo;
//         vk::Queue mVkGraphicsQueue = nullptr;
//         vk::Queue mVkComputeQueue = nullptr;
//         vk::Queue mVkPresentQueue = nullptr;
//         vk::Queue mVkTransferQueue = nullptr;
//
//         vk::SwapchainKHR mVkSwapChain = nullptr;
//         // std::vector<vk::Image> mVkSwapChainImages;
//         // std::vector<vk::ImageView> mVkSwapChainImagesView;
//         struct SwapChainInfo {
//             vk::SurfaceFormatKHR format;
//             vk::PresentModeKHR presentMode;
//             vk::Extent2D extent;
//         } mSwapChainInfo;
//
//         // std::array<Command, MAX_FRAME_IN_FLIGHT> mGraphicsPrimaryCommand;
//         std::vector<vk::Semaphore> mVkGraphicsSemaphores;
//         std::array<vk::Semaphore, MAX_FRAME_IN_FLIGHT> mVkPresentSemaphores;
//         std::array<vk::Fence, MAX_FRAME_IN_FLIGHT> mVkFences;
//         //
//         std::vector<Image> mSwapChainImages;
//
//         vk::CommandPool mVkGraphicsCommandPool;
//         std::vector<vk::CommandBuffer> mVkGraphicsCommandBuffers;
//         vk::CommandPool mVkTransferCommandPool;
//         vk::CommandBuffer mVkTransferCommandBuffer;
//         std::vector<Frame> mFrames;
//         std::unordered_map<std::string, vk::ShaderModule> mVkShaderModules;
//
//         DeletionQueue mDeletionQueue;
//
//         // Image frameBuffer;
//
//         uint32_t mFrameIndex = 0;
//         uint32_t mCurrentImageIndex;
//
//         vk::Extent2D mFrameBufferSize;
//
//         vk::PipelineLayout mVkDefaultPipelineLayout;
//
// 		Context(){};
//
//         template <VulkanWindow Window>
//         Context(Window& window)
//         {
//             auto windowExtensios = window.getRequiredExtensions();
//             for (auto& e : windowExtensios)
//                 mVkRequiredInstanceExtensions.push_back(e);
//             createInstance();
//             VkInstance vkInstance = mVkInstance;
//             mVkSurface = vk::SurfaceKHR(window.createVulkanSurface(vkInstance));
//             mFrameBufferSize = window.getFrameBufferSize();
// #ifndef NDEBUG
//             LOG_INFO_MSG("Vulkan surface created")
// #endif
//             pickPhysicalDevice();
//             createDevice();
//             createSwapChain();
//             createCommandPools();
//             createSyncObjects();
//             // createFramebuffer();
//             createFrames();
//
//             vk::PipelineLayoutCreateInfo pipelineLayoutInfo { .setLayoutCount = 0, .pushConstantRangeCount = 0 };
//             mVkDefaultPipelineLayout = mVkDevice.createPipelineLayout(pipelineLayoutInfo);
//             mDeletionQueue = DeletionQueue(mVkDevice);
//         }
//
//         // Context(const ContextInfo& info);
//         ~Context();
//
//         void waitIdle();
//
//         Frame& beginFrame();
//         void submitFrame();
//
//         vk::CommandBuffer beginTransfer();
//         void submitTransfer();
//
//         vk::Extent2D getResolution() { return mSwapChainInfo.extent; };
//
//         void createShaderModule(std::string name, std::vector<char>& spirv);
//         vk::ShaderModule& getShaderModule(std::string name) { return mVkShaderModules[name]; };
//         [[nodiscard]] vk::DeviceMemory allocateImage(vk::Image& image, vk::MemoryPropertyFlags memProperties);
//         [[nodiscard]] vk::DeviceMemory allocateBuffer(vk::Buffer& buffer, vk::MemoryPropertyFlags memProperties, bool getAdress);
//         [[nodiscard]] vk::DeviceAddress getBufferAdress(vk::Buffer& buffer);
//
//         [[nodiscard]] vk::PipelineLayout createPipelineLayout(vk::PipelineLayoutCreateInfo layoutInfo, vk::PushConstantRange range);
//
//         Image& getSwapChainImage() { return mSwapChainImages[mCurrentImageIndex]; };
//
//     private:
//         void createInstance();
//         void pickPhysicalDevice();
//         void createDevice();
//         void createSwapChain();
//         void createSwapChainImagesView();
//         void createCommandPools();
//         // void allocateCommadBuffers(Command& command);
//         void createSyncObjects();
//         void createFrames();
//         void createStagingBuffer();
//
//         uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);
//
//         bool checkPhysicalDeviceProperties(vk::PhysicalDevice& device);
//         bool checkPhysicalDeviceExtensions(vk::PhysicalDevice& device);
//         bool checkPhysicalDeviceFeatures(vk::PhysicalDevice& device);
//         bool checkPhysicalDeviceQueues(vk::PhysicalDevice& device);
//
//         void findGraphicsQueue(vk::PhysicalDevice& device);
//         void findTransferQueue(vk::PhysicalDevice& device);
//         void findPresentQueue(vk::PhysicalDevice& device);
//         void findComputeQueue(vk::PhysicalDevice& device);
//
//     private:
//         std::vector<const char*> mVkRequiredInstanceExtensions {
// #ifndef NDEBUG
//             vk::EXTDebugUtilsExtensionName,
// #endif
//         };
//         std::vector<const char*> mVkRequiredInstanceLayers {
// #ifndef NDEBUG
//             "VK_LAYER_KHRONOS_validation",
// #endif
//         };
//
//     private:
//         static inline std::vector<const char*> mVkRequiredDeviceExtensions = {
//             vk::KHRSwapchainExtensionName,
//             vk::KHRSpirv14ExtensionName,
//             vk::KHRSynchronization2ExtensionName,
//             vk::KHRCreateRenderpass2ExtensionName,
//             vk::KHRDynamicRenderingExtensionName,
//             vk::KHRBufferDeviceAddressExtensionName
//             // vk::KHRUnifiedImageLayoutsExtensionName
//         };
//         static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
//             vk::DebugUtilsMessageTypeFlagsEXT type,
//             const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
//             void*)
//         {
//             switch (severity) {
//             case vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose:
//                 LOG_VK_VERBOSE_MSG(pCallbackData->pMessage);
//                 break;
//             case vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning:
//                 LOG_VK_WARNING_MSG(pCallbackData->pMessage);
//                 break;
//             case vk::DebugUtilsMessageSeverityFlagBitsEXT::eError:
//                 LOG_VK_ERROR_MSG(pCallbackData->pMessage);
//                 break;
//             case vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo:
//                 LOG_VK_INFO_MSG(pCallbackData->pMessage);
//                 break;
//             }
//             return vk::False;
//         }
//     };
//
// } // namespace vulkan
// } // namespace gfx
