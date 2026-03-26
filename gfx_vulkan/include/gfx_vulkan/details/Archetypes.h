#pragma once

#include <cstdint>
#include <vector>
#include <ecs/Entity.hpp>
#include <cassert>
#include <tuple>


namespace gfx::details {

// template <typename T>
// concept handle = requires(T& handle) {
// 	{ handle.index } ->  std::convertible_to<std::size_t>;
// 	{ handle.generation } -> std::integral;
// };

template <typename T>
concept Component = std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T>;

template <Component... Components>
class Archetype {
public:
	template <Component T>
	[[nodiscard]]
	T& get(ecs::Entity e) {
		assert(has(e));
		return std::get<std::vector<T>>(_sets)[getEntityIndex(e)];
	}

	template<Component T>
	[[nodiscard]]
	std::vector<T>& get() {
		return std::get<std::vector<T>>(_sets);
	}

	template<Component T>
	void set(ecs::Entity e, T value) {
		assert(has(e));
		std::vector<T>& set = std::get<std::vector<T>>(_sets);
		ecs::Entity index = _sparse[e];
		if (set.size() < index)
			set.resize(index + 1);
		set[index] = value;
	}

	void remove(ecs::Entity e) {
		if (!has(e))
			return;
		ecs::Entity index = _sparse[e];
		ecs::Entity last = _dense.size() - 1;

		_dense[index] = _dense[last];
		std::apply([index, last](auto&... set){
				((set[index] = set[last]), ...);
		}, _sets);

		_sparse[_dense[index]] = _dense[last];

		_dense.pop_back();
		std::apply([](auto&... set){
				((set.pop_back()), ...);
		}, _sets);

		_sparse[e] = ecs::INVALID_ENTITY;
	}

	void add(ecs::Entity e) {
        if (_sparse.size() <= e)
            _sparse.resize(e + 1, ecs::INVALID_ENTITY);
		if (has(e))
			return;
		_sparse[e] = _dense.size();
		_dense.push_back(e);

		uint32_t size = _dense.size();
		std::apply([size](auto&... set){
				((set.resize(size)), ...);
		}, _sets);
	}

	[[nodiscard]]
	bool has(ecs::Entity e) {
		return _sparse.size() >= e && _sparse[e] != ecs::INVALID_ENTITY;
	}

private:
	std::tuple<std::vector<Components>...> _sets;
	std::vector<ecs::Entity> _dense;
	std::vector<ecs::Entity> _sparse;

	[[nodiscard]]
	uint32_t getEntityIndex(ecs::Entity e) {
		assert(_sparse[e] != ecs::INVALID_ENTITY);
		return _sparse[e];
	}
};

}



