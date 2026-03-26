#pragma once

#include "Component.hpp"
#include "Entity.hpp"
#include <vector>

namespace ecs {

template <Component T>
struct SSet {
    std::vector<T> dense;
    std::vector<Entity> entities;
    std::vector<Entity> sparse;

    void add(Entity e, T component)
    {
        if (sparse.size() <= e)
            sparse.resize(e + 1, INVALID_ENTITY);
        if (has(e))
            return;
        sparse[e] = dense.size();
        dense.push_back(component);
        entities.push_back(e);
    }

    void add(Entity e, T component, uint32_t index)
    {
        add(e, component);
        swap(index, dense.size() - 1);
    }

    void swap(uint32_t indexA, uint32_t indexB)
    {
        if (indexA == indexB || dense.size() < indexA || dense.size() < indexB)
            return;

        T tempComponent = dense[indexA];
        Entity tempEntity = entities[indexA];

        dense[indexA] = dense[indexB];
        entities[indexA] = entities[indexB];
        dense[indexB] = tempComponent;
        entities[indexB] = tempEntity;

        sparse[entities[indexB]] = indexB;
        sparse[entities[indexA]] = indexA;
    }

    void remove(Entity e)
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
        sparse[e] = INVALID_ENTITY;
    }

    [[nodiscard]] bool has(Entity e)
    {
        return (e < sparse.size() && sparse[e] != INVALID_ENTITY);
    }

    [[nodiscard]] T* get(Entity e)
    {
        if (!has(e))
            return nullptr;
        return &dense[sparse[e]];
    }
};

}
