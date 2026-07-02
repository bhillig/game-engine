#pragma once

#include <ECS/Entity.h>
#include <ECS/EntityMemoryPool.h>

#include <vector>

namespace ECS
{

class EntityManager
{
public:

	EntityManager(size_t entityCount = 0);

	Entity& CreateEntity();

	size_t GetEntityCount() const { return m_entityCount; }

	template<IsComponent T>
	T& GetComponentFromEntity(size_t id)
	{
		return m_entityPool.GetComponent<T>(id);
	}

	template<IsComponent T>
	std::vector<T*> GetComponentsOfType()
	{
		return m_entityPool.GetComponentsOfType<T>();
	}

	Entity& GetEntity(size_t id);

private:
	std::vector<Entity> m_entities;
	EntityMemoryPool m_entityPool;
	size_t m_entityCount;
};

} // namespace ECS