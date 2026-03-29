#pragma once

#include <stdint.h>
#include <concepts>
#include <cassert>
#include <vector>

namespace ecs {

template <typename T>
concept Entity = requires(T& handle) {
    { handle.index } -> std::convertible_to<uint32_t>;
    { handle.generation } -> std::convertible_to<uint32_t>;
    { T::invalid() } -> std::convertible_to<T>;
};

/*
struct ExampleEntity {
    uint32_t index : 24 = 0xFFFFFF;
    uint32_t generation : 8 = 0;
    inline static constexpr ExampleEntity invalid() { return {}; }
    inline bool operator==(const ExampleEntity& c) const { return this->index == c.index && this->generation == c.generation; };
};
*/

template <Entity E, std::size_t N = 0>
class EntityRegistry {
public:
    EntityRegistry()
    {
        _generations.reserve(N);
    }

    [[nodiscard]]
    E allocate()
    {
        E entity;
        if constexpr (N > 0) {
            assert(_generations.size() < N);
        }
        if (_freelist.empty()) {
            entity.index = _generations.size();
            entity.generation = 0;
            _generations.push_back(0);
            return entity;
        }
        entity.index = _freelist.back();
        _freelist.pop_back();
        entity.generation = _generations[entity.index];
        return entity;
    }

    void free(E entity)
    {
        assert(valid(entity));
        _generations[entity.index]++;
        _freelist.push_back(entity.index);
    }

    [[nodiscard]]
    bool valid(E entity) const
    {
        return entity.index < _generations.size() && _generations[entity.index] == entity.generation;
    }

private:
    std::vector<uint32_t> _generations;
    std::vector<uint32_t> _freelist;
};

}
