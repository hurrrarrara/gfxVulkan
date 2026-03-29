#pragma once

#include <cstdint>
#include <vector>
#include <cassert>
#include <tuple>
#include "Component.hpp"

namespace ecs {

template <Component... Components>
class Archetype {
public:
    template <Component T>
    [[nodiscard]]
    T& get(uint32_t e)
    {
        assert(has(e));
        return std::get<std::vector<T>>(_sets)[getEntityIndex(e)];
    }

    template <Component T>
    [[nodiscard]]
    std::vector<T>& get()
    {
        return std::get<std::vector<T>>(_sets);
    }

    template <Component T>
    void set(uint32_t e, T value)
    {
        assert(has(e));
        std::vector<T>& set = std::get<std::vector<T>>(_sets);
        uint32_t index = _sparse[e];
        if (set.size() < index)
            set.resize(index + 1);
        set[index] = value;
    }

    void remove(uint32_t e)
    {
        if (!has(e))
            return;
        uint32_t index = _sparse[e];
        uint32_t last = _dense.size() - 1;

        _dense[index] = _dense[last];
        std::apply([index, last](auto&... set) {
            ((set[index] = set[last]), ...);
        },
            _sets);

        _sparse[_dense[index]] = _dense[last];

        _dense.pop_back();
        std::apply([](auto&... set) {
            ((set.pop_back()), ...);
        },
            _sets);

        _sparse[e] = 0xffffffff;
    }

    void add(uint32_t e)
    {
        if (_sparse.size() <= e)
            _sparse.resize(e + 1, 0xffffffff);
        if (has(e))
            return;
        _sparse[e] = _dense.size();
        _dense.push_back(e);

        uint32_t size = _dense.size();
        std::apply([size](auto&... set) {
            ((set.resize(size)), ...);
        },
            _sets);
    }

    [[nodiscard]]
    bool has(uint32_t e)
    {
        return _sparse.size() >= e && _sparse[e] != 0xffffffff;
    }

    void clear()
    {
        std::apply([](auto&... set) {
            ((set.resize(0)), ...);
        },
            _sets);
        _dense.resize(0);
        _sparse.resize(_sparse.size(), 0xffffffff);
    };

private:
    std::tuple<std::vector<Components>...> _sets;
    std::vector<uint32_t> _dense;
    std::vector<uint32_t> _sparse;

    [[nodiscard]]
    uint32_t getEntityIndex(uint32_t e)
    {
        assert(_sparse[e] != 0xffffffff);
        return _sparse[e];
    }
};

}
