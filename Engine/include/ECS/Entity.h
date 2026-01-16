#pragma once

#include <ECS/EntityMemoryPool.h>

namespace ECS
{

class Entity
{
public:

	size_t GetID() const { return m_id; }

	// Add Component
	template<IsComponent T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		assert(m_pool);
		return m_pool->AddComponent<T>(m_id, std::forward<Args>(args)...);
	}

	// Has Component
	template<IsComponent T>
	bool HasComponent()
	{
		assert(m_pool);
		return m_pool->HasComponent<T>(m_id);
	}

	// Get Component 
	template<IsComponent T>
	T& GetComponent()
	{
		assert(m_pool);
		return m_pool->GetComponent<T>(m_id);
	}

	// Remove Component 
	template<IsComponent T>
	bool RemoveComponent()
	{
		assert(m_pool);
		return m_pool->RemoveComponent<T>(m_id);
	}

	// Destroy entity
	void Destroy() { m_pool->RemoveEntity(m_id); }

private:
	Entity(size_t id, EntityMemoryPool* pool)
		: m_id(id), m_pool(pool) {}

	size_t m_id;
	EntityMemoryPool* m_pool;

	friend class EntityManager;
};

} // namespace ECS