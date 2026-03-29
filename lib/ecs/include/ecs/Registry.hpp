#pragma once

#include "Group.hpp"
#include "SSet.hpp"

namespace ecs {

template <Component... Components>
class Registry {
public:
    template <Component T>
    [[nodiscard]] SSet<T>& get() { return std::get<SSet<T>>(_sets); }

    template <Component T>
    void add(uint32_t e, T component)
    {
        std::get<SSet<T>>(_sets).add(e, component);
    }

    template <Component... Owned>
    [[nodiscard]] OwnsGroup<Owned...> createOwnsGroup()
    {
        return OwnsGroup<Owned...>(std::get<SSet<Owned>>(_sets)...);
    };

private:
    std::tuple<SSet<Components>...> _sets;
};

}
