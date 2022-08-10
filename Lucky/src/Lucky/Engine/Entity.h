#pragma once

#include "../../Lucky.h"

#include "World.h"

NAMESPACE_LUCKY_START

class Entity
{
public:
	Entity() = default;
	Entity(entt::entity handle, World* world);
	Entity(const Entity& other) = default;

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		T& component = m_World->GetRegistry().emplace(m_EntityHandle, std::forward<Args>(args)...);

		return component;
	}

	template<typename T, typename... Args>
	T& AddOrReplaceComponent(Args&&... args)
	{
		T& component = m_World->GetRegistry().emplace_or_replace(m_EntityHandle, std::forward<Args>(args)...);
		return component;
	}

	template<typename T>
	T& GetComponent()
	{
		return m_World->GetRegistry().get<T>(m_EntityHandle);
	}

	template<typename T>
	bool HasComponent()
	{
		return m_World->GetRegistry().any_of<T>(m_EntityHandle);
	}

	bool operator==(const Entity& other) const
	{
		return m_EntityHandle == other.m_EntityHandle && m_World == other.m_World;
	}

	bool operator!=(const Entity& other) const
	{
		return !(*this == other);
	}

private:
	entt::entity m_EntityHandle{ entt::null };
	World* m_World = nullptr;
};

NAMESPACE_LUCKY_END
