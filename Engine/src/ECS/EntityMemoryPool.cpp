#include <ECS/EntityMemoryPool.h>

namespace ECS
{

EntityMemoryPool::EntityMemoryPool(size_t entityCount)
{
	ReserveAll(entityCount);
}

void EntityMemoryPool::ReserveAll(size_t count)
{
	// For all types reserve count
	std::get<0>(m_componentPool).reserve(count); // TransformComponent
	std::get<1>(m_componentPool).reserve(count); // MeshComponent
	std::get<2>(m_componentPool).reserve(count); // BoundingBoxComponent
	m_activePool.reserve(count);
}

void EntityMemoryPool::ResizeAll(size_t count)
{
	if (count <= m_activePool.size())
	{
		return;
	}

	// For all types resize count
	std::get<0>(m_componentPool).resize(count); // TransformComponent
	std::get<1>(m_componentPool).resize(count); // MeshComponent
	std::get<2>(m_componentPool).resize(count); // BoundingBoxComponent
	m_activePool.resize(count);
}

void EntityMemoryPool::AddEntity(size_t index)
{
	if (m_activePool.size() <= index)
	{
		ResizeAll(index + 1);
	}

	m_activePool[index] = true;
}

bool EntityMemoryPool::RemoveEntity(size_t index)
{
	if (m_activePool.size() <= index)
	{
		return false;
	}

	// For all types
	std::get<0>(m_componentPool)[index].SetActive(false); // TransformComponent
	std::get<1>(m_componentPool)[index].SetActive(false); // MeshComponent
	std::get<2>(m_componentPool)[index].SetActive(false); // BoundingBoxComponent
	m_activePool[index] = false;
	return true;
}

} // namespace ECS