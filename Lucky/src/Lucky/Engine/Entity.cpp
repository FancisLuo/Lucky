#include "Entity.h"

NAMESPACE_LUCKY_START

Entity::Entity(entt::entity handle, World* world): m_EntityHandle(handle), m_World(world)
{

}

NAMESPACE_LUCKY_END
