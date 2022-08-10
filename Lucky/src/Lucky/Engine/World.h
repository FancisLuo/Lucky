#pragma once


#include "../../Lucky.h"

#include "entt.hpp"

NAMESPACE_LUCKY_START

class Entity;

// like scene to Unity Engine
class World
{
public:

	World();
	~World();

	Entity CreateEntity(const std::string& name = std::string());
	Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
	void DestroyEntity(Entity entity);

	void UpdateWorld(float deltaTime);

	entt::registry GetRegistry() const;

private:
	/*template<typename T>
	void OnComponentAdded(Entity entity, T& component);*/

private:
	entt::registry m_Registry;
};

NAMESPACE_LUCKY_END

//template<typename T>
//inline void World::OnComponentAdded(Entity entity, T& component)
//{
//}
