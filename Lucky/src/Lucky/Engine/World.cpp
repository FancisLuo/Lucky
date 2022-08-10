#include "World.h"

#include "Entity.h"

#include "Components.h"

NAMESPACE_LUCKY_START


World::World()
{
}

World::~World()
{
}

Entity World::CreateEntity(const std::string& name)
{
	UUID uuid;
	return CreateEntityWithUUID(uuid, name);
}

Entity World::CreateEntityWithUUID(UUID uuid, const std::string& name)
{
	return Entity();
}

//Entity World::CreateEntityWithUUID(UUID uuid, const std::string& name)
//{
//	return Entity();
//}

void World::DestroyEntity(Entity entity)
{
}

void World::UpdateWorld(float deltaTime)
{
}

entt::registry World::GetRegistry() const
{
	return entt::registry();
}

//entt::registry World::GetRegistry() const
//{
//	return m_Registry;
//}

NAMESPACE_LUCKY_END