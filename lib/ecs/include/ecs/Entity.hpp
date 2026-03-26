#pragma once

#include <stdint.h>

namespace ecs {

using Entity = uint32_t; // created by the registry
static constexpr Entity INVALID_ENTITY = UINT32_MAX;

}
