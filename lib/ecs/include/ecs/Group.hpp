#pragma once

#include "SSet.hpp"
#include <tuple>

namespace ecs {

template <Component... Owned>
class OwnsGroup {

public:
    OwnsGroup(SSet<Owned>&... s)
        : _sets(&s...) { };

    void tryAdd(uint32_t e)
    {
        bool allPresent = (std::get<SSet<Owned>*>(_sets)->has(e) && ...);
        if (!allPresent) {
            return;
        }

        uint32_t index = std::get<0>(_sets)->sparse[e];
        bool alreadyGrouped = true;
        std::apply([&](auto&... args) { ((alreadyGrouped = alreadyGrouped && (index == args->sparse[e])), ...); }, _sets);
        if (alreadyGrouped) {
            return;
        }

        (std::get<SSet<Owned>*>(_sets)->swap(_size, std::get<SSet<Owned>*>(_sets)->sparse[e]), ...);
        ++_size;
    };

    void tryRemove(uint32_t e)
    {
        bool allPresent = (std::get<SSet<Owned>*>(_sets)->has(e) && ...);
        if (!allPresent) {
            return;
        }

        uint32_t index = std::get<0>(_sets)->sparse[e];
        bool alreadyGrouped = true;
        std::apply([&](auto&... args) { ((alreadyGrouped = alreadyGrouped && (index == args->sparse[e])), ...); }, _sets);
        if (!alreadyGrouped) {
            return;
        }

        --_size;
        (std::get<SSet<Owned>*>(_sets)->swap(_size, std::get<SSet<Owned>*>(_sets)->sparse[e]), ...);
    }

    template <typename Fn>
    void forEach(Fn&& fn)
    {
        for (uint32_t i = 0; i < _size; ++i) {
            fn(std::get<0>(_sets)->entities[i],
                std::get<SSet<Owned>*>(_sets)->dense[i]...);
        }
    };

private:
    std::tuple<SSet<Owned>*...> _sets;
    uint32_t _size = 0;
};

}
