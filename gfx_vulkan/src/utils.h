#pragma once

// #include "prerequisites.h"// IWYU pragma: keep.
#include <gfx_vulkan/details/prerequisites.h>
#include <fstream> 
#include <limits.h>
#include <unistd.h>
#include <vector>

namespace gfx {

static inline const char* checkRequiredLayers(const std::vector<const char*>& required, const std::vector<vk::LayerProperties>& available) {
	std::vector<const char*> final;
	final.clear();
	final.reserve(required.size());
	for (uint32_t i = 0; i < required.size(); i++) {
		uint32_t j = 0;
		for (; j < available.size(); j++) {
			if (std::strcmp(available[j].layerName, required[i]) == 0)
				break;
		}
		if (j == available.size())
			return available[j].layerName;
	}
	return nullptr;
}

static inline const char* checkRequiredExtensions(const std::vector<const char*>& required, const std::vector<vk::ExtensionProperties>& available) {
	std::vector<const char*> final;
	final.clear();
	final.reserve(required.size());
	for (uint32_t i = 0; i < required.size(); i++) {
		uint32_t j = 0;
		for (; j < available.size(); j++) {
			if (std::strcmp(available[j].extensionName, required[i]) == 0)
				break;
		}
		if (j == available.size())
			return available[j].extensionName;
	}
	return nullptr;
}

static inline std::string getSelfPath() {
	char path[PATH_MAX + 1];
	ssize_t len = ::readlink("/proc/self/exe", path, PATH_MAX);
	if (len == -1)
		throw std::runtime_error("could not get the path of the executable");
	path[len] = '\0';
	std::string str(path);

	str.resize(str.find_last_of("/"));
	str.append("/");

	return str;
}

// static inline std::vector<char> loadBinaryFile(std::string path) {
// 	std::string fullpath = SELF_PATH + path;
// 	std::ifstream file(fullpath, std::ios::ate | std::ios::binary);
//
// 	if (!file.is_open()) {
// 		throw std::runtime_error("failed to open file " + fullpath + " !");
// 	}
// 	std::vector<char> buffer(file.tellg());
// 	file.seekg(0, std::ios::beg);
// 	file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
// 	file.close();
// 	return buffer;
// }

static inline uint32_t	findMemoryType(vk::PhysicalDevice& physicalDevice, uint32_t	typeFilter, vk::MemoryPropertyFlags properties) {
	vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice.getMemoryProperties();
	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
        	return i;
    	}
	}
	throw std::runtime_error("failed to find a suitable memory type!");
}


} // namespace gfx::vulkan
