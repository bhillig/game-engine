#pragma once

#include <ECS/Component.h>

#include <tuple>
#include <vector>

namespace ECS
{

template<typename T>
concept IsComponent = std::is_base_of_v<Component, T>;

class EntityMemoryPool
{

public:
	EntityMemoryPool(size_t entityCount = 1);

	void ReserveAll(size_t count);

	void ResizeAll(size_t count);

	template<IsComponent T, typename... Args>
	T& AddComponent(size_t index, Args&&... args)
	{
		std::vector<T>& pool = std::get<std::vector<T>>(m_componentPool);
		if (pool.size() <= index)
		{
			ResizeAll(index + 1);
		}

		pool[index] = T(std::forward<Args>(args)...);
		pool[index].SetActive(true);
		return pool[index];
	}

	template<IsComponent T>
	bool HasComponent(size_t index)
	{
		std::vector<T>& pool = std::get<std::vector<T>>(m_componentPool);
		if (pool.size() <= index)
		{
			return false;
		}

		T& comp = pool[index];
		return comp.IsActive();
	}

	// MUST call HasComponent<T> before calling this
	template<IsComponent T>
	T& GetComponent(size_t index)
	{
		std::vector<T>& pool = std::get<std::vector<T>>(m_componentPool);
		T& comp = pool[index];
		return comp;
	}

	template<IsComponent T>
	bool RemoveComponent(size_t index)
	{
		std::vector<T>& pool = std::get<std::vector<T>>(m_componentPool);
		if (pool.size() <= index)
		{
			return false;
		}

		T& comp = pool[index];
		comp.SetActive(false);
		return true;
	}

	template<IsComponent T>
	std::vector<T>& GetComponentsOfType()
	{
		return std::get<std::vector<T>>(m_componentPool);
	}

	void AddEntity(size_t index);

	bool RemoveEntity(size_t index);

private:
	std::tuple<std::vector<TransformComponent>, std::vector<MeshComponent>, std::vector<BoundingBoxComponent>> m_componentPool;
	std::vector<bool> m_activePool;
};

} // namespace ECS