#pragma once

#include "Group.hpp"
#include "SSet.hpp"

namespace ecs {

/*
 * Hold SSet, one per component
 * need a way to map component to a UID to ordonnate between SSet
 *
 *
 */
template <Component... Components>
class Registry {
public:
    template <Component T>
    [[nodiscard]] SSet<T>& get() { return std::get<SSet<T>>(_sets); }

    template <Component T>
    void add(Entity e, T component)
    {
        std::get<SSet<T>>(_sets).add(e, component);
    }

    [[nodiscard]] Entity newEntity()
    {
        _entitiesCount += 1;
        return _entitiesCount - 1;
    };

    template <Component... Owned>
    [[nodiscard]] OwnsGroup<Owned...> createOwnsGroup()
    {
        return OwnsGroup<Owned...>(std::get<SSet<Owned>>(_sets)...);
    };

private:
    std::tuple<SSet<Components>...> _sets;

    uint32_t _entitiesCount = 0;
};

}
