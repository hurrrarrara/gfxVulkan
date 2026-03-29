#pragma once

#include "Component.hpp"
#include "Entity.hpp"
#include <vector>

namespace ecs {

template <Component T>
struct SSet {
    std::vector<T> dense;
    std::vector<uint32_t> entities;
    std::vector<uint32_t> sparse;

    void add(uint32_t e, T component)
    {
        if (sparse.size() <= e)
            sparse.resize(e + 1, 0xffffffff);
        if (has(e))
            return;
        sparse[e] = dense.size();
        dense.push_back(component);
        entities.push_back(e);
    }

    void add(uint32_t e, T component, uint32_t index)
    {
        add(e, component);
        swap(index, dense.size() - 1);
    }

    void swap(uint32_t indexA, uint32_t indexB)
    {
        if (indexA == indexB || dense.size() < indexA || dense.size() < indexB)
            return;

        T tempComponent = dense[indexA];
        uint32_t tempEntity = entities[indexA];

        dense[indexA] = dense[indexB];
        entities[indexA] = entities[indexB];
        dense[indexB] = tempComponent;
        entities[indexB] = tempEntity;

        sparse[entities[indexB]] = indexB;
        sparse[entities[indexA]] = indexA;
    }

    void remove(uint32_t e)
    {
        if (!has(e))
            return;
        uint32_t index = sparse[e];
        uint32_t last = dense.size() - 1;

        dense[index] = dense[last];
        entities[index] = entities[last];

        sparse[entities[index]] = index;

        dense.pop_back();
        entities.pop_back();
        sparse[e] = 0xffffffff;
    }

    [[nodiscard]] bool has(uint32_t e)
    {
        return (e < sparse.size() && sparse[e] != 0xffffffff);
    }

    [[nodiscard]] T* get(uint32_t e)
    {
        if (!has(e))
            return nullptr;
        return &dense[sparse[e]];
    }
};

}
