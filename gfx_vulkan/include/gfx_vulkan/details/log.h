#pragma once

#include <iostream> // IWYU pragma: keep.

#define LOG_COLOR_CLEAR "\033[m"
#define LOG_COLOR_RED "\033[31m"
#define LOG_COLOR_GREEN "\033[32m"
#define LOG_COLOR_YELLOW "\033[33m"
#define LOG_COLOR_BLUE "\033[34m"
#define LOG_COLOR_MAGENTA "\033[35m"

#define LOG_ERROR_MSG(MSG) std::cerr << LOG_COLOR_RED << "Error: " << MSG << LOG_COLOR_CLEAR << "\n";
#define LOG_WARNING_MSG(MSG) std::cerr << LOG_COLOR_YELLOW << "Warning: " << MSG << LOG_COLOR_CLEAR << "\n";
#define LOG_VERBOSE_MSG(MSG) std::cout << LOG_COLOR_BLUE << MSG << LOG_COLOR_CLEAR << "\n";
#define LOG_INFO_MSG(MSG) std::cout << LOG_COLOR_GREEN << MSG << LOG_COLOR_CLEAR << "\n";

#define LOG_VK_ERROR_MSG(MSG) std::cerr << LOG_COLOR_RED << "[VK]-Error: " << MSG << LOG_COLOR_CLEAR << "\n";
#define LOG_VK_WARNING_MSG(MSG) std::cerr << LOG_COLOR_YELLOW << "[VK]-Warning: " << MSG << LOG_COLOR_CLEAR << "\n";
#define LOG_VK_INFO_MSG(MSG) std::cout << LOG_COLOR_BLUE << "[VK]-Info: " << MSG << LOG_COLOR_CLEAR << "\n";
#define LOG_VK_VERBOSE_MSG(MSG) std::cout << LOG_COLOR_MAGENTA << "[VK]-Verbose: " << MSG << LOG_COLOR_CLEAR << "\n";
