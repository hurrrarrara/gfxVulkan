#pragma once

#include <concepts>

namespace ecs {

template <typename T>
concept Component = std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T>;

}
